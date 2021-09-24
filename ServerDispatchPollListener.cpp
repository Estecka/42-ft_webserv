/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerDispatchPollListener.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 18:25:14 by abaur             #+#    #+#             */
/*   Updated: 2021/09/24 12:09:12 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerDispatchPollListener.hpp"

#include "PollManager.hpp"
#include "SimpleHttpResponse.hpp"

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
		this->_requestReceived = false;
	}
	ServerDispatchPollListener::~ServerDispatchPollListener(){
		close(this->_acceptfd);
		if (this->_request != NULL)
			delete this->_request;
		this->_request = NULL;
	}

	void	ServerDispatchPollListener::GetPollFd(struct pollfd& outpfd){
		outpfd.fd      = this->_acceptfd;
		outpfd.events  = (this->_request==NULL) ? POLLIN : POLLOUT;
		outpfd.revents = 0;
	}

	void ServerDispatchPollListener::OnPollEvent(const struct pollfd&){
		if (!_requestReceived) {
			this->ReadRequest();
			this->_requestReceived = true;
			PollManager::SetDirty();
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
		std::cout << "[TEST]"  << input.str() << std::endl;
		std::cout << '\n' << input.str() << std::endl;
		if (input.str().empty())
			std::cerr << "[WARN] Empty request received on port " << _port << std::endl;
		else
			this->_request = new HttpRequest(input);
	}
	void ServerDispatchPollListener::DispatchRequest() {
		SimpleHttpResponse resp;
		if (_request == NULL)
		{
			resp.Setcode(418);
			resp.ToFd(_acceptfd);
		}
		else if (!_request->IsOk())
		{
			std::cerr << "[WARN] Invalid request received on port " 
			          << this->_port << std::endl;
			resp.Setcode(400);
			resp.ToFd(_acceptfd);
		}
		else if(_request->GetHostPort() != this->_port)
		{
			std::cerr << "[WARN] Port mismatch: got " << _request->GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			resp.Setcode(422);
			resp.ToFd(_acceptfd);
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
				resp.Setcode(404);
				resp.ToFd(_acceptfd);
			}
		}
		return;
	}

}
