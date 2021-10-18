/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:34:44 by abaur             #+#    #+#             */
/*   Updated: 2021/10/09 13:57:34 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHeader.hpp"

#include "HttpCode.hpp"

namespace ft
{
	ResponseHeader::ResponseHeader(void) { this->Setcode(204); }

	ResponseHeader::ResponseHeader(int code) { this->Setcode(code); }

	ResponseHeader::ResponseHeader(int code, std::string extension) {
		this->Setcode(code);
		this->SetContentType(extension);
		this->SetLocation("");
	}
	ResponseHeader::ResponseHeader(const ResponseHeader& other) {
		this->operator=(other);
	}
	ResponseHeader::~ResponseHeader(){}

	ResponseHeader&	ResponseHeader::operator=(const ResponseHeader& other){
		this->Setcode(other._code);
		this->SetContentType(other._contentType);
		return *this;
	}

	void	ResponseHeader::SendErrCode(int code, int fd){
		ResponseHeader	head;
		std::stringstream str;

		head.Setcode(code);
		str << head << code << ' ' << ft::strhttp(code) << '\n';

		write(fd, str.str().c_str(), str.str().length());
	}

	void	ResponseHeader::Setcode(int code) {
		this->_code = code;
	}


	void	ResponseHeader::SetContentType(std::string extension) {
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

	void	ResponseHeader::SetLocation(std::string location) {
		if (location != "")
			_location = "Location: " + location + "\n";
	}

	std::string ResponseHeader::GetContentType(void) { return (_contentType); }

	std::ostream&	ResponseHeader::ToStream(std::ostream& out) const {
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
	std::string	ResponseHeader::ToString() const {
		std::stringstream stream;
		this->ToStream(stream);
		return stream.str();
	}
	void	ResponseHeader::ToFd(int fd) const {
		std::string str = this->ToString();
		write(fd, str.c_str(), str.length());
	}

}

std::ostream&	operator<<(std::ostream& dst, const ft::ResponseHeader& src) {
	return src.ToStream(dst);
}
