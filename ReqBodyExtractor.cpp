/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqBodyExtractor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:48:44 by abaur             #+#    #+#             */
/*   Updated: 2021/11/12 14:10:32 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReqBodyExtractor.hpp"

#include "PollManager.hpp"
#include "HttpCode.hpp"
#include "HttpException.hpp"
#include "clibft/ErrnoException.hpp"
#include "logutil/logutil.hpp"

#include <algorithm>
#include <cstdio>
#include <fcntl.h>

#define NPOS	std::string::npos

namespace ft
{
	ReqBodyExtractor::ReqBodyExtractor(RequestHandler& parent) :
		_parent(parent),
		_request(*parent.GetReqHead()),
		_chunkToRead(0)
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

		if (!_request.IsChunked())
			this->_chunkToRead = _request.GetContentLength();
		this->PrepareToRead();
	}

	ReqBodyExtractor::~ReqBodyExtractor(){
		if (this->_body)
			std::fclose(_body);
		ft::clog << log::info << &_parent << " BodyExtractor destroyed." << std::endl;
	}


/******************************************************************************/
/* ## PollListener Interface                                                  */
/******************************************************************************/

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


/******************************************************************************/
/* ## Preparation                                                             */
/******************************************************************************/

	bool	ReqBodyExtractor::PrepareToRead() {
		this->_activeListener = &_input;
		this->_pollAction     = (_request.IsChunked()) ? &ReqBodyExtractor::ChunkedRead : &ReqBodyExtractor::Read;
		PollManager::SetDirty();
		return _input.fail && _input.eof;
	}
	bool	ReqBodyExtractor::PrepareToWrite() {
		this->_activeListener = &_output;
		this->_pollAction = &ReqBodyExtractor::Write;
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
		_parent.OnBodyExtracted(r, _output.writeAmount);
		return true;
	}


/******************************************************************************/
/* ## Poll Actions                                                            */
/******************************************************************************/

	bool	ReqBodyExtractor::Read(const pollfd&) {
		if (!_input.buffer.empty())
		{
			std::cout << _input.buffer;
			if (_input.buffer.length() > _chunkToRead)
				throw ft::HttpException(HTTP_BAD_REQUEST, "Request body is longer than expected.");
			_chunkToRead -= _input.buffer.length();
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
		else if (_request.GetContentLength() <= _input.readAmount)
			return this->PrepareToQuit();
		else
			return true;
	}


	static short	HexcharToInt(char c){
		if ('0' <= c && c <= '9')
			return c - '0';
		if ('a' <= c && c <= 'f')
			return c - 'a' + 0xa;
		throw std::runtime_error((std::string)"Not an hex char" + c);
	}

	/**
	 * @return
	 * 	 1 OK
	 * 	 0 Not enough characters to work with.
	 * 	-1 Invalid syntax
	 */
	static short	ExtractChunkLength(std::string& inbuffer, size_t& outlen) {
		static const std::string validChars("0123456789abcdef");
		size_t lineend = inbuffer.find("\r\n");
		if (lineend == NPOS)
			return 0;

		size_t chunksize = 0;
		for (size_t i=0; i<lineend; i++){
			if (validChars.find(inbuffer[i]) == NPOS){
				// ft::clog << log::warning << "Invalid character in hex number: "
				// 	<< ft::BitToCString(inbuffer.data(), lineend) << std::endl;
				return -1;
			}
			chunksize *= 0x10;
			chunksize += HexcharToInt(inbuffer[i]);
		}

		inbuffer.erase(0, lineend+2);
		outlen = chunksize;
		return 1;
	}


	bool	ReqBodyExtractor::ChunkedRead(const pollfd&) {
		if (_chunkToRead == 0){
			// ft::clog << log::debug << "Trying to read chunk size..." << std::endl;
			short r = ExtractChunkLength(_input.buffer, _chunkToRead);
			if (r < 0)
				throw ft::HttpException(HTTP_BAD_REQUEST, "Malformed chunked request.");
			if (r == 0) {
				if (_input.eof)
					throw ft::HttpException(HTTP_BAD_REQUEST, "Client connection closed while transmitting request.");
				else
					return true;
			}
			// ft::clog << log::debug << "New chunk size is: " << _chunkToRead << std::endl;
		}

		if (_chunkToRead == 0)
			return PrepareToQuit();
		else if (_input.buffer.empty()){
			// ft::clog << log::debug << "Input buffer was empty.\n" 
			//          << "Fail: " << _input.fail << ", Eof: " << _input.eof << std::endl;
			if (_input.eof)
				throw ft::HttpException(HTTP_BAD_REQUEST, "Client Connection closed while transmitting request.");
			else
				return _input.fail;
		}
		else {
			size_t extractlen = std::min(_chunkToRead, _input.buffer.length());
			// ft::clog << log::debug << "Extracting " << extractlen << " bytes:\n"
			//          << ft::BitToCString(_input.buffer.data(), extractlen) << std::endl;
			_output.buffer.append(_input.buffer.data(), extractlen);
			_input.buffer.erase(0, extractlen);
			_chunkToRead -= extractlen;
			return PrepareToWrite();
		}
	}

	bool	ReqBodyExtractor::Write(const pollfd&) {
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
