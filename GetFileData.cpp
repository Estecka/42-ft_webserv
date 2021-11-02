/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetFileData.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:44:34 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/02 17:53:46 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetFileData.hpp"
#include "logutil/logutil.hpp"

namespace	ft {

	GetFileData::GetFileData(std::string path, int acceptfd, RequestHandler& parent):
	_path(path),
	_acceptfd(acceptfd),
	_parent(parent) {
		this->ReadFile();
		ft::clog << log::info << "GetFileData created." << std::endl;
		ft::clog << log::info << path << std::endl;
	}

	GetFileData::~GetFileData(void) {
		ft::clog << log::info << "GetFileData destroyed." << std::endl;
	}

	void	GetFileData::GetPollFd(pollfd& outfd) {
		outfd.fd = _acceptfd;
		outfd.events = POLLOUT;
	}

	void	GetFileData::OnPollEvent(const pollfd&) {
		while (true) {
			size_t	len = write(_acceptfd, _toWriteData.c_str(), _toWriteData.size());
			if (len < 0)
				return ;
			else if (len < _toWriteData.size())
				_toWriteData = _toWriteData.substr(len);
			else
				break ;
		}
		_parent.Destroy();
	}

	void	GetFileData::ReadFile(void) {
		std::ifstream	file(_path.c_str());

		_head.Setcode(200);
		if (_path.rfind(".") == std::string::npos)
			_head.SetContentType("");
		else
			_head.SetContentType(_path.substr(_path.rfind(".")));
		_data << _head.ToString();
		while (!file.eof()) {
			file.read(_buffer, 1024);
			_bufflen = file.gcount();
			_data.write(_buffer, _bufflen);
		}
		_toWriteData = _data.str();
	}
}
