/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollListener.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:51:17 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 17:37:09 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollListener.hpp"
#include "PollManager.hpp"

#include "RequestReadPollListener.hpp"

#include <iostream>

#include <cerrno>
#include <cstring>
#include <unistd.h>

namespace ft
{
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

	void	SocketPollListener::OnPollEvent(const struct pollfd&) {

		int acceptfd = _sock.Accept();
		if (acceptfd < 0) {
			std::cerr << "[ERR] Socket acceptation failed on port " << _sock.GetPort() << ": "
			          << errno << ' ' << std::strerror(errno) << std::endl;
			return;
		}

		RequestReadPollListener& readListener = *new RequestReadPollListener(acceptfd, _sock.GetPort());
		PollManager::AddListener(readListener);
	}

}
