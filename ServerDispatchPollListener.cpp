/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerDispatchPollListener.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 18:25:14 by abaur             #+#    #+#             */
/*   Updated: 2021/09/22 15:25:14 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerDispatchPollListener.hpp"

#include "PollManager.hpp"

#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>

namespace ft
{
	std::list<ft::Server>*	ServerDispatchPollListener::_availableServers = NULL;

	ServerDispatchPollListener::ServerDispatchPollListener(int acceptfd, int port) {
		this->_acceptfd = acceptfd;
		this->_port     = port;
		this->_request  = NULL;
	}
	ServerDispatchPollListener::~ServerDispatchPollListener(){
		close(this->_acceptfd);
		if (this->_request)
			delete this->_request;
		this->_request = NULL;
		
	}

	void	ServerDispatchPollListener::GetPollFd(struct pollfd& outpfd){
		outpfd.fd      = this->_acceptfd;
		outpfd.events  = (this->_request==NULL) ? POLLIN : POLLOUT;
		outpfd.revents = 0;
	}

	void ServerDispatchPollListener::OnPollEvent(const struct pollfd&){
		if (_request == NULL) {
			this->ReadRequest();
			PollManager::SetDity();
		}
		else {
			this->DispatchRequest();
			PollManager::RemoveListener(*this);
			delete this;
		}
	}


	void ServerDispatchPollListener::ReadRequest(){
		std::stringstream	input;
		char buffer[1025] = {0};
		ssize_t bufflen;

		fcntl(_acceptfd, F_SETFL, O_NONBLOCK);
		while(0 < (bufflen=read(_acceptfd, buffer, 1024))) {
			buffer[bufflen] = '\0';
			input << buffer;
		}

		std::cout << '\n' << input.str() << std::endl;
		if (input.str().empty())
			std::cerr << "[WARN] Empty request received on port " << _port << std::endl;

		this->_request = new HttpRequest(input);
	}
	void ServerDispatchPollListener::DispatchRequest() {
		if (!_request->IsOk())
		{
			std::cerr << "[WARN] Invalid request received on port " 
			          << this->_port << std::endl;
			// Need to return a code 400 to the client here.
			// ...
		}
		else if(_request->GetHostPort() != this->_port)
		{
			std::cerr << "[WARN] Port mismatch: got " << _request->GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			// 400...
		}
		else
		{
			bool serverfound = false;
			for (ServList::iterator it=_availableServers->begin(); it!=_availableServers->end(); it++)
				if (it->MatchRequest(*_request)) {
					it->Accept(_acceptfd, *_request);
					serverfound = true;
					break;
				}
			if (!serverfound) {
				std::cerr << "[ERR] No server found to answer request at: " << _request->GetHost() << std::endl;
				// Need to return some error code. Probably 404 ?
				// ...
			}
		}
		return;
	}

}
