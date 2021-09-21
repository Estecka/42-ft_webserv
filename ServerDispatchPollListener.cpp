/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerDispatchPollListener.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 18:25:14 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 19:03:39 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerDispatchPollListener.hpp"

#include "PollManager.hpp"

#include <unistd.h>

namespace ft
{
	std::list<ft::Server>*	ServerDispatchPollListener::_availableServers = NULL;

	ServerDispatchPollListener::ServerDispatchPollListener(int acceptfd, int port, const std::string& rawRequest) {
		this->_acceptfd   = acceptfd;
		this->_port       = port;
		this->_rawRequest = rawRequest;
		if (rawRequest.empty())
			std::cerr << "[WARN] Empty request on port " << port << std::endl;
	}
	ServerDispatchPollListener::~ServerDispatchPollListener(){
		close(this->_acceptfd);
	}

	void	ServerDispatchPollListener::GetPollFd(struct pollfd& outpfd){
		outpfd.fd      = this->_acceptfd;
		outpfd.events  = POLLOUT;
		outpfd.revents = 0;
	}

	void ServerDispatchPollListener::OnPollEvent(const struct pollfd&){
		PollManager::RemoveListener(*this);

		if (this->_rawRequest.empty()) {
			// Send 400 here ?
			// ...
			delete this;
			return;
		}
		std::cout << '\n' << _rawRequest << std::endl;


		// Parse request.
		HttpRequest req(_rawRequest);
		bool	serverfound = false;
		if (!req.IsOk()) {
			std::cerr << "[WARN] Invalid request received on port " 
			          << this->_port << std::endl;
			// Need to return a code 400 to the client here.
			// ...
			delete this;
			return;
		}
		else if(req.GetHostPort() != this->_port) {
			std::cerr << "[WARN] Port mismatch: got " << req.GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			// 400...
			delete this;
			return;
		}
		// Dispatch request to server.
		else for (ServList::iterator it=_availableServers->begin(); it!=_availableServers->end(); it++) {
			if (it->MatchRequest(req)) {
				it->Accept(_acceptfd, req);
				serverfound = true;
				break;
			}
		}

		if (!serverfound) {
			std::cerr << "[ERR] No server found to answer request at: " << req.GetHost() << std::endl;
			// Need to return some error code. Probably 404 ?
			// ...
		}

		delete this;
		return;
	}
}
