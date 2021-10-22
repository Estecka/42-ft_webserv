/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/22 13:41:43 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"
#include "PollManager.hpp"

namespace ft {

	PostMethod::PostMethod(FILE* body, RequestHandler& parent): _body(body), _parent(parent) {
		std::cerr << "[DEBUG] PostMethod created." << std::endl;
		_bodyfd = fileno(_body);
		_tmpFile = std::tmpfile();
		_tmpFileFd = fileno(_tmpFile);
		int	err = fcntl(_tmpFileFd, F_SETFL, O_NONBLOCK);
		if (err < 0)
			throw ft::ErrnoException("Couldn't set temporary file to non-blocking");
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
	}

	bool	PostMethod::PrepareToRead(void) {
		this->_pollfd.fd = _bodyfd;
		this->_pollfd.events = POLLIN;
		this->_pollAction = &PostMethod::read;
		PollManager::SetDirty();
		return false;
	}

	bool	PostMethod::PrepareToWrite(void) {
		this->_pollfd.fd = _tmpFileFd;
		this->_pollfd.events = POLLOUT;
	//	this->_pollAction = &PostMethod::write;
		PollManager::SetDirty();
		_parent.Destroy();
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
		this->_pollAction = &PostMethod::findName;
		return false;
	}

	bool	PostMethod::findName(void) {
		const char	*str = _ssBody.str().c_str();
		int		len = 0;
		size_t	pos = _ssBody.str().find("filename=") + 10;
		
		for (size_t i = 0; i < pos; i++)
			str++;
		while (*str != '\"') {
			len++;
			str++;
		}
		_fileName = _ssBody.str().substr(pos,len);
		this->_pollAction = &PostMethod::PrepareToWrite;
		return false;
	}
	

}
