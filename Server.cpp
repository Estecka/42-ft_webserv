/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 11:25:26 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include "HttpRequest.hpp"

#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>

namespace ft 
{
	static const char	defaultresponse[] = 
		"HTTP/1.1 200 OK\n"
		"Server: ft_webserv\n"
		"Accept-Ranges: bytes\n"
	//	"Content-Length: 50\n"
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
		this->_port = conf.GetPort();
		this->_hostname = conf.GetName();
	}


/******************************************************************************/
/* # Member functions                                                         */
/******************************************************************************/

	bool	Server::MatchRequest(const HttpRequest& req) const {
		if (req.IsValidPath())
			return (true);
		return req.IsOk() 
		    && this->_port == req.GetHostPort()
		    && (this->_hostname.empty() || this->_hostname == req.GetHostname())
			;
	}

	void	Server::Accept(int acceptfd, const HttpRequest& req) {
		if (!req.IsOk() || !req.IsMatchPath())
			send(acceptfd, malformedResponse, std::strlen(malformedResponse), 0);
		else {
			send(acceptfd, defaultresponse, std::strlen(defaultresponse), 0);
			send(acceptfd, req.GetHostname().c_str(), std::strlen(req.GetHostname().c_str()), 0);
		}
		close(acceptfd);
	}
}
