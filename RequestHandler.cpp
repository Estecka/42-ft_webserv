/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 11:50:26 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "PollManager.hpp"

#include <cstdlib>

namespace ft
{
	RequestHandler::RequestHandler(fd_ip ip_fd, int port) :
		httpin(ip_fd.acceptfd),
		httpout(ip_fd.acceptfd),
		_port(port) {
		_clientIP = ip_fd.ip;
		std::cerr << "[DEBUG] RequestHandler created." << std::endl;
		fcntl(ip_fd.acceptfd, F_SETFL, O_NONBLOCK);
		this->PollInit();
	}

	RequestHandler::~RequestHandler() {
		if (this->_header)
			delete _header;
		std::cerr << "[DEBUG] RequestHandler destroyed." << std::endl;
	}


	void	RequestHandler::GetPollFd(pollfd& outpfd) {
		outpfd = this->_pollfd;
	}
	void	RequestHandler::OnPollEvent(const pollfd& pfd) {
		(this->*_onPollEvent)(pfd);
	}



	void	RequestHandler::PollInit(){
		_pollfd.fd      = httpin.fd;
		_pollfd.events  = POLLIN;
		_pollfd.revents = 0;
		_onPollEvent    = &RequestHandler::ExtractRequestHeader;

		PollManager::AddListener(*this);
	}

	void	RequestHandler::ExtractRequestHeader(const pollfd& pfd){
		std::string	line;

		httpin.clear();
		while (true) {
			std::getline(httpin, line);
			_stringbuff << line;

			std::cerr << "[DEBUG] Reading Request Header: " << BitToCString(line) << "\n"
			          << "        Fail: " << httpin.fail() << ", Eof: " << httpin.eof()
			          << std::endl;

			if (httpin.eof())
				break;
			if (httpin.fail())
				return;
			else {
				_stringbuff << '\n';
				if (line == "" || line == "\r")
					break;
			}
		}

		if (_stringbuff.str().length() > 0) {
			std::cout << _stringbuff.str() << std::endl;
			this->_header = new HttpRequest(_stringbuff);
		}
		else {
			std::cerr << "[WARN] Empty request received on port " << this->_port << std::endl;
			this->_header = NULL;
		}

		_stringbuff.str("");
		_stringbuff.clear();
		_onPollEvent = &RequestHandler::ExtractRequestBody;
		this->ExtractRequestBody(pfd);
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
		_onPollEvent = &RequestHandler::DispatchRequest;
		PollManager::SetDirty();
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
