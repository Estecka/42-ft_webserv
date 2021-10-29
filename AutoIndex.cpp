/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 12:31:54 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/29 17:02:24 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"
#include "ErrorPage.hpp"

namespace	ft {

	AutoIndex::AutoIndex(int acceptfd, std::string path, std::string root, std::string dirName, RequestHandler& parent):
	_acceptfd(acceptfd),
	_path(path),
	_root(root),
	_dirName(dirName),
	_parent(parent) {
		std::cerr << "[DEBUG] AutoIndex created." << std::endl;
		this->SetIndex();
	}

	AutoIndex::~AutoIndex(void) {
		std::cerr << "[DEBUG] AutoIndex destroyed." << std::endl;
	}

	void	AutoIndex::GetPollFd(pollfd& outfd) {
		outfd.fd = _acceptfd;
		outfd.events = POLLOUT;
	}

	void	AutoIndex::OnPollEvent(const pollfd&) {
		while (true) {
			std::size_t	len = write(_acceptfd, _index.c_str(), _index.size());

			if (len < 0)
				return ;
			else if (len < _index.size())
				_index = _index.substr(len);
			else
				break;
		}
		_parent.Destroy();
		close(_acceptfd);
	}

	void	AutoIndex::SetIndex(void) {
		ResponseHeader	header(200, ".html");
		_index = header.ToString();
		_dirName == "/" ? _href = "" : _href = _dirName;
		if ((_dir = opendir(_path.c_str()))) {
			_index += \
			"<!DOCTYPE html>\n\
			<html>\n\
				<head>\n\
					<title>" + _dirName + "</title>\n\
				</head>\n\
				<body>\n\
					<h1>Index</h1>\n\
					<p>\n";
			while ((_ent = readdir(_dir))) {
				if (strcmp(_ent->d_name, "."))
					_inDirFile.push_back(_ent->d_name);
			}
			_inDirFile.sort();
			for (_it = _inDirFile.begin(); _it != _inDirFile.end(); _it++) {
				bool	isDir = IsDir(_root + *_it, false);
				_index += \
						"<a href=\"" + _href + *_it + (isDir ? "/" : "") + "\">" + *_it + (isDir ? "/" : "") + "</a><br>\n";
			}
			_index += \
						"<br><br></p>\n\
						<hr>\n\
						<p> abaur | WEBSERV | apitoise<br></p>\n\
				</body>\n\
			</html>\n\
			";
			closedir(_dir);
		}
		else {
			std::cerr << "[WARNING] CANNOT OPEN THIS DIRECTORY" << std::endl;
			return _parent.SetPollEvent(new ErrorPage(404, _acceptfd, _parent));
		}
	}
}
