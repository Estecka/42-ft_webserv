/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/14 14:37:09 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "PollManager.hpp"
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
		_port(port), _code(200) {
		_clientIP = ip_fd.ip;
		std::cerr << "[DEBUG] RequestHandler created." << std::endl;
		fcntl(ip_fd.acceptfd, F_SETFL, O_NONBLOCK);
		this->PollInit();
	}

	RequestHandler::~RequestHandler() {
		if (this->_subPollListener)
			delete _subPollListener;
		if (this->_header)
			delete _header;
		std::cerr << "[DEBUG] RequestHandler destroyed." << std::endl;
	}


	void	RequestHandler::GetPollFd(pollfd& outpfd) {
		if (_subPollListener)
			return _subPollListener->GetPollFd(outpfd);
		else
			outpfd = this->_pollfd;
	}
	void	RequestHandler::OnPollEvent(const pollfd& pfd) {
		if (_subPollListener)
			return _subPollListener->OnPollEvent(pfd);
		else
			return (this->*_onPollEvent)(pfd);
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



	void	RequestHandler::PollInit(){
		this->SetPollEvent(new ReqHeadExtractor(*this, httpin));
		PollManager::AddListener(*this);
	}

	void	RequestHandler::OnHeaderExtracted(HttpRequest* req) {
		this->_header = req;
		if (req == NULL) {
			std::cerr << "[WARN] Empty request received on port " << _port << std::endl;
			this->SetPollEvent(httpout.fd, POLLOUT, &RequestHandler::DispatchRequest);
		}
		else {
			this->SetPollEvent(httpin.fd, POLLIN, &RequestHandler::ExtractRequestBody);
			this->OnPollEvent(_pollfd);
		}
	}

	// Placeholder function. For now, the request body is discarded.
	void	RequestHandler::ExtractRequestBody(const pollfd&){
		httpin.clear();
		while (true) {
			httpin.ignore(1024);

			std::cerr << "[DEBUG] Discarding request body: " << httpin.gcount() << " bytes.\n"
			          << "        Fail: " << httpin.fail() << ", Eof: " << httpin.eof()
			          << std::endl;

			if (httpin.fail())
				break;
		}

		_pollfd.fd = httpout.fd;
		_pollfd.events = POLLOUT;
		this->SetPollEvent(httpin.fd, POLLOUT, &RequestHandler::CheckRequest);
		PollManager::SetDirty();
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

