/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestReadPollListener.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 17:13:38 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 17:39:35 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestReadPollListener.hpp"

#include "HttpRequest.hpp"
#include "PollManager.hpp"

#include <iostream>
#include <sstream>

#include <unistd.h>

namespace ft
{
	std::list<ft::Server>*	RequestReadPollListener::_availableServers = NULL;


	RequestReadPollListener::RequestReadPollListener(int acceptfd, int port) {
		this->acceptfd = acceptfd;
		this->port     = port;
	}
	RequestReadPollListener::~RequestReadPollListener(){
	}

	void	RequestReadPollListener::GetPollFd(struct pollfd& outpfd){
		outpfd.fd      = this->acceptfd;
		outpfd.events  = POLLIN;
		outpfd.revents = 0;
	}

	void	RequestReadPollListener::OnPollEvent(const struct pollfd&) {
		PollManager::RemoveListener(*this);

		// Receive request
		std::stringstream	input;
		char buffer[1025];
		ssize_t bufflen;

		bufflen = read(acceptfd, buffer, 1024);
		buffer[bufflen] = '\0';
		input << buffer;

		if (input.str().length() == 0) {
			std::cerr << "[WARN] Empty request on port " << this->port << std::endl;
			// Send 400 here ?
			// ...
			close(acceptfd);
			delete this;
			return;
		}
		std::cout << '\n' << input.str() << std::endl;


		// Parse request.
		HttpRequest req(input);
		bool	serverfound = false;
		if (!req.IsOk()) {
			std::cerr << "[WARN] Invalid request received on port " << this->port << std::endl;
			// Need to return a code 400 to the client here.
			// ...
			close(acceptfd);
		}
		// Dispatch request to server.
		else for (ServList::iterator it=_availableServers->begin(); it!=_availableServers->end(); it++) {
			if (it->MatchRequest(req)) {
				it->Accept(acceptfd, req);
				serverfound = true;
				break;
			}
		}

		if (!serverfound) {
			std::cerr << "[ERR] No server found to answer request at: " << req.GetHost() << std::endl;
			// Need to return some error code. Probably 404 ?
			// ...
			close(acceptfd);
		}

		delete this;
		close(acceptfd);
	}

}
