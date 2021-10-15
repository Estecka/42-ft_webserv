/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqBodyExtractor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:48:44 by abaur             #+#    #+#             */
/*   Updated: 2021/10/15 15:02:59 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReqBodyExtractor.hpp"

#include "PollManager.hpp"
#include "clibft/ErrnoException.hpp"

#include <cstdio>

#define BUFFMAX	sizeof(ReqBodyExtractor::_buffer)

namespace ft
{
	ReqBodyExtractor::ReqBodyExtractor(RequestHandler& parent) :
		_parent(parent),
		_httpin(parent.httpin),
		_bodylen(0),
		_inFail(false),
		_inEof(false),
		_outFail(false),
		_outEof(false)
	{
		_body = std::tmpfile();
		if (_body == NULL)
			throw ft::ErrnoException("Failed to create a temporary file to hold request body.");

		_bodyfd = fileno(_body);
		if (_bodyfd < 0)
			throw ft::ErrnoException("Bad FILE* to file descriptor conversion.");

		int err = fcntl(_bodyfd, F_SETFL, O_NONBLOCK);
		if (err < 0)
			throw ft::ErrnoException("Couldn't set temporary file to non-blocking");

		this->PrepareToRead();
	}

	ReqBodyExtractor::~ReqBodyExtractor(){
		if (this->_body)
			std::fclose(_body);
	}


	void	ReqBodyExtractor::GetPollFd(pollfd& outfd) {
		outfd = this->_pollfd;
	}
	void	ReqBodyExtractor::OnPollEvent(const pollfd& pfd) {
		if (pfd.fd == _httpin.fd) {
			_inFail = false;
			_inEof  = false;
			_httpin.clear();
		}
		else if (pfd.fd == _bodyfd)
		{
			_outFail = false;
			_outEof  = true;
			std::clearerr(_body);
		}

		while (!(this->*_pollAction)())
			continue;
	}

	bool	ReqBodyExtractor::PrepareToRead() {
		this->_pollfd.fd     = _parent.httpin.fd;
		this->_pollfd.events = POLLIN;
		this->_pollAction    = &ReqBodyExtractor::read;
		PollManager::SetDirty();
		this->_buffstart = 0;
		this->_buffend   = 0;
		return _inFail && _inEof;
	}
	bool	ReqBodyExtractor::PrepareToWrite() {
		this->_pollfd.fd = _bodyfd;
		this->_pollfd.events = POLLOUT;
		this->_pollAction = &ReqBodyExtractor::write;
		PollManager::SetDirty();
		return _outFail && _outEof;
	}
	bool	ReqBodyExtractor::PrepareToQuit() {
		FILE* r = NULL;
		if (_bodylen)
		 	r = _body;
		else
			std::fclose(_body);
		this->_pollAction = NULL;
		this->_body = NULL;
		_parent.OnBodyExtracted(r);
		return true;
	}

	bool	ReqBodyExtractor::read() {
		while (true) 
		{
			std::cerr << "[DEBUG] About to read body...\n";
			char* 	readstart = _buffer + _buffstart;
			size_t	readmax   = BUFFMAX - _buffstart;
			_httpin.read(readstart, readmax);
			_buffend += _httpin.gcount();

			_inFail = _httpin.fail() || _httpin.eof();
			_inEof  = _httpin.fail() || _httpin.eof();
			std::cerr << "        Read:" << _httpin.gcount() << ", Fail:" << _httpin.fail() << ", Eof:" << _httpin.eof() << std::endl;

			if (_buffend > 0) {
				if (_buffend == BUFFMAX || _inFail)
					return this->PrepareToWrite();
				else
					continue;
			}
			else {
				if (_inFail)
					return this->PrepareToQuit();
				else
					continue;
			}
		}
	}

	bool	ReqBodyExtractor::write() {
		while (true) {
			std::cerr << "[DEBUG] About to write to temp file...\n";
			char* 	writestart = _buffer  + _buffstart;
			size_t	writemax   = _buffend - _buffstart;

			size_t	writelen = std::fwrite(writestart, 1, writemax, _body);
			_buffstart += writelen;
			_bodylen   += writelen;

			_outFail = std::ferror(_body);
			_outEof  = std::feof  (_body);
			std::cerr << "        Wrote:" << writelen<<'/'<<writemax << ", Fail:" << _outFail << ", Eof:" << _outEof << std::endl;

			if (_buffstart == _buffend) {
				if (_inEof)
					return this->PrepareToQuit();
				else
					return this->PrepareToRead();
			}
			else if (_outEof){
				throw ErrnoException("Body closed before writting could finish.");
			}
			else if (_outFail)
				return false;
			else
				continue;
		}
	}
}
