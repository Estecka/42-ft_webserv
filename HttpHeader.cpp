/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:34:44 by abaur             #+#    #+#             */
/*   Updated: 2021/09/30 12:23:33 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpHeader.hpp"

namespace ft
{
	HttpHeader::HttpHeader(void) { this->Setcode(204); }

	HttpHeader::HttpHeader(int code) { this->Setcode(code); }

	HttpHeader::HttpHeader(int code, std::string extension) {
		this->Setcode(code);
		this->SetContentType(extension);
	}
	HttpHeader::HttpHeader(const HttpHeader& other) {
		this->operator=(other);
	}
	HttpHeader::~HttpHeader(){}

	HttpHeader&	HttpHeader::operator=(const HttpHeader& other){
		this->Setcode(other._code);
		this->SetContentType(other._contentType);
		return *this;
	}

	void	HttpHeader::SendErrCode(int code, int fd){
		HttpHeader	head;
		std::stringstream str;

		head.Setcode(code);
		str << head << code << ' ' << head._codeMsg << '\n';

		write(fd, str.str().c_str(), str.str().length());
	}

	void	HttpHeader::Setcode(int code) {
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
			case 410:	_codeMsg = "Gone";	break;
			case 413:	_codeMsg = "Request Entity Too Large";	break;
			case 415:	_codeMsg = "Unsupported Media Type";	break;
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


	void	HttpHeader::SetContentType(std::string extension) {
		#define	TYPEC 15
		static const char*const	types[TYPEC][2] = {
			{ ".html",	"text/html"        },
			{ ".htm", 	"text/html"        },
			{ ".txt", 	"text/plain"       },
			{ ".xml", 	"text/xml"         },
			{ ".css", 	"text/css"         },
			{ ".php",	"text/html"		   },
			{ ".gif", 	"image/gif"        },
			{ ".jpg", 	"image/jpeg"       },
			{ ".jpeg",	"image/jpeg"       },
			{ ".png", 	"image/png"        },
			{ ".ico", 	"image/x-icon"     },
			{ ".pdf", 	"application/pdf"  },
			{ ".zip", 	"application/zip"  },
			{ ".json",	"application/json" },
			{ ".mp4",	"video/mp4"        },
		};

		for (int i=0; i<TYPEC; i++)
		if (extension == types[i][0]) {
			this->_contentType = types[i][1];
			return;
		}

		this->_contentType = "application/octet-stream";
	}

	std::ostream&	HttpHeader::ToStream(std::ostream& out) const {
		out << "HTTP/1.1 " << _code << ' ' << _codeMsg << "\n"
			"Server: ft_webserv\n"
			"Accept-Ranges: bytes\n"
			"Vary: Accept-Encoding\n"
			"Content-Type: " << _contentType << "\n"
			"\n"
		;
		return out;
	}
	std::string	HttpHeader::ToString() const {
		std::stringstream stream;
		this->ToStream(stream);
		return stream.str();
	}
	void	HttpHeader::ToFd(int fd) const {
		std::string str = this->ToString();
		write(fd, str.c_str(), str.length());
	}

}

std::ostream&	operator<<(std::ostream& dst, const ft::HttpHeader& src) {
	return src.ToStream(dst);
}
