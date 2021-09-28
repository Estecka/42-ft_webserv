/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/28 14:38:55 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace ft 
{

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
		this->_root = conf.GetRoot();
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
		else if ((IsDir(_root + req.GetRequestPath()) && req.GetRequestPath().size() > 1) || req.GetRequestPath().size() == 1)
			GetIndex(acceptfd, req);
		else if (req.GetRequestPath().size() > 1) 
			GetFileData(acceptfd, req.GetRequestPath());
		else 
			send(acceptfd, defaultresponse, std::strlen(defaultresponse), 0);
		close(acceptfd);
	}

	bool	Server::IsDir(const std::string	path) const {
		struct stat	statbuf;

		stat(path.c_str(), &statbuf);
		if (S_ISDIR(statbuf.st_mode))
			return true;
		return false;
	}

	bool	Server::MatchPath(const HttpRequest& req) const {
		std::string	path = _root + req.GetRequestPath();
		
		if (FILE *file = fopen(path.c_str(), "r+")) {
			fclose(file);
			return true;
		}
		else if (IsDir(path))
			return true;
		else if (req.GetRequestPath() == "/")
			return true;
		return false;
	}

	void	Server::GetFileData(int acceptfd, std::string reqPath) const {
		std::string		path = _root + reqPath;
		std::string		ret;
		HttpHeader		head(200, reqPath.substr(reqPath.find(".")));
		std::ifstream	file(path.c_str());
		
		send(acceptfd, head.ToString().c_str(), head.ToString().size(), 0);
		while (std::getline(file, ret)) {
			send(acceptfd, ret.c_str(), ret.size(), 0);
			send(acceptfd, "\n", 1, 0);
		}
	}

	void	Server::GetIndex(int acceptfd, const HttpRequest& req) const {
		DIR				*dir;
		struct dirent	*ent;
		std::string		path = _root + req.GetRequestPath();

		if ((dir = opendir(path.c_str())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				std::string	inDirFile = ent->d_name;
				if (inDirFile == "index.html") {
					closedir(dir);
					return (GetFileData(acceptfd, req.GetRequestPath() + "/index.html"));
				}
			}
		}
		AutoIndex	autoIndex(req, acceptfd, _root + req.GetRequestPath());
	}

}
