/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollListener.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:51:17 by abaur             #+#    #+#             */
/*   Updated: 2021/10/31 19:57:48 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollListener.hpp"

#include "RequestHandler.hpp"
#include "logutil/logutil.hpp"

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
		Socket::fd_ip	ip_fd;

		ip_fd = _sock.Accept();
		if (ip_fd.acceptfd < 0) {
			ft::clog << log::error 
			         << "Socket acceptation failed on port " << _sock.GetPort() 
			         << ": " << errno << ' ' << std::strerror(errno)
			         << std::endl;
			return;
		}

		new RequestHandler(ip_fd.acceptfd, ip_fd.ip, _sock.GetPort());
	}

}
