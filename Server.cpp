/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/30 14:02:27 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace ft 
{
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
			ErrorPage	error(400, acceptfd);
		else if (!MatchPath(req, conf))
			ErrorPage	error(404, acceptfd);
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
		HttpHeader		head(200);
		std::ifstream	file(path.c_str());
		
		if (reqPath.find(".") == std::string::npos)
			head.SetContentType("");
		else
			head.SetContentType(reqPath.substr(reqPath.find(".")));
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
		AutoIndex(req, acceptfd, conf.root + req.GetRequestPath());
	}

	void	Server::AutoIndex(const HttpRequest& req, int acceptfd, std::string path) const {
		DIR				*dir;
		std::string		dirName = req.GetRequestPath();
		struct dirent	*ent;
		std::string		href;
		ft::HttpHeader	header(200, ".html");
		std::string		index;
	
		dirName == "/" ? href = "" : href = dirName + "/";
		if ((dir = opendir(path.c_str())) != NULL) {
			index = \
			"<!DOCTYPE html>\n\
			<html>\n\
				<head>\n\
					<title>" + dirName + "</title>\n\
				</head>\n\
				<body>\n\
					<h1>Index</h1>\n\
					<p>\n";
			while ((ent = readdir(dir)) != NULL) {
				std::string	inDirFile = ent->d_name;
				index += \
					"<a href=\"" + href +  inDirFile + "\">" + inDirFile + "</a><br>\n";
			}
			index += \
					"<br><br></p>\n\
					<hr>\n\
					<p> abaur | WEBSERV | apitoise<br></p>\n\
				</body>\n\
			</html>\n\
			";
			closedir(dir);
			send(acceptfd, header.ToString().c_str(), header.ToString().size(), 0);
			send(acceptfd, index.c_str(), index.size(), 0);
		}
		else
			std::cerr << "[WARN] CANNOT OPEN THIS DIRECTORY" << std::endl;
	}
}
