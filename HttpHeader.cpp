/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:34:44 by abaur             #+#    #+#             */
/*   Updated: 2021/10/06 13:27:05 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpHeader.hpp"

#include "HttpCode.hpp"

namespace ft
{
	HttpHeader::HttpHeader(void) { this->Setcode(204); }

	HttpHeader::HttpHeader(int code) { this->Setcode(code); }

	HttpHeader::HttpHeader(int code, std::string extension) {
		this->Setcode(code);
		this->SetContentType(extension);
		this->SetLocation("");
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
		str << head << code << ' ' << ft::strhttp(code) << '\n';

		write(fd, str.str().c_str(), str.str().length());
	}

	void	HttpHeader::Setcode(int code) {
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

	void	HttpHeader::SetLocation(std::string location) {
		if (location != "")
			_location = "Location: " + location + "\n";
	}

	std::ostream&	HttpHeader::ToStream(std::ostream& out) const {
		out << "HTTP/1.1 " << _code << ' ' << ft::strhttp(_code) << "\n"
			"Server: ft_webserv\n"
			"Accept-Ranges: bytes\n"
			"Vary: Accept-Encoding\n"
			<< _location <<
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
