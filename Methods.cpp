/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:43:42 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/02 14:30:49 by apitoise         ###   ########.fr       */
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

	Methods::Methods(const UriConfig& conf, const RequestHeader& req, int fd, RequestHandler& parent, FILE* body):
	_acceptfd(fd),
	_method(req.GetMethod()),
	_reqPath(req.GetRequestPath()),
	_config(conf), _parent(parent),
	_body(body) {
		std::cerr << "[DEBUG] Methods created." << std::endl;
		Parse();
		delete this;
	}

	Methods::~Methods(void) {
		std::cerr << "[DEBUG] Methods destroyed." << std::endl;
	}

	void	Methods::Parse(void) {
		for (std::size_t i = 0; i < _config.methods.size(); i++) {
			if (_method == _config.methods[i]) {
				if (_method == "DELETE")
					return Delete();
				else if (_method == "GET")
					return Get();
				else if (_method == "POST")
					return Post();
			}
		}
		if (_method == "DELETE" || _method == "GET" || _method == "POST")
			return _parent.SetPollEvent(new ErrorPage(405, _acceptfd, _parent));
		else
			return _parent.SetPollEvent(new ErrorPage(501, _acceptfd, _parent));
	}

	void	Methods::Delete(void) {
		std::string	path = _config.root + _reqPath;
		
		if (_config.cgiPath != ""){
			pid_t	cgiPid;
			int  	cgiPipeout;
			LaunchCGI(_parent, cgiPid, cgiPipeout);
			return _parent.SetPollEvent(new Cgi2Http(_parent, cgiPid, cgiPipeout));
		}
		else if (MatchPath() && !IsDir(path, true)) {
			if (!remove(path.c_str()))
			{
				std::cout << GREEN << "DELETE SUCCEED" << RESET << std::endl;
				return _parent.SetPollEvent(new ErrorPage(202, _acceptfd, _parent));
			}
			else
				return _parent.SetPollEvent(new ErrorPage(403, _acceptfd, _parent));
		}
		else
			return _parent.SetPollEvent(new ErrorPage(404, _acceptfd, _parent));
	}

	void	Methods::Get(void) {
		if (_config.handle.path != "")
			_reqPath = _reqPath.substr(0, _reqPath.rfind(_config.handle.path)) + "/"
				+ _reqPath.substr(_reqPath.rfind(_config.handle.path)
				+ _config.handle.path.size());
		if (_config.returnCode || _config.returnPath != "") {
			if (_config.returnPath != "")
				Redirection();
			else
				return _parent.SetPollEvent(new ErrorPage(_config.returnCode, _acceptfd, _parent));
		}
		else if (_config.cgiPath != ""){
			pid_t	cgiPid;
			int  	cgiPipeout;
			LaunchCGI(_parent, cgiPid, cgiPipeout);
			return _parent.SetPollEvent(new Cgi2Http(_parent, cgiPid, cgiPipeout));
		}
		else if (!MatchPath())
			return _parent.SetPollEvent(new ErrorPage(404, _acceptfd, _parent));
		else if ((IsDir(_config.root + _reqPath, true) && _reqPath.size() >= 1))
			GetIndex();
		else if (_reqPath.size() > 1)
			return _parent.SetPollEvent(new GetFileData(_config.root + _reqPath, _acceptfd, _parent));
	}

	void	Methods::Post(void) {
		if (_config.cgiPath != "") {
			pid_t	cgiPid;
			int		cgiPipeout;
			LaunchCGI(_parent, cgiPid, cgiPipeout);
			return _parent.SetPollEvent(new Cgi2Http(_parent, cgiPid, cgiPipeout));	
		}
		return _parent.SetPollEvent(new PostMethod(_body, _parent, _config.upload_path, _acceptfd));
	}

	void	Methods::Redirection() {
		ResponseHeader			header(_config.returnCode != 0 ? _config.returnCode : 303, ".html");
		std::string			page;

		header.SetLocation(_config.returnPath);

		page = header.ToString();
		while (true) {
			std::size_t	len = write(_acceptfd, page.c_str(), page.size());
			if (len < 0)
				return ;
			else if (len < page.size())
				page = page.substr(len);
			else
				break;
		}
		_parent.Destroy();
	}

	bool	Methods::MatchPath() {
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
