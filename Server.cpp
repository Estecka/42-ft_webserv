/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/18 17:41:21 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include "HttpRequest.hpp"

#include <cstring>
#include <unistd.h>

namespace ft 
{
	static const char	defaultresponse[] = 
		"HTTP/1.1 200 OK\n"
		"Server: ft_webserv\n"
		"Accept-Ranges: bytes\n"
		"Content-Length: 50\n"
		"Vary: Accept-Encoding\n"
		"Content-Type: text/plain\n"
		"\n"
		"Beep boop. I am a robot, and I make robot noises.\n"
	;

	static const char	malformedResponse[] =
		"HTTP/1.1 400 Bad Request\n"
		"Server: ft_webserv\n"
		"Content-Length: 16\n"
		"Content-Type: text/plain\n"
		"\n"
		"400 Bad Request\n"
	;

	Server::Server() {};
	Server::~Server() {};


/******************************************************************************/
/* # Accessors                                                                */
/******************************************************************************/

	void	Server::SetSocket(ft::Socket& sock) {
		this->_socket = &sock;
	}

	void	Server::SetConfig(const ServerConfig& conf) {
		this->_config = &conf;
		this->_port = conf.GetPort();
		this->_hostname = conf.GetName();
	}

	int	Server::GetSockFd() const {
		return this->_socket->GetSocketFD();
	}


/******************************************************************************/
/* # Member functions                                                         */
/******************************************************************************/

	bool	Server::MatchRequest(const HttpRequest& req) const {
		return req.IsOk() 
		    && this->_hostname == req.GetHostname() 
		    && this->_port     == req.GetPort()
			;
	}

	void	Server::Accept() const
	{
		int acceptfd = this->_socket->Accept();

		char	inbuffer[1025] = {0};
		ssize_t inlen = read(acceptfd, inbuffer, 1024);
		if (inlen <= 0)
			std::cout << "Nothing received ("<<inlen<<")\n" << std::endl;
		else {
			std::cout << "REQUEST:\n" << inbuffer;
			std::cout.flush();
			ft::HttpRequest	req(inbuffer);
			if (!req.IsOk())
				send(acceptfd, malformedResponse, std::strlen(malformedResponse), 0);
			else
				send(acceptfd, defaultresponse, std::strlen(defaultresponse), 0);
		}

		close(acceptfd);
	}
}
