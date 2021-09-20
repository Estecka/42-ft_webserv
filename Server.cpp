/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/20 18:38:41 by abaur            ###   ########.fr       */
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

	void	Server::SetConfig(const ServerConfig& conf) {
		this->_config = &conf;
		this->_ports = conf.GetPorts();
		this->_hostname = conf.GetName();
	}


/******************************************************************************/
/* # Member functions                                                         */
/******************************************************************************/

	bool	Server::MatchRequest(const HttpRequest& req) const {
		if (!req.IsOk())
			return false;

		if (!this->_hostname.empty() && this->_hostname != req.GetHostname())
			return false;

		for (size_t i=0; i<this->_ports.size(); i++)
			if (_ports[i] == req.GetHostPort())
				return true;

		return false;
	}

	void	Server::Accept(int acceptfd, const HttpRequest& req) {
		if (!req.IsOk())
			send(acceptfd, malformedResponse, std::strlen(malformedResponse), 0);
		else
			send(acceptfd, defaultresponse, std::strlen(defaultresponse), 0);

		close(acceptfd);
	}
}
