/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:43:42 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/25 15:34:05 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Methods.hpp"
#include "ErrorPage.hpp"
#include "CGILauncher.hpp"
#include "Cgi2Http.hpp"
#include "PostMethod.hpp"

namespace ft {

	Methods::Methods(const UriConfig& conf, const RequestHeader& req, int fd, RequestHandler& parent, FILE* body): _acceptfd(fd), _method(req.GetMethod()), _reqPath(req.GetRequestPath()), _config(conf), _parent(parent), _body(body) {
		std::cerr << "[DEBUG] Methods created." << std::endl;
	}


	Methods::~Methods(void) {
		std::cerr << "[DEBUG] Methods destroyed." << std::endl;
	}

	void	Methods::GetPollFd(pollfd& poll_fd) {
		poll_fd.fd = _acceptfd;
		poll_fd.events = POLLOUT;
	}

	void	Methods::OnPollEvent(const pollfd&) {
		for (std::size_t i = 0; i < _config.methods.size(); i++) {
			if (_method == _config.methods[i]) {
				if (_method == "DELETE")
					return Delete();
				else if (_method == "GET")
					return Get_Post();
				else if (_method == "POST") {
					_parent.SetPollEvent(new PostMethod(_body, _parent));
					close(_acceptfd);
					return _parent.Destroy();
				}
			}
		}
		if (_method == "DELETE" || _method == "GET" || _method == "POST")
			return _parent.SetPollEvent(new ErrorPage(405, _acceptfd, _parent));
		else
			return _parent.SetPollEvent(new ErrorPage(501, _acceptfd, _parent));
	}

	void	Methods::Delete(void) {
		std::string	path = _config.root + _reqPath;

		if (MatchPath() && !IsDir(path, true)) {
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

	void	Methods::Get_Post(void) {
		if (_config.handle.path != "")
			_reqPath = _reqPath.substr(0, _reqPath.rfind(_config.handle.path)) + "/" + _reqPath.substr(_reqPath.rfind(_config.handle.path) + _config.handle.path.size());
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
			GetFileData();
		close(_acceptfd);
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

	bool	Methods::IsDir(const std::string path, bool slash) const {
		struct stat	statbuf;

		stat(path.c_str(), &statbuf);
		if (S_ISDIR(statbuf.st_mode) && !slash)
			return true;
		if (S_ISDIR(statbuf.st_mode) && slash && path[path.size() - 1] == '/')
			return true;
		return false;
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

	void	Methods::GetFileData() {
		std::string				path = _config.root + _reqPath;
		std::string				ret;
		ResponseHeader				head(200);
		std::ifstream			file(path.c_str());
		std::stringstream		page;
		char					buff[1024];
		std::size_t				bufflen;

		if (_reqPath.rfind(".") == std::string::npos)
			head.SetContentType("");
		else
			head.SetContentType(_reqPath.substr(_reqPath.rfind(".")));
		page << head.ToString();
		while (!file.eof()) {
			file.read(buff, 1024);
			bufflen = file.gcount();
			page.write(buff, bufflen);
		}
		std::string	strPage = page.str();
		while (true) {
			size_t	len = write(_acceptfd, strPage.c_str(), strPage.size());
			if (len < 0)
				return;
			else if (len < strPage.size())
				strPage = strPage.substr(len);
			else
				break ;
		}
		_parent.Destroy();
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
						return (GetFileData());
					}
				}
			}
		}
		if (_config.autoindex)
			AutoIndex(path);
		else
			return _parent.SetPollEvent(new ErrorPage(403, _acceptfd, _parent));
	}

	void	Methods::AutoIndex(std::string path) {
		DIR									*dir;
		std::string							dirName = _reqPath;
		struct dirent						*ent;
		std::string							href;
		ft::ResponseHeader					header(200, ".html");
		std::string							index;
		std::list<std::string>				inDirFile;
		std::list<std::string>::iterator	it;

		index = header.ToString();
		dirName == "/" ? href = "" : href = dirName;
		if ((dir = opendir(path.c_str())) != NULL) {
			index += \
			"<!DOCTYPE html>\n\
			<html>\n\
				<head>\n\
					<title>" + dirName + "</title>\n\
				</head>\n\
				<body>\n\
					<h1>Index</h1>\n\
					<p>\n";
			while ((ent = readdir(dir)) != NULL) {
				if (strcmp(ent->d_name, "."))
					inDirFile.push_back(ent->d_name);
			}
			inDirFile.sort();
			for (it = inDirFile.begin(); it != inDirFile.end(); it++) {
				bool	dir = IsDir(_config.root + *it, false);
				index += \
					"<a href=\"" + href + *it + (dir ? "/" : "") + "\">" + *it + (dir ? "/" : "") + "</a><br>\n";
			}
			index += \
					"<br><br></p>\n\
					<hr>\n\
					<p> abaur | WEBSERV | apitoise<br></p>\n\
				</body>\n\
			</html>\n\
			";
			closedir(dir);
			while (true) {
				std::size_t	len = write(_acceptfd, index.c_str(), index.size());
				
				if (len < 0)
					return ;
				else if (len < index.size())
					index = index.substr(len);
				else
					break;
			}
			_parent.Destroy();
		}
		else {
			std::cerr << "[WARN] CANNOT OPEN THIS DIRECTORY" << std::endl;
			return _parent.SetPollEvent(new ErrorPage(404, _acceptfd, _parent));
		}
	}


}
