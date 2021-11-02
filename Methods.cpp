/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:43:42 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/31 20:06:08 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Methods.hpp"

#include "AutoIndex.hpp"
#include "CGILauncher.hpp"
#include "Cgi2Http.hpp"
#include "ErrorPage.hpp"
#include "GetFileData.hpp"
#include "PostMethod.hpp"
#include "clibft/filesystem.hpp"
#include "logutil/logutil.hpp"

namespace ft {


	IPollListener	Methods(const UriConfig& config, const RequestHeader& req, int fd, RequestHandler& parent, FILE* body):
		for (std::size_t i = 0; i < config.methods.size(); i++) {
			if (req.GetMethod() == config.methods[i]) {
				if (req.GetMethod() == "DELETE")
					return Delete(config, req.GetRequestPath(), fd, parent);
				else if (req.GetMethod() == "GET")
					return Get(config, req.GetRequestPath(), fd, parent);
				else if (req.GetMethod() == "POST")
					return Post();
			}
		}
		if (req.GetMethod() == "DELETE" || req.GetMethod() == "GET" || req.GetMethod() == "POST")
			return new ErrorPage(405, fd, parent);
		else
			return new ErrorPage(501, fd, parent);
	}

	static IPollListener	Delete(const UriConfig& config, std::string reqPath, int fd, RequestHandler& parent) {
		std::string	path = config.root + reqPath;
		
		if (config.cgiPath != ""){
			pid_t	cgiPid;
			int  	cgiPipeout;
			LaunchCGI(parent, cgiPid, cgiPipeout);
			return new Cgi2Http(parent, cgiPid, cgiPipeout);
		}
		else if (MatchPath() && !ft::IsDir(path, true)) {
			if (!remove(path.c_str()))
			{
				ft::clog << log::debug << "DELETE SUCCEED" << std::endl;
				return new ErrorPage(202, fd, parent));
			}
			else
				return new ErrorPage(403, fd, parent);
		}
		else
			return new ErrorPage(404, fd, parent);
	}

	static IPollListener	Get(const UriConfig& config, std::string reqPath, int fd, RequestHandler& parent) {
		if (config.handle.path != "")
			reqPath = reqPath.substr(0, reqPath.rfind(config.handle.path)) + "/"
				+ reqPath.substr(reqPath.rfind(config.handle.path)
				+ config.handle.path.size());
		if (config.returnCode || config.returnPath != "") {
			if (config.returnPath != "")
				Redirection();
			else
				return new ErrorPage(config.returnCode, fd, parent);
		}
		else if (config.cgiPath != ""){
			pid_t	cgiPid;
			int  	cgiPipeout;
			LaunchCGI(parent, cgiPid, cgiPipeout);
			return new Cgi2Http(parent, cgiPid, cgiPipeout);
		}
		else if (!MatchPath())
			return new ErrorPage(404, acceptfd, parent);
		else if ((IsDir(config.root + reqPath, true) && reqPath.size() >= 1))
			GetIndex();
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

		header.SetLocation(_config.returnPath);

		page = header.ToString();
		while (true) {
			std::size_t	len = write(_acceptfd, page.c_str(), page.size());
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

	bool	Methods::MatchPath() {
		std::string	path = _config.root + _reqPath;
	
		if (FILE *file = fopen(path.c_str(), "r+")) {
			fclose(file);
			return true;
		}
		else if (ft::IsDir(path, true))
			return true;
		else if (_reqPath == "/")
			return true;
		return false;
	}

	void	Methods::GetIndex(void) {
		DIR				*dir;
		struct dirent	*ent;
		std::string		path = _config.root + _reqPath;

		if ((dir = opendir(path.c_str())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				std::string	inDirFile = ent->d_name;
				for (std::size_t i = 0; i < _config.index.size(); i++) {
					if (inDirFile == _config.index[i]) {
						closedir(dir);
						_reqPath += "/index.html";
						return _parent.SetPollEvent(new GetFileData(_config.root + _reqPath, _acceptfd, _parent));
					}
				}
			}
		}
		if (_config.autoindex)
			return _parent.SetPollEvent(new AutoIndex(_acceptfd, path, _config.root, _reqPath, _parent));
		else
			return _parent.SetPollEvent(new ErrorPage(403, _acceptfd, _parent));
	}
}
