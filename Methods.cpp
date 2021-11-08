/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 15:56:52 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/08 11:45:34 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Methods.hpp"
#include "ErrorPage.hpp"
#include "CGILauncher.hpp"
#include "Cgi2Http.hpp"
#include "PostMethod.hpp"
#include "GetFileData.hpp"
#include "AutoIndex.hpp"
#include "Redirection.hpp"
#include "clibft/filesystem.hpp"
#include "logutil/logutil.hpp"
#include "HttpException.hpp"

namespace ft {

	static bool	MatchPath(std::string path, std::string reqPath) {
		if (FILE *file = fopen(path.c_str(), "r+")) {
			fclose(file);
			return true;
		}
		else if (IsDir(path, true))
			return true;
		else if (reqPath == "/")
			return true;
		return false;
	}

	static std::string	CheckCgi(const UriConfig& config, std::string extension) {
		for (std::map<std::string, std::string>::const_iterator it = config.cgis.begin(); it != config.cgis.end(); it++) {
			if (it->first == extension)
				return it->second;
		}
	return "";
	}

	static IPollListener*	Launch_Cgi(std::string cgiPath, RequestHandler& parent, std::string reqPath) {
		pid_t	cgiPid;
		int		cgiPipeout;
		
		LaunchCGI(parent, cgiPid, cgiPipeout, cgiPath, reqPath);
		return new Cgi2Http(parent, cgiPid, cgiPipeout);
	}

	static IPollListener*	Delete(const UriConfig& config, std::string reqPath) {
		std::string	path = config.root + reqPath;
		
		if (MatchPath(config.root + reqPath, reqPath) && !IsDir(path, true)) {
			if (!remove(path.c_str()))
			{
				ft::clog << log::debug << "DELETE SUCCEED" << RESET << std::endl;
				throw	HttpException(202);
			}
			else
				throw	HttpException(403);
		}
		else
			throw	HttpException(404);
	}

	static IPollListener*	GetIndex(std::string reqPath, const UriConfig& config, int fd, RequestHandler& parent) {
		DIR				*dir;
		struct dirent	*ent;
		std::string		path = config.root + reqPath;

		if ((dir = opendir(path.c_str())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				std::string	inDirFile = ent->d_name;
				for (std::size_t i = 0; i < config.index.size(); i++) {
					if (inDirFile == config.index[i]) {
						closedir(dir);
						reqPath += inDirFile;
						if (CheckCgi(config, inDirFile.substr(inDirFile.rfind("."))) != "")
							return Launch_Cgi(CheckCgi(config, inDirFile.substr(inDirFile.rfind("."))), parent, config.root + reqPath);
						else
							return new GetFileData(config.root + reqPath, fd, parent);
					}
				}
			}
			closedir(dir);
		}
		if (config.autoindex)
			return new AutoIndex(fd, path, config.root, reqPath, parent);
		else
			throw	HttpException(403);
	}

	static IPollListener*	Get(const UriConfig& config, std::string reqPath, int fd, RequestHandler& parent) {
		if (config.rootedUri != "")
			reqPath = reqPath.substr(0, reqPath.rfind(config.rootedUri)) + "/"
				+ reqPath.substr(reqPath.rfind(config.rootedUri)
				+ config.rootedUri.size());
		if (config.returnCode || config.returnPath != "") {
			if (config.returnPath != "")
				return new Redirection(config, fd, parent);
			else
				throw	HttpException(config.returnCode);
		}
		else if (!MatchPath(config.root + reqPath, reqPath))
			throw	HttpException(404);
		else if ((IsDir(config.root + reqPath, true) && reqPath.size() >= 1))
			return GetIndex(reqPath, config, fd, parent);
		else
			return new GetFileData(config.root + reqPath, fd, parent);
	}

	IPollListener*	Methods(const UriConfig& config, const RequestHeader& req, int fd, RequestHandler& parent, FILE* body) {
		std::string	extension = "";
		
		if (req.GetRequestPath().rfind(".") != std::string::npos)
			extension = req.GetRequestPath().substr(req.GetRequestPath().rfind("."));
		
		if (CheckCgi(config, extension) != ""
			&& IsFile(config.root + req.GetRequestPath()))
			return Launch_Cgi(CheckCgi(config, extension), parent, config.root + req.GetRequestPath());
		
		for (std::size_t i = 0; i < config.methods.size(); i++) {
			if (req.GetMethod() == config.methods[i]) {
				if (req.GetMethod() == "DELETE")
					return Delete(config, req.GetRequestPath());
				else if (req.GetMethod() == "GET")
					return Get(config, req.GetRequestPath(), fd, parent);
				else if (req.GetMethod() == "POST")
					return new PostMethod(body, parent, config.upload_path, fd);
			}
		}
		if (req.GetMethod() == "DELETE" || req.GetMethod() == "GET" || req.GetMethod() == "POST")
			throw 	HttpException(405);
		else
			throw	HttpException(501);
	}
}
