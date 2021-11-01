/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqBodyExtractor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:48:44 by abaur             #+#    #+#             */
/*   Updated: 2021/11/01 14:54:32 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReqBodyExtractor.hpp"

#include "PollManager.hpp"
#include "clibft/ErrnoException.hpp"
#include "logutil/logutil.hpp"

#include <cstdio>
#include <fcntl.h>

#define BUFFMAX	sizeof(ReqBodyExtractor::_buffer)

namespace ft
{
	ReqBodyExtractor::ReqBodyExtractor(RequestHandler& parent) :
		_parent(parent)
	{
		ft::clog << log::info << &_parent << " BodyExtractor Created" << std::endl;
		_body = std::tmpfile();
		if (_body == NULL)
			throw ft::ErrnoException("Failed to create a temporary file to hold request body.");

		int bodyfd = fileno(_body);
		if (bodyfd < 0)
			throw ft::ErrnoException("Bad FILE* to file descriptor conversion.");

		int err = fcntl(bodyfd, F_SETFL, O_NONBLOCK);
		if (err < 0)
			throw ft::ErrnoException("Couldn't set temporary file to non-blocking");

		_input.SetStream(_parent.httpin);
		_output.SetFile(*_body);
		this->PrepareToRead();
	}

	ReqBodyExtractor::~ReqBodyExtractor(){
		if (this->_body)
			std::fclose(_body);
		ft::clog << log::info << &_parent << " BodyExtractor destroyed." << std::endl;
	}


	void	ReqBodyExtractor::GetPollFd(pollfd& outfd) {
		_activeListener->GetPollFd(outfd);
	}
	void	ReqBodyExtractor::OnPollEvent(const pollfd& pfd) {
		bool	fail = false;
		while (!fail){
			this->_activeListener->OnPollEvent(pfd);
			fail = (this->*_pollAction)(pfd);
		}
	}

	bool	ReqBodyExtractor::PrepareToRead() {
		this->_activeListener = &_input;
		this->_pollAction    = &ReqBodyExtractor::read;
		PollManager::SetDirty();
		return _input.fail && _input.eof;
	}
	bool	ReqBodyExtractor::PrepareToWrite() {
		this->_activeListener = &_output;
		this->_pollAction = &ReqBodyExtractor::write;
		PollManager::SetDirty();
		return _output.fail && _output.eof;
	}
	bool	ReqBodyExtractor::PrepareToQuit() {
		FILE* r = NULL;
		if (_output.writeAmount)
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

	bool	ReqBodyExtractor::read(const pollfd&) {
		if (!_input.buffer.empty())
		{
			_output.buffer.append(_input.buffer);
			_input.buffer.clear();
			return this->PrepareToWrite();
		}
		else if (_input.eof){
			ft::clog << log::error << "Eof encountered while reading body." << std::endl;
			return this->PrepareToQuit();
		}
		else if (!_input.fail)
			return false;
		else if (_parent.GetReqHead()->GetContentLength() <= _input.readAmount)
			return this->PrepareToQuit();
		else
			return true;
	}

	bool	ReqBodyExtractor::write(const pollfd&) {
		if (_output.buffer.empty())
			return this->PrepareToRead();
		else if (_output.eof) {
			ft::clog << log::error << "Eof encountered while writing to body temporary file." << std::endl;
			return this->PrepareToQuit();
		}
		else
			return _output.fail;
	}
}
