/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:16:21 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/12 14:47:52 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"
#include "HttpCode.hpp"
#include "logutil/logutil.hpp"

namespace ft {

ErrorPage::ErrorPage(int code, int acceptfd, RequestHandler& parent) :
	_acceptfd(acceptfd),
	_parent(parent)
{
	this->SetPage(code, strhttp(code), deschttp(code));
	ft::clog << log::info << &_parent << " Error Page created: " << code << std::endl;
}

ErrorPage::ErrorPage(int code, int acceptfd, RequestHandler& parent, const std::string& msg) :
	_acceptfd(acceptfd),
	_parent(parent)
{
	this->SetPage(code, strhttp(code), msg.c_str());
	ft::clog << log::info << &_parent << " Error Page created: " << code << std::endl;
}

ErrorPage::ErrorPage(const ErrorPage &other):_parent(other._parent) { this->operator=(other); }

ErrorPage::~ErrorPage(void) {
	ft::clog << log::info << &_parent << " Error Page destroyed." << std::endl;
}

ErrorPage	&ErrorPage::operator=(const ErrorPage& other) {
	this->_strPage = other._strPage;
	return (*this);
}
void	ErrorPage::SetPage(int code, const char* title, const char* msg) {
	ft::ResponseHeader	header(code, ".html");
	std::stringstream page;
	page << header.ToString();
	page << \
	"<!DOCTYPE html>\n"
	"<html>\n"
	"	<title>" << title << "</title>\n"
	"	<body>\n"
	"		<div>\n"
	"			<h1>" << code << ' ' << title << "</h1>\n"
	"			<p>" << msg << "<br><br><br></p>\n"
	"		</div>\n"
	"		<hr>\n"
	"		<p> abaur | WEBSERV | apitoise<br></p>\n"
	"	</body>\n"
	"</html>\n"
	;
	_strPage = page.str();
}

void	ErrorPage::OnPollEvent(const pollfd&) {
	std::size_t	len = write(_acceptfd, _strPage.data(), _strPage.size());
	if (len < 0)
		return;
	else if (len == 0) {
		ft::clog << log::error << "Client connection closed while sending error page." << std::endl;
		delete &_parent;
	}
	else if (len < _strPage.size())
		_strPage = _strPage.substr(len);
	else
		delete &_parent;
}

void	ErrorPage::GetPollFd(pollfd& poll_fd) {
	poll_fd.fd = _acceptfd;
	poll_fd.events = POLLOUT;
}

}
