/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/28 12:00:09 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"
#include "PollManager.hpp"
#include "ErrorPage.hpp"

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
		std::cerr << "[DEBUG] PostMethod created." << std::endl;
		_bodyfd = fileno(_body);
		this->PrepareToRead();
	}

	PostMethod::~PostMethod(void) {
		std::cerr << "[DEBUG] PostMethod destroyed." << std::endl;
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
		//_parent.Destroy();
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
		close(_newFd);
		this->_pollAction = NULL;
		this->_body = NULL;
		return true;
	}

	bool	PostMethod::read(void) {
		std::string	line;

		while (true) {
			if (_strBuff.empty()) {
				while (!std::feof(_body)) {
					_strBuff.clear();
					ssize_t	readlen = std::fread(_buffer, 1, 1024, _body);
					if (std::ferror(_body) || readlen < 0)
						return true;
					_strBuff = std::string(_buffer, readlen);
					if (_firstLoop)
						FirstParsing();
					break;
				}
			}
			else if (_reachedEoF)
				break;
			else {
				TreatBuffer();
				return false;
			}
		}
		this->_pollAction = &PostMethod::PrepareToQuit;
		return false;
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
		this->_pollAction = &PostMethod::read;
		return false;
	}

	void	PostMethod::FirstParsing(void) {
		_boundary = _strBuff.substr(0, _strBuff.find("\r"));
		_eof = _boundary + "--";
		_strBuff = _strBuff.substr(_boundary.size());
		_firstLoop = false;
	}
	
	void	PostMethod::ParseHeader(void) {
		std::string	header = _strBuff.substr(0, _strBuff.find("\r\n\r\n"));

		_fileName.clear();
		_name.clear();
		_fileName = FindWord(header, "filename=\"", '\"');
		_name = FindWord(header, "name=\"", '\"');
		if (_fileName == "")
			_fileName = _name;
		_strBuff = _strBuff.substr(_strBuff.find("\r\n\r\n") + 4);
		_newFile = false;
		mkdir(_upload_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		chdir(_upload_path.c_str());
		_newFd = open(_fileName.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666);
	}

	void	PostMethod::TreatBuffer(void) {
		while (!_strBuff.empty()) {
			if (_strBuff == _eof) {
				_reachedEoF = true;
				return ;
			}
			if (_newFile) {
				ParseHeader();
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
			this->_pollAction = &PostMethod::PrepareToWrite;
			return ;
		}
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
