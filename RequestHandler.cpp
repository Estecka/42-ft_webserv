/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/17 16:03:30 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

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
		_body(NULL),
		_code(200)
	{
		std::cerr << "[DEBUG] RequestHandler created." << std::endl;
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
		std::cerr << "[DEBUG] RequestHandler destroyed." << std::endl;
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
		try {
			if (_subPollListener)
				return _subPollListener->OnPollEvent(pfd);
			else
				return (this->*_onPollEvent)(pfd);
		}
		catch (const std::exception& e) {
			std::clog << "[ERR] Exception occured while processing request: " << e.what() << std::endl;
			delete this;
		}
	}



/******************************************************************************/
/* ## Accessors                                                               */
/******************************************************************************/

	const HttpRequest*	RequestHandler::GetReqHead() const {
		return this->_header;
	}

	void	RequestHandler::SetPollEvent(IPollListener* sublistener){
		if (this->_subPollListener) {
			delete _subPollListener;
		}
		this->_subPollListener = sublistener;
		this->_onPollEvent     = NULL;
		PollManager::SetDirty();
	}
	void	RequestHandler::SetPollEvent(int fd, short event, void(RequestHandler::*function)(const pollfd&)) {
		if (this->_subPollListener)
			delete _subPollListener;
		this->_subPollListener = NULL;
		this->_onPollEvent     = function;
		this->_pollfd.fd = fd;
		this->_pollfd.events = event;
		PollManager::SetDirty();
	}



/******************************************************************************/
/* ## Handling process                                                        */
/******************************************************************************/

	void	RequestHandler::PollInit(){
		this->SetPollEvent(new ReqHeadExtractor(*this, httpin));
		PollManager::AddListener(*this);
		TimeoutManager::AddListener(*this, 5);
	}

	void	RequestHandler::OnHeaderExtracted(HttpRequest* req) {
		this->_header = req;
		if (req == NULL) {
			std::cerr << "[WARN] Empty request received on port " << _port << std::endl;
			this->SetPollEvent(httpout.fd, POLLOUT, &RequestHandler::CheckRequest);
		}
		else {
			this->SetPollEvent(new ReqBodyExtractor(*this));
			this->OnPollEvent(_pollfd);
		}
	}

	void	RequestHandler::OnBodyExtracted(FILE* body){
		this->_body = body;
		if (!body)
			std::cerr << "[INFO] The request doesn't appear to have a body." << std::endl;
		this->SetPollEvent(httpout.fd, POLLOUT, &RequestHandler::CheckRequest);
	}

	void	RequestHandler::CheckRequest(const pollfd&) {
		if (_header == NULL)
			_code = 418;
		else if (!_header->IsOk()) {
			std::cerr << "[WARN] Invalid request received on port " 
			          << this->_port << std::endl;
			_code = 400;
		}
		else if(_header->GetHostPort() != this->_port) {
			std::cerr << "[WARN] Port mismatch: got " << _header->GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			_code = 422;
		}
		else if (_header->GetMajorHttpVersion() != 1 || _header->GetMinorHttpVersion() != 1)
			_code = 505;
		if (_code == 200)
			this->SetPollEvent(httpin.fd, POLLOUT, &RequestHandler::DispatchRequest);
		else
			this->SetPollEvent(new ErrorPage(_code, httpin.fd, *(this)));
	}

	void	RequestHandler::DispatchRequest(const pollfd&) {
		bool serverfound = false;
		for (std::list<ft::Server>::iterator it=Server::availableServers.begin(); it!=Server::availableServers.end(); it++) {
			if (it->MatchRequest(*_header)) {
				_config = it->Accept(*_header);
				if (_header->GetMethod() != "GET"
					&& _header->GetMethod() != "DELETE"
					&& _header->GetMethod() != "POST")
					_code = 404;
				serverfound = true;
				break;
			}
		}
		if (!serverfound) {
			std::cerr << "[ERR] No server found to answer request at: " << _header->GetHost() << std::endl;
			_code = 404;
		}
		if (_code == 200)
			this->SetPollEvent(new Methods(_config, *_header, httpin.fd, *(this)));
		else
			this->SetPollEvent(new ErrorPage(_code, httpin.fd, *(this)));
	}

	void	RequestHandler::Destroy() {
		std::cerr << "[DEBUG] Destroy called" << std::endl;
		PollManager::RemoveListener(*this);
		delete this;
	}
}

