/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/10 18:04:46 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "PollManager.hpp"

#include <cstdlib>

namespace ft
{
	RequestHandler::RequestHandler(int acceptfd, int port) :
		httpin  (acceptfd),
		httpout (acceptfd),
		_port   (port    ) {
		std::cerr << "[DEBUG] RequestHandler created." << std::endl;
		fcntl(acceptfd, F_SETFL, O_NONBLOCK);
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
		// To be implemented.
		PollManager::RemoveListener(*this);
		delete this;
	}
	
}