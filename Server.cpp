/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/29 17:35:38 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace ft 
{
	/*
	** These pre-made responses should be disused and replaced with the HttpHeader class.
	*/
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

	Server::Server(const ServerConfig& conf)
	: _config(conf), _ports(conf.ports), _hostname(conf.servername)
	{};

	Server::~Server() {};


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
		const UriConfig&	conf = _config.GetUriConfig(req.GetRequestPath());
		
		if (!req.IsOk())
			send(acceptfd, malformedResponse, std::strlen(malformedResponse), 0);
		else if (!MatchPath(req, conf))
			send(acceptfd, notFoundResponse, std::strlen(notFoundResponse), 0);
		else if ((IsDir(conf.root + req.GetRequestPath()) && req.GetRequestPath().size() > 1) || req.GetRequestPath().size() == 1)
			GetIndex(acceptfd, req, conf);
		else if (req.GetRequestPath().size() > 1) 
			GetFileData(acceptfd, req.GetRequestPath(), conf);
		close(acceptfd);
	}

	bool	Server::IsDir(const std::string	path) const {
		struct stat	statbuf;

		stat(path.c_str(), &statbuf);
		if (S_ISDIR(statbuf.st_mode))
			return true;
		return false;
	}

	bool	Server::MatchPath(const HttpRequest& req, const UriConfig& conf) const {
		std::string	path = conf.root + req.GetRequestPath();
		
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

	void	Server::GetFileData(int acceptfd, std::string reqPath, const UriConfig& conf) const {
		std::string		path = conf.root + reqPath;
		std::string		ret;
		HttpHeader		head(200, reqPath.substr(reqPath.find(".")));
		std::ifstream	file(path.c_str());
		
		send(acceptfd, head.ToString().c_str(), head.ToString().size(), 0);
		while (std::getline(file, ret)) {
			send(acceptfd, ret.c_str(), ret.size(), 0);
			send(acceptfd, "\n", 1, 0);
		}
	}

	void	Server::GetIndex(int acceptfd, const HttpRequest& req, const UriConfig& conf) const {
		DIR				*dir;
		struct dirent	*ent;
		std::string		path = conf.root + req.GetRequestPath();

		if ((dir = opendir(path.c_str())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				std::string	inDirFile = ent->d_name;
				if (inDirFile == "index.html") {
					closedir(dir);
					return (GetFileData(acceptfd, req.GetRequestPath() + "/index.html", conf));
				}
			}
		}
		AutoIndex	autoIndex(req, acceptfd, conf.root + req.GetRequestPath());
	}

}
