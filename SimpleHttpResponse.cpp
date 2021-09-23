/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleHttpResponse.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:34:44 by abaur             #+#    #+#             */
/*   Updated: 2021/09/22 17:33:33 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SimpleHttpResponse.hpp"

#include "unistd.h"

namespace ft
{
	SimpleHttpResponse::SimpleHttpResponse(int code, const std::string& body) {
		this->Setcode(code);
		this->_body.str(body);
	}
	SimpleHttpResponse::SimpleHttpResponse(const SimpleHttpResponse& other) {
		this->operator=(other);
	}
	SimpleHttpResponse::~SimpleHttpResponse(){}

	SimpleHttpResponse&	SimpleHttpResponse::operator=(const SimpleHttpResponse& other){
		this->Setcode(other._code);
		this->_body.str(other._body.str());
		return *this;
	}


	std::ostream&	SimpleHttpResponse::GetStream(){ return this->_body; }

	void	SimpleHttpResponse::Setcode(int code) {
		switch (code) {
			default:	code = 500;

			case 200:	_codeMsg = "OK";	break;
			case 204:	_codeMsg = "No Content";	break;
			case 301:	_codeMsg = "Moved Permanently";	break;
			case 302:	_codeMsg = "Found";	break;
			case 303:	_codeMsg = "See Other";	break;
			case 400:	_codeMsg = "Bad Request";	break;
			case 401:	_codeMsg = "Unauthorized";	break;
			case 403:	_codeMsg = "Forbidden";	break;
			case 404:	_codeMsg = "Not Found";	break;
			case 406:	_codeMsg = "Not Acceptable";	break;
			case 418:	_codeMsg = "I'm a teapot";	break;
			case 422:	_codeMsg = "Unprocessable Entity";	break;
			case 429:	_codeMsg = "Too Many Requests";	break;
			case 500:	_codeMsg = "Internal Server Error";	break;
			case 501:	_codeMsg = "Not Implemented";	break;
			case 503:	_codeMsg = "Service Unavailable";	break;
			case 505:	_codeMsg = "HTTP Version Not Supported";	break;
		
		}
		this->_code = code;
	}

	std::ostream&	SimpleHttpResponse::ToStream(std::ostream& out) const {
		out << "HTTP/1.1 " << _code << ' ' << _codeMsg << "\n"
			"Server: ft_webserv\n"
			"Accept-Ranges: bytes\n"
			"Vary: Accept-Encoding\n"
			"Content-Type: text/plain\n"
			"\n"
		;
		if (_body.str().empty() && _code != 204)
			out << _code << ' ' << _codeMsg << std::endl;
		else
			out << _body.str() << std::endl;
		return out;
	}
	std::string	SimpleHttpResponse::ToString() const {
		std::stringstream stream;
		this->ToStream(stream);
		return stream.str();
	}
	void	SimpleHttpResponse::ToFd(int fd) const {
		std::string str = this->ToString();
		write(fd, str.c_str(), str.length());
	}

}

std::ostream&	operator<<(std::ostream& dst, ft::SimpleHttpResponse& src) {
	return src.ToStream(dst);
}
