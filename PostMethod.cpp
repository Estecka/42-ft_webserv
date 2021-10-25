/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/25 16:04:02 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"
#include "PollManager.hpp"

namespace ft {

	PostMethod::PostMethod(FILE* body, RequestHandler& parent): _body(body), _parent(parent) {
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
		_parent.Destroy();
		return false;
	}

	bool	PostMethod::PrepareToQuit(void) {
		close(_newFd);
		return true;
	}

	bool	PostMethod::read(void) {
		while (true) {
			while (!std::feof(_body)) {
				ssize_t	readlen = std::fread(_buffer, 1, 1024, _body);
				_ssBody << ft::BitToCString(_buffer, readlen);
			}
			break ;
		}
		this->_pollAction = &PostMethod::parse;
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
		this->_pollAction = &PostMethod::PrepareToQuit;
		return false;
	}

	bool	PostMethod::parse(void) {
		size_t	start = 0;
		size_t	len;
		_boundary = _ssBody.str().substr(1, _ssBody.str().find("\\"));
		while ((start = _ssBody.str().find(_boundary, start)) != std::string::npos) {

			len = _ssBody.str().find(_boundary, start + _boundary.size()) - start - _boundary.size();
	
			_newFilesList.push_back(_ssBody.str().substr(start + _boundary.size(), len));
			start += _boundary.size();
		}
		for (std::list<std::string>::iterator it = _newFilesList.begin(); it != _newFilesList.end(); it++) {
			_contentType = findWord(*it, "Content-Type: ", '\\');
			_fileName = findWord(*it, "filename=\"", '\"');
			_name = findWord(*it, "name=\"", '\"');
			if (_fileName == "")
				_fileName = _name;
			_contentDisposition = findWord(*it, "Content-Disposition: ", ';');
			_content =  findWord(*it, "\\r\\n\\r\\n", '\0');
			if (*it == _newFilesList.back()) {
				_content = _content.substr(0, _content.size() - 64);
			}
			std::cerr << RED << "[" << _fileName << "] : \n" << _content << RESET << std::endl;
		}
		this->_pollAction = &PostMethod::PrepareToWrite;
		return false;
	}

	std::string	PostMethod::findWord(std::string content, std::string toFind, char sep) {
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
