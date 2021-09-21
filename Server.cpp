/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 16:10:54 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include "HttpRequest.hpp"

#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

namespace ft 
{

	static const char	headermsg[] =
		"HTTP/1.1 200 OK\n"
		"Server: ft_webserv\n"
		"Accept-Ranges: bytes\n"
		"Vary: Accept-Encoding\n"
		"Content-Type: text/plain\n"
		"\n"
	;

	static const char	defaultresponse[] = 
		"HTTP/1.1 200 OK\n"
		"Server: ft_webserv\n"
		"Accept-Ranges: bytes\n"
		"Vary: Accept-Encoding\n"
		"Content-Type: text/plain\n"
		"\n"
		"Beep boop. I am a robot, and I make robot noises.\n"
	;

	static const char	notFoundResponse[] =
		"HTTP/1.1 404 Not Found\n"
		"Server: ft_webserv\n"
		"Content-Type: text/plain\n"
		"\n"
		"Error 404 (Not Found) !\n"
		"The requested URL was not found on this server\n"
	;

	static const char	malformedResponse[] =
		"HTTP/1.1 400 Bad Request\n"
		"Server: ft_webserv\n"
		"Content-Type: text/plain\n"
		"\n"
		"Error 400 Bad Request\n"
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
		else if (!MatchPath(req))
			send(acceptfd, notFoundResponse, std::strlen(notFoundResponse), 0);
		else if (req.GetRequestPath().size() > 1) {
			send(acceptfd, headermsg, std::strlen(headermsg), 0);
			send(acceptfd, GetFileData(req).c_str(), GetFileData(req).size(), 0);
		}
		else 
			send(acceptfd, defaultresponse, std::strlen(defaultresponse), 0);
		close(acceptfd);
	}

	bool	Server::MatchPath(const HttpRequest& req) const {
		std::string	path = std::getenv("PWD") + req.GetRequestPath();

		if (FILE *file = fopen(path.c_str(), "r")) {
			fclose(file);
			return true;
		}
		else if (req.GetRequestPath() == "/")
			return true;
		return false;
	}

	std::string	Server::GetFileData(const HttpRequest& req) const {
		std::string		path = std::getenv("PWD") + req.GetRequestPath();
		std::ifstream	file(path.c_str());
		std::string		ret;

		std::getline(file, ret);
		return (ret);
	}

}
