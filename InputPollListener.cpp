/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputPollListener.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:06:00 by abaur             #+#    #+#             */
/*   Updated: 2021/11/01 14:55:43 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputPollListener.hpp"

#include "clibft/ErrnoException.hpp"
#include "logutil/logutil.hpp"
#include <unistd.h>

#define BUFFMAX	1024

namespace ft
{
	InputPollListener::InputPollListener(void) :
		fail(false),
		eof (false),
		buffer(),
		readAmount(0),
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
		this->SetFd(fd);
	}

	InputPollListener::InputPollListener(std::FILE& file){
		new(this) InputPollListener();
		this->SetFile(file);
	}

	InputPollListener::InputPollListener(ft::ifdstream& stream){
		new(this) InputPollListener();
		this->SetStream(stream);
	}

	InputPollListener::~InputPollListener() {}


	void	InputPollListener::SetFd(int fd){
		this->_fd         = fd;
		this->_pollfd.fd  = fd;
		this->_pollAction = &InputPollListener::ReadFd;
	}
	void	InputPollListener::SetFile(std::FILE& file){
		this->_file       = &file;
		this->_pollfd.fd  = ::fileno(&file);
		this->_pollAction = &InputPollListener::ReadFile;

		if (_pollfd.fd < 0)
			throw ft::ErrnoException("Invalid FILE* to fd conversion.");
	}
	void	InputPollListener::SetStream(ft::ifdstream& stream){
		this->_istream    = &stream;
		this->_pollfd.fd  = stream.fd;
		this->_pollAction = &InputPollListener::ReadStream;
	}


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
		// ft::clog << log::debug << "Read:  " << readAmount << ", Fail: " << fail
		//          << ", Eof: " << eof << std::endl;
	}

	void	InputPollListener::ReadFd(const pollfd&){
		char _buff[BUFFMAX];

		size_t	readlen = ::read(_fd, _buff, BUFFMAX);
		if (readlen < 0) 
			this->fail = true;
		else if (readlen == 0)
			this->eof = true;
		else {
			this->readAmount += readlen;
			this->buffer.append(_buff, readlen);
		}
	}

	void	InputPollListener::ReadFile(const pollfd&){
		char _buff[BUFFMAX];

		std::clearerr(_file);
		size_t readlen = std::fread(_buff, 1, BUFFMAX, _file);
	
		this->readAmount += readlen;
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

		this->readAmount += readlen;
		this->buffer.append(_buff, readlen);
		this->fail = _istream->fail();
		this->eof  = _istream->eof ();
	}

}
