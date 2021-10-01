/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/10/01 14:38:13 by apitoise         ###   ########.fr       */
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
		std::string			reqPath = req.GetRequestPath();
		const UriConfig&	conf = _config.GetUriConfig(reqPath);

		reqPath = reqPath.substr(0, reqPath.rfind(conf.handle.path)) + "/" + reqPath.substr(reqPath.rfind(conf.handle.path) + conf.handle.path.size());
		if (conf.returnCode || conf.returnPath != "") {
			if (conf.returnPath != "")
				Redirection(acceptfd, conf);
			else
				ErrorPage	error(conf.returnCode, acceptfd);
		}
		else if (!req.IsOk())
			ErrorPage	error(400, acceptfd);
		else if (!MatchPath(reqPath, conf))
			ErrorPage	error(404, acceptfd);
		else if ((IsDir(conf.root + reqPath) && reqPath.size() >= 1)) {
			GetIndex(acceptfd, req, conf, reqPath);
		}
		else if (reqPath.size() > 1) 
			GetFileData(acceptfd, reqPath, conf);
		close(acceptfd);
	}

	void	Server::Redirection(int acceptfd, const UriConfig& conf) const {
		HttpHeader	header(303, ".html", conf.returnPath);

		send(acceptfd, header.ToString().c_str(), header.ToString().size(), 0);
	}

	bool	Server::IsDir(const std::string	path) const {
		struct stat	statbuf;

		stat(path.c_str(), &statbuf);
		if (S_ISDIR(statbuf.st_mode))
			return true;
		return false;
	}

	bool	Server::MatchPath(const std::string reqPath, const UriConfig& conf) const {
		std::string	path = conf.root + reqPath;
	
		if (FILE *file = fopen(path.c_str(), "r+")) {
			fclose(file);
			return true;
		}
		else if (IsDir(path))
			return true;
		else if (reqPath == "/")
			return true;
		return false;
	}

	void	Server::GetFileData(int acceptfd, std::string reqPath, const UriConfig& conf) const {
		std::string		path = conf.root + reqPath;
		std::string		ret;
		HttpHeader		head(200);
		std::ifstream	file(path.c_str());
		
		if (reqPath.rfind(".") == std::string::npos)
			head.SetContentType("");
		else
			head.SetContentType(reqPath.substr(reqPath.rfind(".")));
		send(acceptfd, head.ToString().c_str(), head.ToString().size(), 0);
		while (std::getline(file, ret)) {
			send(acceptfd, ret.c_str(), ret.size(), 0);
			send(acceptfd, "\n", 1, 0);
		}
	}

	void	Server::GetIndex(int acceptfd, const HttpRequest& req, const UriConfig& conf, std::string reqPath) const {
		DIR				*dir;
		struct dirent	*ent;
		std::string		path = conf.root + reqPath;

		if ((dir = opendir(path.c_str())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				std::string	inDirFile = ent->d_name;
				for (std::size_t i = 0; i < conf.index.size(); i++) {
					if (inDirFile == conf.index[i]) {
						closedir(dir);
						return (GetFileData(acceptfd, reqPath + "/index.html", conf));
					}
				}
			}
		}
		if (conf.autoindex)
			AutoIndex(req, acceptfd, path);
		else
			ErrorPage	error(403, acceptfd);
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
		else {
			std::cerr << "[WARN] CANNOT OPEN THIS DIRECTORY" << std::endl;
			ErrorPage	error(400, acceptfd);
		}
	}
}
