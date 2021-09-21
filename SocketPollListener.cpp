/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollListener.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:51:17 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 16:18:20 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollListener.hpp"

#include "HttpRequest.hpp"

#include <iostream>
#include <sstream>

#include <cerrno>
#include <cstring>
#include <unistd.h>

namespace ft
{
	std::list<ft::Server>*	SocketPollListener::_availableServers = NULL;


	SocketPollListener::SocketPollListener(ft::Socket& sock) 
	: _sock(sock)
	{
	}

	SocketPollListener::SocketPollListener(const SocketPollListener& other)
	: _sock(other._sock)
	{
	}
	SocketPollListener::~SocketPollListener()
	{
	}


	void	SocketPollListener::GetPollFd(struct pollfd& outpfd) {
		outpfd.fd      = this->_sock.GetSocketFD();
		outpfd.events  = POLLIN;
		outpfd.revents = 0;
	}

	void	SocketPollListener::OnPollEvent(const struct pollfd& pfd)
	{
		if (!(pfd.revents & POLLIN))
			return;

		// Accept request
		int acceptfd = _sock.Accept();
		if (acceptfd < 0) {
			std::cerr << "[ERR] Socket acceptation failed on port " << _sock.GetPort() << ": "
			          << errno << ' ' << std::strerror(errno) << std::endl;
			return;
		}

		// Receive request
		std::stringstream	input;
		char buffer[1025];
		ssize_t bufflen;
		bufflen = read(acceptfd, buffer, 1024);
		buffer[bufflen] = '\0';
		input << buffer;
		if (input.str().length() == 0) {
			std::cerr << "[WARN] Empty request on port " << _sock.GetPort() << std::endl;
			close(acceptfd);
			return;
		}
		std::cout << '\n' << input.str() << std::endl;

		/*
		**If polling accpetfd turns out to be required, 
		**req will need to outlive the scope of this function,
		** and thus allocated on the heap instead of the stack.
		*/
		// Parse request.
		HttpRequest req(input);
		bool	serverfound = false;
		if (!req.IsOk()) {
			std::cerr << "[WARN] Invalid request received on port " << _sock.GetPort() << std::endl;
			// Need to return an code 400 to the client here.
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
	}

}
