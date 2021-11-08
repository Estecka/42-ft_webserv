/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 12:31:54 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/08 15:51:46 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"

#include "ErrorPage.hpp"
#include "clibft/filesystem.hpp"
#include "logutil/logutil.hpp"
#include "clibft/ErrnoException.hpp"

namespace	ft {

	AutoIndex::AutoIndex(int acceptfd, std::string path, std::string root, std::string dirName, RequestHandler& parent):
	_acceptfd(acceptfd),
	_path(path),
	_root(root),
	_dirName(dirName),
	_parent(parent) {
		ft::clog << log::info << &_parent << "AutoIndex created." << std::endl;
		this->SetIndex();
	}

	AutoIndex::~AutoIndex(void) {
		ft::clog << log::info << &_parent << "AutoIndex destroyed." << std::endl;
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
				bool	isDir = ft::IsDir(_root + _dirName + *_it, false);
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
			if (closedir(_dir) == -1)
				throw ft::ErrnoException("Can not close directory.");
		}
		else {
			ft::clog << log::error << "Cannot open directory: " << _dirName << std::endl;
			if (IsDir(_path, false))
				throw ft::ErrnoException("Failed to open current directory");
			else
				return _parent.SetPollEvent(new ErrorPage(404, _acceptfd, _parent));
		}
	}
}
