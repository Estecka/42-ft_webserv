/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 15:24:23 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "PollManager.hpp"

#include <cstdlib>

namespace ft
{
	RequestHandler::RequestHandler(fd_ip ip_fd, int port) :
		httpin(ip_fd.acceptfd),
		httpout(ip_fd.acceptfd),
		_port(port), _code(200) {
		_clientIP = ip_fd.ip;
		std::cerr << "[DEBUG] RequestHandler created." << std::endl;
		fcntl(ip_fd.acceptfd, F_SETFL, O_NONBLOCK);
		this->PollInit();
	}

	RequestHandler::~RequestHandler() {
		if (this->_header)
			delete _header;
		std::cerr << "[DEBUG] RequestHandler destroyed." << std::endl;
	}


	void	RequestHandler::GetPollFd(pollfd& outpfd) {
		outpfd = this->_pollfd;
	}
	void	RequestHandler::OnPollEvent(const pollfd& pfd) {
		(this->*_onPollEvent)(pfd);
	}



	void	RequestHandler::PollInit(){
		_pollfd.fd      = httpin.fd;
		_pollfd.events  = POLLIN;
		_pollfd.revents = 0;
		_onPollEvent    = &RequestHandler::ExtractRequestHeader;

		PollManager::AddListener(*this);
	}

	void	RequestHandler::ExtractRequestHeader(const pollfd& pfd){
		std::string	line;

		httpin.clear();
		while (true) {
			std::getline(httpin, line);
			_stringbuff << line;

			std::cerr << "[DEBUG] Reading Request Header: " << BitToCString(line) << "\n"
			          << "        Fail: " << httpin.fail() << ", Eof: " << httpin.eof()
			          << std::endl;

			if (httpin.eof())
				break;
			if (httpin.fail())
				return;
			else {
				_stringbuff << '\n';
				if (line == "" || line == "\r")
					break;
			}
		}

		if (_stringbuff.str().length() > 0) {
			std::cout << _stringbuff.str() << std::endl;
			this->_header = new HttpRequest(_stringbuff);
		}
		else {
			std::cerr << "[WARN] Empty request received on port " << this->_port << std::endl;
			this->_header = NULL;
		}

		_stringbuff.str("");
		_stringbuff.clear();
		_onPollEvent = &RequestHandler::ExtractRequestBody;
		this->ExtractRequestBody(pfd);
	}

	// Placeholder function. For now, the request body is discarded.
	void	RequestHandler::ExtractRequestBody(const pollfd&){
		httpin.clear();
		while (true) {
			httpin.ignore(1024);

			std::cerr << "[DEBUG] Discarding request body: " << httpin.gcount() << " bytes.\n"
			          << "        Fail: " << httpin.fail() << ", Eof: " << httpin.eof()
			          << std::endl;

			if (httpin.fail())
				break;
		}

		_pollfd.fd = httpout.fd;
		_pollfd.events = POLLOUT;
		_onPollEvent = &RequestHandler::CheckRequest;
		PollManager::SetDirty();
	}

	void	RequestHandler::CheckRequest(const pollfd&) {
		if (_header == NULL)
			_code = 418;
		else if (!_header->IsOk()) {
			std::cerr << "[WARN] Invalid request received on port " 
			          << this->_port << std::endl;
			_code = 400;
		}
		else if(_header->GetHostPort() != this->_port) {
			std::cerr << "[WARN] Port mismatch: got " << _header->GetHostPort() 
			          << "instead of " << this->_port << std::endl;
			_code = 422;
		}
		else if (_header->GetMajorHttpVersion() != 1 || _header->GetMinorHttpVersion() != 1)
			_code = 505;
		if (_code == 200)
			_onPollEvent = &RequestHandler::DispatchRequest;
		else {
			_onPollEvent = &RequestHandler::SendErrorPage;
		}
	}

	void	RequestHandler::DispatchRequest(const pollfd&) {
		bool serverfound = false;
		for (std::list<ft::Server>::iterator it=Server::availableServers.begin(); it!=Server::availableServers.end(); it++)
			if (it->MatchRequest(*_header)) {
				it->Accept(httpin.fd, *_header, _clientIP);
				serverfound = true;
				break;
			}
		if (!serverfound) {
			std::cerr << "[ERR] No server found to answer request at: " << _header->GetHost() << std::endl;
			HttpHeader::SendErrCode(404, httpin.fd);
		}
		PollManager::RemoveListener(*this);
		delete this;
	}
	
	void	RequestHandler::SendErrorPage(const pollfd&) {
		ft::HttpHeader		header(_code, ".html");
		std::string			title;
		std::string			msg;
		std::stringstream	_errorPage;

		switch (_code) {
			case 204:	title = "204 No Content"; msg = "This request is not returning any content.";	break;
			case 301:	title = "301 Moved Permanently"; msg = "This content has been moved permanently.";	break;
			case 302:	title = "302 Found"; msg = "This content has been moved temporarily.";	break;
			case 303:	title = "303 See Other"; msg = "This content is somewhere else.";	break;
			case 400:	title = "400 Bad Request"; msg = "Bad Request.";	break;
			case 401:	title = "401 Unauthorized"; msg = "Authorization required.";	break;
			case 403:	title = "403 Forbidden"; msg = "Request unauthorized due to invalid permissions or credentials.";	break;
			case 404:	title = "404 Not Found"; msg = "Page not found.";	break;
			case 405:	title = "405 Method Not Allowed"; msg = "The requested methodis not allowed.";	break;
			case 406:	title = "406 Not Acceptable"; msg = "An appropriate representation of the requested resource could not be found on this server.";	break;
			case 410:	title = "410 Gone"; msg = "The requested ressource is no longer available and will not be available again.";	break;
			case 413:	title = "413 Request Entity Too Large"; msg = "Your client issued a request that was too large.";	break;
			case 415:	title = "415 Unsupported Media Type"; msg = "The file type of the request is unsupported.";	break;
			case 418:	title = "418 I'm a teapot"; msg = "Just a teapot.";	break;
			case 422:	title = "422 Unprocessable Entity"; msg = "Request with semantical errors.";	break;
			case 429:	title = "429 Too Many Requests"; msg = "You have sent too many requests in a given amount of time.";	break;
			case 500:	title = "500 Internal Server Error"; msg = "Server is not responding, but it is alive ! Try again.";	break;
			case 501:	title = "501 Not Implemented"; msg = "The requested method is not implemented by the server.";	break;
			case 503:	title = "503 Service Unavailable"; msg = "The server is temporarily busy, try again later.";	break;
			case 505:	title = "505 HTTP Version Not Supported"; msg = "HTTP Version not supported.";	break;
		}
		while (true) {
			_errorPage << header.ToString();
			_errorPage << \
			"<!DOCTYPE html>\n\
			<html>\n\
				<title>" + title + "</title>\n\
				<body>\n\
					<div>\n\
						<H1>" + title + "</H1>\n\
						<p>" + msg + "<br><br><br></p>\n\
					</div>\n\
					<hr>\n\
					<p> abaur | WEBSERV | apitoise<br></p>\n\
				</body>\n\
			</html>\n\
			";
			std::size_t	len = write(httpin.fd, _errorPage.str().c_str(), _errorPage.str().length());
			if (len < 0)
				return ;
			else if (len < _errorPage.str().length())
				_errorPage.str().substr(len);
			break;
		}
		PollManager::RemoveListener(*this);
		delete this;
	}
}
