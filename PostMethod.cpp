/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/26 17:00:08 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"
#include "PollManager.hpp"

namespace ft {

	PostMethod::PostMethod(FILE* body, RequestHandler& parent): _body(body), _parent(parent), _firstLoop(true), _newFile(true), _reachedEoF(false), _endOfNewFile(false) {
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
		_parent.Destroy();
	}

	bool	PostMethod::PrepareToRead(void) {
		this->_pollfd.fd = _bodyfd;
		this->_pollfd.events = POLLIN;
		this->_pollAction = &PostMethod::read;
		PollManager::SetDirty();
		return false;
	}

	bool	PostMethod::PrepareToWrite(void) {
		_newFd = open(_fileName.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
		this->_pollfd.fd = _newFd;
		this->_pollfd.events = POLLOUT;
		this->_pollAction = &PostMethod::write;
		PollManager::SetDirty();
		return false;
	}

	bool	PostMethod::PrepareToQuit(void) {
		close(_newFd);
		this->_pollAction =&PostMethod::quit;
		return false;
	}

	bool	PostMethod::quit(void) {
		return true;
	}

	bool	PostMethod::read(void) {
		std::string	line;

		while (true) {
			if (_strBuff.empty()) {
				while (!std::feof(_body)) {
					_strBuff.clear();
					ssize_t	readlen = std::fread(_buffer, 1, 1024, _body);
					if (readlen < 0) 
						return false;
					_strBuff = _buffer;
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
				return false;
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
		_fileName = FindWord(_strBuff, "filename=\"", '\"');
		_name = FindWord(_strBuff, "name=\"", '\"');
		if (_fileName == "")
			_fileName = _name;
		_strBuff = _strBuff.substr(_strBuff.find("\r\n\r\n"));
		_newFile = false;
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
			if (_strBuff.find(_eof) != std::string::npos) {
				_content = _strBuff.substr(0, _strBuff.find(_boundary));
				_endOfNewFile = true;
				_strBuff.clear();
				_strBuff = _eof;
			}
			else if (_strBuff.find(_boundary) != std::string::npos) {
				_content = _strBuff.substr(0, _strBuff.find(_boundary));
				_endOfNewFile = true;
				_newFile = true;
				_strBuff = _strBuff.substr(_strBuff.find(_boundary) + _boundary.size());
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
