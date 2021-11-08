/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi2Http.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 17:20:29 by abaur             #+#    #+#             */
/*   Updated: 2021/11/08 14:28:29 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi2Http.hpp"

#include "clibft/string.hpp"
#include "logutil/logutil.hpp"
#include "CgiKiller.hpp"
#include "TimeoutManager.hpp"
#include <cerrno>
#include <cstring>
#include <signal.h>
#include <sys/wait.h>

#define BUFFMAX	(sizeof(Cgi2Http::_buffer))

namespace ft
{
	Cgi2Http::Cgi2Http(RequestHandler& parent, pid_t cgiPid, int cgiPipeout) :
		_parent(parent),
		_cgiin(cgiPipeout),
		_httpout(parent.httpout),
		_cgiPid(cgiPid),
		_cgiParentPid(getpid()),
		_inFail(false),
		_inEof(false),
		_outFail(false),
		_outEof(false),
		_buffStart(0),
		_buffEnd(0)
	{
		ft::clog << log::info << &_parent << " Cgi2Http created. " << std::endl;
		this->PrepareToReadHead();
	}

	Cgi2Http::~Cgi2Http() {
		if (getpid() == _cgiParentPid) {
			pid_t waiterr = waitpid(_cgiPid, NULL, WNOHANG);
			if (0 < waiterr)
				ft::clog << log::info << "Fork " << _cgiPid << " terminated on its own." << std::endl;
			else {
				ft::clog << log::info << "Fork " << _cgiPid << " has yet to terminate on its own. Checking back later." << std::endl;
				TimeoutManager::AddListener(*new CgiKiller(_cgiPid, _cgiParentPid), 5);
			}
		}

		ft::clog << log::info << &_parent << " Cgi2Http destroyed." << std::endl;
	}


/******************************************************************************/
/* ## Listener Interface                                                      */
/******************************************************************************/

	void	Cgi2Http::GetPollFd(pollfd& outpfd){
		outpfd = this->_pollfd;
	}

	void	Cgi2Http::OnPollEvent(const pollfd& pfd){
		if (pfd.fd == _cgiin.fd)
			_inFail = _inEof = false;
		else if (pfd.fd == _httpout.fd)
			_outFail = _outEof = false;

		while (!(this->*_pollaction)())
			continue;
	}



/******************************************************************************/
/* ## Poll Event preparation                                                  */
/******************************************************************************/

	bool	Cgi2Http::PrepareToReadHead(){
		_pollfd.fd     = _cgiin.fd;
		_pollfd.events = POLLIN;
		_pollaction    = &Cgi2Http::ReadHead;
		return _inFail || _inEof;
	}

	bool	Cgi2Http::PrepareToReadBody(){
		_pollfd.fd     = _cgiin.fd;
		_pollfd.events = POLLIN;
		_pollaction    = &Cgi2Http::ReadBody;
		_buffStart = 0;
		_buffEnd   = 0;
		return _inFail || _inEof;
	}

	bool	Cgi2Http::PrepareToWriteHead(){
		_pollfd.fd     = _httpout.fd;
		_pollfd.events = POLLOUT;
		_pollaction    = &Cgi2Http::WriteHead;
		return _outFail || _outEof;
	}

	bool	Cgi2Http::PrepareToWriteBody(){
		_pollfd.fd     = _httpout.fd;
		_pollfd.events = POLLOUT;
		_pollaction    = &Cgi2Http::WriteBody;
		return _outFail || _outEof;
	}

	bool	Cgi2Http::PrepareToQuit(){
		_pollaction = NULL;
		_pollfd.fd = -1;

		delete &_parent;
		return true;
	}



/******************************************************************************/
/* ## Poll Actions                                                            */
/******************************************************************************/

	bool	Cgi2Http::ReadHead() {
		std::string line;

		// ft::clog << log::debug << "ReadHEad" << std::endl;
		_cgiin.clear();
		std::getline(_cgiin, line);
		_lineBuffer += line;
		_inFail = _cgiin.fail();
		_inEof  = _cgiin.eof();

		// ft::clog << log::debug << "Read: " << ft::BitToCString(line) << '\n'
		//           << "Fail: " << _inFail << ", Eof: " << _inEof
		//           << std::endl;

		if (_inFail && !_inEof)
			return true; // Repoll and continue reading
		else if (ft::StartsWith(_lineBuffer, "Status:")) {
			_lineBuffer = "HTTP/1.1 " + _lineBuffer.substr(7) + '\n';
			_headBuffer.str(_lineBuffer + _headBuffer.str());
			return PrepareToWriteHead();
		}

		std::string fullLine = _lineBuffer;
		_headBuffer << _lineBuffer;
		_lineBuffer = "";
		if (!_inEof)
			_headBuffer << '\n';

		if (_inEof || fullLine.find(':') == std::string::npos || fullLine == "\r"){
			ft::clog << log::warning << "No status code found in cgi output, assuming '200'." << std::endl;
			_headBuffer.str("HTTP/1.1 200 OK\r\n" + _headBuffer.str());
			return PrepareToWriteHead();
		}
		else
			return false; // Read next line immediately;
	}

	bool	Cgi2Http::WriteHead(){
		// ft::clog << log::debug << "WriteHead" << std::endl;
		if (_buffEnd <= _buffStart) {
			_headBuffer.read(_buffer, 1024);
			_buffStart = 0;
			_buffEnd   = _headBuffer.gcount();
			if (0 < _buffEnd)
				_parent.SetStreamingStarted();
		}
		if (_buffEnd <= 0)
			return PrepareToReadBody();

		char*  	writestart = _buffer  + _buffStart;
		ssize_t	writemax   = _buffEnd - _buffStart;

		ssize_t	writelen  = write(_httpout.fd, writestart, writemax);
		_outFail = (writelen < writemax);
		_outEof  = (writelen == 0);
		if (writelen < 0)
			writelen = 0;
		_buffStart += writelen;

		if (_outEof) {
			ft::clog << log::error << "Httpout closed while transmitting CGI." << std::endl;
			return PrepareToQuit();
		}

		return _outFail; // Continue writting, repoll if necessary.
	}

	bool	Cgi2Http::ReadBody(){
		// ft::clog << log::debug << "ReadBody" << std::endl;
		_cgiin.clear();
		while (true){
			_cgiin.get(*(_buffer + _buffEnd));
			if (_cgiin.fail())
				break;
			_buffEnd++;
			if (_buffEnd == BUFFMAX)
				break;
		}

		_inFail = _cgiin.fail();
		_inEof  = _cgiin.eof();

		if (_buffStart < _buffEnd)
			return PrepareToWriteBody();
		else if (_inEof)
			return PrepareToQuit();
		else if (_inFail)
			return true;
		else
			return false;
	}

	bool	Cgi2Http::WriteBody(){
		// ft::clog << log::debug << "WriteBody" << std::endl;
		char*  	writeStart = _buffer  + _buffStart;
		ssize_t	writeMax   = _buffEnd - _buffStart;

		ssize_t writeLen = write(_httpout.fd, writeStart, writeMax);
		_outFail = (writeLen < writeMax);
		_outEof  = (writeLen == 0);
		if (writeLen < 0)
			writeLen = 0;
		_buffStart += writeLen;

		if (_outEof) {
			ft::clog << log::error << "Httpout closed while transmitting Cgi." << std::endl;
			return PrepareToQuit();
		}
		if (_buffStart < _buffEnd)
			return _outFail; // Keep writting, repoll if needed.
		else if (!_inEof)
			return PrepareToReadBody();
		else
			return PrepareToQuit();
	}
}
