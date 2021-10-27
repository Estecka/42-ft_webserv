/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputPollListener.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:06:00 by abaur             #+#    #+#             */
/*   Updated: 2021/10/27 19:53:06 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputPollListener.hpp"

#include "clibft/ErrnoException.hpp"

#define BUFFMAX	1024

namespace ft
{
	InputPollListener::InputPollListener(void) :
		fail(false),
		eof (false),
		buffer(),
		_istream(NULL),
		_file(NULL),
		_fd(-1),
		_pollfd(),
		_pollAction(NULL)
	{
		this->_pollfd.events = POLLIN;
	}

	InputPollListener::InputPollListener(int fd){
		new(this) InputPollListener();
		this->_fd         = fd;
		this->_pollfd.fd  = fd;
		this->_pollAction = &InputPollListener::ReadFd;
	}

	InputPollListener::InputPollListener(std::FILE& file){
		new(this) InputPollListener();
		this->_file       = &file;
		this->_pollfd.fd  = ::fileno(&file);
		this->_pollAction = &InputPollListener::ReadFile;

		if (_pollfd.fd < 0)
			throw ft::ErrnoException("Invalid FILE* to fd conversion.");
	}

	InputPollListener::InputPollListener(ft::ifdstream& istream){
		new(this) InputPollListener();
		this->_istream    = &istream;
		this->_pollfd.fd  = istream.fd;
		this->_pollAction = &InputPollListener::ReadStream;
	}

	InputPollListener::~InputPollListener() {}



/******************************************************************************/
/* # PollListener Interface                                                   */
/******************************************************************************/

	void	InputPollListener::GetPollFd(pollfd& outpfd){
		outpfd = this->_pollfd;
	}

	void	InputPollListener::OnPollEvent(const pollfd& pfd){
		this->fail = false;
		this->eof  = false;
		(this->*_pollAction)(pfd);
	}

	void	InputPollListener::ReadFd(const pollfd&){
		char _buff[BUFFMAX];

		size_t	readlen = ::read(_fd, _buff, BUFFMAX);
		if (readlen < 0) 
			this->fail = true;
		else if (readlen == 0)
			this->eof = true;
		else
			this->buffer.append(_buff, readlen);
	}

	void	InputPollListener::ReadFile(const pollfd&){
		char _buff[BUFFMAX];

		std::clearerr(_file);
		size_t readlen = std::fread(_buff, 1, BUFFMAX, _file);
	
		this->buffer.append(_buff, readlen);
		this->fail = std::ferror(_file);
		this->eof  = std::feof(_file);
	}


	void	InputPollListener::ReadStream(const pollfd&){
		char _buff[BUFFMAX];

		_istream->clear();
		size_t readlen;
		for (readlen=0; readlen<BUFFMAX; readlen++){
			_istream->get(*(_buff + readlen));
			if (_istream->fail() || _istream->eof())
				break;
		}
		
		this->buffer.append(_buff, readlen);
		this->fail = _istream->fail();
		this->eof  = _istream->eof ();
	}

}
