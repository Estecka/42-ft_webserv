/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/16 17:06:53 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "PollManager.hpp"
#include "ReqHeadExtractor.hpp"
#include "ReqBodyExtractor.hpp"
#include "TimeoutManager.hpp"

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
		_body(NULL) {
		std::cerr << "[DEBUG] RequestHandler created." << std::endl;
		fcntl(ip_fd.acceptfd, F_SETFL, O_NONBLOCK);
		this->PollInit();
	}

	RequestHandler::~RequestHandler() {
		if (this->_subPollListener)
			delete _subPollListener;
		if (this->_header)
			delete _header;
		if (this->_body)
			std::fclose(_body);
		std::cerr << "[DEBUG] RequestHandler destroyed." << std::endl;
	}

	void	RequestHandler::OnTimeout(){
		std::clog << "[WARN] Request took too long to execute. Aborting" << std::endl;
		PollManager::RemoveListener(*this);
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
			std::cerr << "[ERR] Exception occured while processing request: " << e.what() << std::endl;
			PollManager::RemoveListener(*this);
			delete this;
		}
	}

	void	RequestHandler::SetPollEvent(IPollListener* sublistener){
		if (this->_subPollListener)
			delete _subPollListener;
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
		TimeoutManager::AddListener(*this, 5);
	}

	void	RequestHandler::OnHeaderExtracted(HttpRequest* req) {
		this->_header = req;
		if (req == NULL) {
			std::cerr << "[WARN] Empty request received on port " << _port << std::endl;
			this->SetPollEvent(httpout.fd, POLLOUT, &RequestHandler::DispatchRequest);
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
		this->SetPollEvent(httpout.fd, POLLOUT, &RequestHandler::DispatchRequest);
	}

	void	RequestHandler::DispatchRequest(const pollfd&){
		if (_header == NULL)
			HttpHeader::SendErrCode(418, httpin.fd);
		else if (!_header->IsOk())
		{
			std::cerr << "[WARN] Invalid request received on port " 
			          << this->_port << std::endl;
			HttpHeader::SendErrCode(400, httpin.fd);
		}
		else if(_header->GetHostPort() != this->_port)
		{
			std::cerr << "[WARN] Port mismatch: got " << _header->GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			HttpHeader::SendErrCode(422, httpin.fd);
		}
		else if (_header->GetMajorHttpVersion() != 1 || _header->GetMinorHttpVersion() != 1)
			HttpHeader::SendErrCode(505, httpin.fd);
		else
		{
			bool serverfound = false;
			for (std::list<ft::Server>::iterator it=Server::availableServers.begin(); it!=Server::availableServers.end(); it++)
				if (it->MatchRequest(*_header)) {
					it->Accept(httpin.fd, *_header, _clientIP);
					serverfound = true;
					break;
				}
			if (!serverfound) {
				std::cerr << "[ERR] No server found to answer request at: " << _header->GetHost() << std::endl;
				HttpHeader::SendErrCode(404, httpin.fd);
			}
		}
		PollManager::RemoveListener(*this);
		delete this;
	}

}
