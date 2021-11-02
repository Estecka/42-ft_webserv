/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:43:42 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/02 15:52:08 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Methods.hpp"
#include "ErrorPage.hpp"
#include "CGILauncher.hpp"
#include "Cgi2Http.hpp"
#include "PostMethod.hpp"
#include "GetFileData.hpp"
#include "AutoIndex.hpp"

namespace ft {

	IPollListener	Methods(const UriConfig& config, const RequestHeader& req, int fd, RequestHandler& parent, FILE* body):
		for (std::size_t i = 0; i < config.methods.size(); i++) {
			if (req.GetMethod() == config.methods[i]) {
				if (req.GetMethod() == "DELETE")
					return Delete(config, req.GetRequestPath(), fd, parent);
				else if (req.GetMethod() == "GET")
					return Get(config, req.GetRequestPath(), fd, parent);
				else if (req.GetMethod() == "POST")
					return Post(config, fd, parent, body);
			}
		}
		if (req.GetMethod() == "DELETE" || req.GetMethod() == "GET" || req.GetMethod() == "POST")
			throw 	HttpException(405)
		else
			throw	HttpException(501);
	}

	static IPollListener	Delete(const UriConfig& config, std::string reqPath, int fd, RequestHandler& parent) {
		std::string	path = config.root + reqPath;
		
		if (config.cgiPath != ""){
			pid_t	cgiPid;
			int  	cgiPipeout;
			LaunchCGI(parent, cgiPid, cgiPipeout);
			return new Cgi2Http(parent, cgiPid, cgiPipeout);
		}
		else if (MatchPath(config.root + reqPath) && !IsDir(path, true)) {
			if (!remove(path.c_str()))
			{
				std::clog << log::debug << "DELETE SUCCEED" << RESET << std::endl;
				throw	HttpException(202);
			}
			else
				throw	HttpException(403);
		}
		else
			throw	HttpException(404);
	}

	static IPollListener	Get(const UriConfig& config, std::string reqPath, int fd, RequestHandler& parent) {
		if (config.handle.path != "")
			reqPath = reqPath.substr(0, reqPath.rfind(config.handle.path)) + "/"
				+ reqPath.substr(reqPath.rfind(config.handle.path)
				+ config.handle.path.size());
		if (config.returnCode || config.returnPath != "") {
			if (config.returnPath != "")
				Redirection(config, fd);
			else
				throw	HttpException(config.returnCode);
		}
		else if (config.cgiPath != ""){
			pid_t	cgiPid;
			int  	cgiPipeout;
			LaunchCGI(parent, cgiPid, cgiPipeout);
			return new Cgi2Http(parent, cgiPid, cgiPipeout);
		}
		else if (!MatchPath(config.root + reqPath))
			throw	HttpException(404);
		else if ((IsDir(config.root + reqPath, true) && reqPath.size() >= 1))
			GetIndex(reqPath, config, fd, parent);
		else if (reqPath.size() > 1)
			return new GetFileData(config.root + reqPath, fd, parent);
	}

	static IPollListener	Post(const UriConfig& config, int fd, RequestHandler& parent, FILE* body) {
		if (config.cgiPath != "") {
			pid_t	cgiPid;
			int		cgiPipeout;
			LaunchCGI(parent, cgiPid, cgiPipeout);
			return new Cgi2Http(parent, cgiPid, cgiPipeout);
		}
		return new PostMethod(body, parent, config.upload_path, fd);
	}

	static IPollListener	Redirection(const UriConfig& config, int fd) {
		ResponseHeader		header(config.returnCode != 0 ? config.returnCode : 303, ".html");
		std::string			page;

		header.SetLocation(config.returnPath);

		page = header.ToString();
		while (true) {
			std::size_t	len = write(fd, page.c_str(), page.size());
			if (len < 0)
				return NULL;
			else if (len < page.size())
				page = page.substr(len);
			else
				break;
		}
		parent.Destroy();
		return NULL
	}

	static bool	MatchPath(std::string path) {
		std::string	path = _config.root + _reqPath;
	
		if (FILE *file = fopen(path.c_str(), "r+")) {
			fclose(file);
			return true;
		}
		else if (IsDir(path, true))
			return true;
		else if (_reqPath == "/")
			return true;
		return false;
	}

	static IPollListener	GetIndex(std::string reqPath, const UriConfig& config, int fd, RequestHandler& parent) {
		DIR				*dir;
		struct dirent	*ent;
		std::string		path = config.root + reqPath;

		if ((dir = opendir(path.c_str())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				std::string	inDirFile = ent->d_name;
				for (std::size_t i = 0; i < config.index.size(); i++) {
					if (inDirFile == config.index[i]) {
						closedir(dir);
						reqPath += "/index.html";
						return new GetFileData(config.root + reqPath, fd, parent);
					}
				}
			}
		}
		if (_config.autoindex)
			return new AutoIndex(fd, path, config.root, reqPath, parent);
		else
			throw	HttpException(403);
	}
}
