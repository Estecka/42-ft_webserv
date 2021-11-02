/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/11/02 15:55:28 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "HttpCode.hpp"
#include "HttpException.hpp"
#include "PollManager.hpp"
#include "ReqBodyExtractor.hpp"
#include "TimeoutManager.hpp"
#include "ReqHeadExtractor.hpp"
#include "ErrorPage.hpp"
#include "Server.hpp"
#include "Methods.hpp"
#include "logutil/logutil.hpp"

#include <cstdlib>

namespace ft
{
	RequestHandler::RequestHandler(int acceptfd, std::string clientIp, int port) :
		httpin (acceptfd),
		httpout(acceptfd),
		_subPollListener(NULL),
		_port(port),
		_clientIP(clientIp),
		_header(NULL),
		_body(NULL),
		_streamingStarted(false)
	{
		ft::clog << log::notice << this << " RequestHandler created." << std::endl;
		fcntl(acceptfd, F_SETFL, O_NONBLOCK);
		this->PollInit();
	}

	RequestHandler::~RequestHandler() {
		PollManager::RemoveListener(*this, false);
		TimeoutManager::RemoveListener(*this, false);
		if (this->_subPollListener)
			delete _subPollListener;
		if (this->_header)
			delete _header;
		if (this->_body)
			std::fclose(_body);
		ft::clog << log::notice << this << " RequestHandler destroyed." << std::endl;
	}



/******************************************************************************/
/* ## Listener interfaces                                                     */
/******************************************************************************/

	void	RequestHandler::OnTimeout(){
		ft::clog << log::error << "Request took too long to execute.\n";
		if (!_streamingStarted){
			ft::clog << "Corresponding error page will be sent" << std::endl;
			TimeoutManager::AddListener(*this, 5);
			return SendErrCode(HTTP_REQ_TIMEOUT);
		}
		else {
			ft::clog << "Forcibly aborting connection." << std::endl;
			delete this;
		}
	}

	void	RequestHandler::GetPollFd(pollfd& outpfd) {
		if (_subPollListener)
			return _subPollListener->GetPollFd(outpfd);
		else
			outpfd = this->_pollfd;
	}
	void	RequestHandler::OnPollEvent(const pollfd& pfd) {
		if (_subPollListener){
			try {
				return _subPollListener->OnPollEvent(pfd);
			}
			catch (const ft::HttpException& e) {
				if (!_streamingStarted) {
					ft::clog << log::warning << "Caught HttpException, sending "
						"corresponding error page.\n" << e.what() << std::endl;
					this->SendErrCode(e.GetHttpCode());
				}
				else {
					ft::clog << log::error << "Caught an HttpException at a point "
						"where error page could not be sent.\n" << e.what() << std::endl;
					delete this;
				}
			}
			catch (const std::exception& e) {
				ft::clog << log::error << "Unmanaged exception occured while processing request: " << e.what() << std::endl;
				if(!_streamingStarted)
					this->SendErrCode(HTTP_INTERNAL_ERROR);
				else
					delete this;
			}
		}
		else {
			ft::clog << log::error << "No sub-PollListener to handle the request." << std::endl;
			delete this;
		}
	}



/******************************************************************************/
/* ## Accessors                                                               */
/******************************************************************************/

	int                 	RequestHandler::GetPort()     const { return this->_port;     }
	const std::string&  	RequestHandler::GetClientIp() const { return this->_clientIP; }
	const RequestHeader*	RequestHandler::GetReqHead()  const { return this->_header;   }
	const FILE*         	RequestHandler::GetReqBody()  const { return this->_body;     }
	size_t              	RequestHandler::GetBodyLen()  const { return this->_bodyLen;  }
	const UriConfig&    	RequestHandler::GetConfig()   const { return this->_config;   }

	void	RequestHandler::SetStreamingStarted() { this->_streamingStarted = true; }

	void	RequestHandler::SetPollEvent(IPollListener* sublistener){
		if (this->_subPollListener) {
			delete _subPollListener;
		}
		this->_subPollListener = sublistener;
		PollManager::SetDirty();
	}

	void	RequestHandler::SendErrCode(int code){
		_streamingStarted = true;
		this->SetPollEvent(new ErrorPage(code, httpout.fd, *this));
	}



/******************************************************************************/
/* ## Handling process                                                        */
/******************************************************************************/

	void	RequestHandler::PollInit(){
		this->SetPollEvent(new ReqHeadExtractor(*this, httpin));
		PollManager::AddListener(*this);
		TimeoutManager::AddListener(*this, 5);
	}

	void	RequestHandler::OnHeaderExtracted(RequestHeader* req) {
		this->_header = req;
		if (req == NULL) {
			ft::clog << log::warning << "Empty request received on port " << _port << std::endl;
			return SendErrCode(HTTP_TEAPOT);
		}
		else {
			this->SetPollEvent(new ReqBodyExtractor(*this));
			this->OnPollEvent(_pollfd);
		}
	}

	void	RequestHandler::OnBodyExtracted(FILE* body, size_t bodylen){
		this->_body = body;
		this->_bodyLen = bodylen;
		if (!body)
			ft::clog << log::info << "The request doesn't appear to have a body." << std::endl;
		this->DispatchRequest();
	}

	void	RequestHandler::DispatchRequest() {
		if (!_header->IsOk()) {
			ft::clog << log::warning << "Invalid request received on port " 
			          << this->_port << std::endl;
			return SendErrCode(HTTP_BAD_REQUEST);
		}
		else if(_header->GetHostPort() != this->_port) {
			ft::clog << log::warning << "Port mismatch: got " << _header->GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			return SendErrCode(HTTP_UNPROCESSABLE);
		}
		else if (_header->GetMajorHttpVersion() != 1 || _header->GetMinorHttpVersion() != 1)
			return SendErrCode(HTTP_V_UNSUPPORTED);

		const std::string& met = _header->GetMethod();
		if (met!="GET" && met!="DELETE" && met!="POST")
				return SendErrCode(HTTP_NOT_IMPLEMENTED);

		bool serverfound = false;
		for (std::list<ft::Server>::iterator it=Server::availableServers.begin(); it!=Server::availableServers.end(); it++)
		if (it->MatchRequest(*_header)) {
			this->_config = it->Accept(*_header);
			serverfound = true;
			break;
		}

		if (!serverfound) {
			ft::clog << log::warning << "No server found to answer request at: " << _header->GetHost() << std::endl;
			return SendErrCode(HTTP_NOT_FOUND);
		}
		else
			return this->SetPollEvent(new Methods(_config, *_header, httpin.fd, *(this), _body));
	}

	void	RequestHandler::Destroy() {
		PollManager::RemoveListener(*this);
		delete this;
	}
}

