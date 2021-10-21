/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/21 18:40:18 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "HttpCode.hpp"
#include "PollManager.hpp"
#include "ReqBodyExtractor.hpp"
#include "TimeoutManager.hpp"
#include "ReqHeadExtractor.hpp"
#include "ErrorPage.hpp"
#include "Server.hpp"
#include "Methods.hpp"

#include <cstdlib>

namespace ft
{
	RequestHandler::RequestHandler(fd_ip ip_fd, int port) :
		httpin(ip_fd.acceptfd),
		httpout(ip_fd.acceptfd),
		_subPollListener(NULL),
		_port(port),
		_clientIP(ip_fd.ip),
		_header(NULL),
		_body(NULL)
	{
		std::clog << "[DEBUG] RequestHandler created." << std::endl;
		fcntl(ip_fd.acceptfd, F_SETFL, O_NONBLOCK);
		this->PollInit();
	}

	RequestHandler::~RequestHandler() {
		PollManager::RemoveListener(*this);
		TimeoutManager::RemoveListener(*this);
		if (this->_subPollListener)
			delete _subPollListener;
		if (this->_header)
			delete _header;
		if (this->_body)
			std::fclose(_body);
		std::clog << "[DEBUG] RequestHandler destroyed." << std::endl;
	}



/******************************************************************************/
/* ## Listener interfaces                                                     */
/******************************************************************************/

	void	RequestHandler::OnTimeout(){
		std::clog << "[WARN] Request took too long to execute. Aborting" << std::endl;
		delete this;
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
			catch (const std::exception& e) {
				std::clog << "[ERR] Exception occured while processing request: " << e.what() << std::endl;
				delete this;
			}
		}
		else {
			std::clog << "[ERR] No sub-PollListener to handle the request." << std::endl;
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
	const UriConfig&    	RequestHandler::GetConfig()   const { return this->_config;   }


	void	RequestHandler::SetPollEvent(IPollListener* sublistener){
		if (this->_subPollListener) {
			delete _subPollListener;
		}
		this->_subPollListener = sublistener;
		PollManager::SetDirty();
	}

	void	RequestHandler::SendErrCode(int code){
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
			std::clog << "[WARN] Empty request received on port " << _port << std::endl;
			this->CheckRequest();
		}
		else {
			this->SetPollEvent(new ReqBodyExtractor(*this));
			this->OnPollEvent(_pollfd);
		}
	}

	void	RequestHandler::OnBodyExtracted(FILE* body){
		this->_body = body;
		if (!body)
			std::clog << "[INFO] The request doesn't appear to have a body." << std::endl;
		this->CheckRequest();
	}

	void	RequestHandler::CheckRequest() {
		if (_header == NULL)
			return SendErrCode(HTTP_TEAPOT);
		else if (!_header->IsOk()) {
			std::clog << "[WARN] Invalid request received on port " 
			          << this->_port << std::endl;
			return SendErrCode(HTTP_BAD_REQUEST);
		}
		else if(_header->GetHostPort() != this->_port) {
			std::clog << "[WARN] Port mismatch: got " << _header->GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			return SendErrCode(HTTP_UNPROCESSABLE);
		}
		else if (_header->GetMajorHttpVersion() != 1 || _header->GetMinorHttpVersion() != 1)
			return SendErrCode(HTTP_V_UNSUPPORTED);
		else
			return this->DispatchRequest();
	}

	void	RequestHandler::DispatchRequest() {
		bool serverfound = false;
		for (std::list<ft::Server>::iterator it=Server::availableServers.begin(); it!=Server::availableServers.end(); it++)
		if (it->MatchRequest(*_header)) {
			this->_config = it->Accept(*_header);
			serverfound = true;

			const std::string& met = _header->GetMethod();
			if (met!="GET" && met!="DELETE" && met!="POST")
				return SendErrCode(HTTP_NOT_IMPLEMENTED);
			else
				break;
		}

		if (!serverfound) {
			std::clog << "[ERR] No server found to answer request at: " << _header->GetHost() << std::endl;
			return SendErrCode(HTTP_NOT_FOUND);
		}
		else
			return this->SetPollEvent(new Methods(_config, *_header, httpin.fd, *(this)));
	}

	void	RequestHandler::Destroy() {
		std::clog << "[DEBUG] Destroy called" << std::endl;
		PollManager::RemoveListener(*this);
		delete this;
	}
}

