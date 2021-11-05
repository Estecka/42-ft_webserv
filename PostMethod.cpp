/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/05 09:40:51 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"
#include "PollManager.hpp"
#include "ErrorPage.hpp"
#include "logutil/logutil.hpp"
#include "clibft/ErrnoException.hpp"

namespace ft {

	PostMethod::PostMethod(FILE* body, RequestHandler& parent, std::string upload_path, int acceptfd):
	_body(body),
	_parent(parent),
	_upload_path(upload_path),
	_acceptfd(acceptfd) {
		_firstLoop = true;
		_newFile = true;
		_reachedEoF = false;
		_endOfNewFile = false;
		ft::clog << log::info << "PostMethod created." << std::endl;
		_bodyfd = fileno(_body);
		this->PrepareToRead();
	}

	PostMethod::~PostMethod(void) {
		ft::clog << log::info << "PostMethod destroyed." << std::endl;
	}

	void	PostMethod::GetPollFd(pollfd& outfd) {
		outfd = this->_pollfd;
	}

	void	PostMethod::OnPollEvent(const pollfd&) {
		while (!(this->*_pollAction)())
			continue ;
		if (!_reachedEoF)
			return ;
		else
			return _parent.SetPollEvent(new ErrorPage(202, _acceptfd, _parent));
	}

	bool	PostMethod::PrepareToRead(void) {
		this->_pollfd.fd = _bodyfd;
		this->_pollfd.events = POLLIN;
		this->_pollAction = &PostMethod::read;
		PollManager::SetDirty();
		return false;
	}

	bool	PostMethod::PrepareToWrite(void) {
		this->_pollfd.fd = _newFd;
		this->_pollfd.events = POLLOUT;
		this->_pollAction = &PostMethod::write;
		PollManager::SetDirty();
		return false;
	}

	bool	PostMethod::PrepareToQuit(void) {
		this->_pollAction = NULL;
		this->_body = NULL;
		return true;
	}

	bool	PostMethod::read(void) {
		std::string	line;

		while (true) {

			if (_strBuff.empty() && !_reachedEoF) {
				while (!std::feof(_body)) {
					_strBuff.clear();
					ssize_t	readlen = std::fread(_buffer, 1, 1024, _body);
					_strBuff = std::string(_buffer, readlen);
					if (std::ferror(_body) || readlen < 0)
						return true;
					if (_firstLoop)
						FirstParsing();
					break;
				}
			}
			else if (_reachedEoF)
				break;
			else
				return TreatBuffer();
		}
		return PrepareToQuit();
	}

	bool	PostMethod::write(void) {
		while (true) {
			size_t	len = ::write(_newFd, _content.c_str(), _content.size());
			
			if (len < 0)
				return true;
			else if (len < _content.size())
				_content = _content.substr(len);
			else
				break ;
		}
		if (_endOfNewFile)
			close(_newFd);
		return PrepareToRead();
	}

	void	PostMethod::FirstParsing(void) {
		_boundary = "\r\n" + _strBuff.substr(0, _strBuff.find("\r"));
		_eof = _boundary + "--";
		_strBuff = _strBuff.substr(_boundary.size() - 2);
		if (_strBuff.empty())
			_reachedEoF = true;
		_firstLoop = false;
	}
	
	void	PostMethod::ParseHeader(void) {
		std::string	header = _strBuff.substr(0, _strBuff.find("\r\n\r\n"));

		_fileName.clear();
		_name.clear();
		_fileName = FindWord(header, "filename=\"", '\"');
		_name = FindWord(header, "name=\"", '\"');
		if (_fileName == "") {
			if (_strBuff.find(_boundary) == _strBuff.find(_eof)) {
				_strBuff.clear();
				_reachedEoF = true;
			}
			else if (_strBuff.find(_boundary) != std::string::npos) {
				_strBuff =_strBuff.substr(_strBuff.find(_boundary) + _boundary.size());
			}
			else
				_strBuff.clear();
			return ;
		}
		_strBuff = _strBuff.substr(_strBuff.find("\r\n\r\n") + 4);
		_newFile = false;
		if (!IsDir(_upload_path, false))
			if (RecursiveMkdir(_upload_path, 2) == -1)
				throw ft::ErrnoException("Failed to create upload directory.");
		std::string	path = _upload_path + "/" + _fileName;
		if ((_newFd = open(path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
			throw ft::ErrnoException("Failed to create new file.");
	}

	bool	PostMethod::TreatBuffer(void) {
		while (!_strBuff.empty()) {
			if (_strBuff == _eof) {
				_reachedEoF = true;
				return false;
			}
			if (_newFile) {
				ParseHeader();
				if (_fileName == "")
					return false;
				continue ;
			}
			if (_strBuff.find(_boundary) != std::string::npos && _strBuff.find(_boundary) != _strBuff.find(_eof)) {
				_content = _strBuff.substr(0, _strBuff.find(_boundary));
				_endOfNewFile = true;
				_newFile = true;
				_strBuff = _strBuff.substr(_strBuff.find(_boundary) + _boundary.size());
			}
			else if (_strBuff.find(_eof) != std::string::npos) {	
				_content = _strBuff.substr(0, _strBuff.find(_boundary) - 2);
				_endOfNewFile = true;
				_strBuff.clear();
				_strBuff = _eof;
			}
			else {
				_endOfNewFile = false;
				_newFile = false;
				_content = _strBuff;
				_strBuff.clear();
			}
			return this->PrepareToWrite();
		}
		ft::clog << log::warning << "No buffer to treat." << std::endl;
		return false;
	}

	std::string	PostMethod::FindWord(std::string content, std::string toFind, char sep) {
		const char	*str = content.c_str();
		int			len = 0;
		size_t		toFind_len = toFind.size();
		size_t		pos = content.find(toFind) + toFind_len;
		
		if (content.find(toFind) == std::string::npos)
			return ("");
		for (size_t i = 0; i < pos; i++)
			str++;
		while (*str != sep) {
			len++;
			str++;
		}
		return content.substr(pos,len);
	}
}
