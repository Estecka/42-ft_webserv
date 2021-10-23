/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqBodyExtractor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:48:44 by abaur             #+#    #+#             */
/*   Updated: 2021/10/23 18:26:34 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReqBodyExtractor.hpp"

#include "PollManager.hpp"
#include "clibft/ErrnoException.hpp"
#include "logutil/logutil.hpp"

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
		ft::clog << log::debug << "BodyExtractor Created" << std::endl;
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
		ft::clog << log::debug << "BodyExtractor destroyed." << std::endl;
	}


	void	ReqBodyExtractor::GetPollFd(pollfd& outfd) {
		outfd = this->_pollfd;
	}
	void	ReqBodyExtractor::OnPollEvent(const pollfd& pfd) {
		if (pfd.fd == _httpin.fd) {
			_inFail = false;
			_inEof  = false;
		}
		else if (pfd.fd == _bodyfd)
		{
			_outFail = false;
			_outEof  = true;
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
		if (r){
			std::fflush(r);
			std::rewind(r);
		}
		_parent.OnBodyExtracted(r);
		return true;
	}

	bool	ReqBodyExtractor::read() {
		// ft::clog << log::debug << "About to read body...\n";
		_httpin.clear();
		while (true) 
		{
			char* 	readstart = _buffer + _buffstart;
			size_t	readmax   = BUFFMAX - _buffstart;
			size_t	readlen   = 0;

			while (_httpin.get(*(readstart+readlen)), !_httpin.fail()) {
				// std::cout.put(*(readstart+readlen));
				readlen++;
				if (readlen == readmax)
					break;
			}
			// std::cout.write(readstart, readlen);
			_bodylen += readlen;
			_buffend += readlen;

			_inFail = _httpin.fail() || _httpin.eof();
			_inEof  = _httpin.eof();
			// ft::clog << "Read:" << readlen << ", Fail:" << _httpin.fail() << ", Eof:" << _httpin.eof() << std::endl;

			if (_buffend > 0) {
				if (_buffend == BUFFMAX || _inFail)
					return this->PrepareToWrite();
				else
					continue;
			}
			else {
				if (_inEof)
					return this->PrepareToQuit();
				if (_inFail) {
					// ft::clog << log::debug << _bodylen << "vs " << _parent.GetReqHead()->GetContentLength() << std::endl;
					if (_bodylen >= _parent.GetReqHead()->GetContentLength())
						return this->PrepareToQuit();
					else
						return true;
				} 
				else
					continue;
			}
		}
	}

	bool	ReqBodyExtractor::write() {
		// ft::clog << log::debug << "About to write to temp file...\n";
		std::clearerr(_body);
		while (true) {
			char* 	writestart = _buffer  + _buffstart;
			size_t	writemax   = _buffend - _buffstart;

			size_t	writelen = std::fwrite(writestart, 1, writemax, _body);
			_buffstart += writelen;
			std::cout.write(writestart, writelen);

			_outFail = std::ferror(_body);
			_outEof  = std::feof  (_body);
			// ft::clog << "Wrote:" << writelen<<'/'<<writemax << ", Fail:" << _outFail << ", Eof:" << _outEof << std::endl;

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
