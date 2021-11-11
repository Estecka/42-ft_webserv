/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:16:21 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/11 17:36:35 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"
#include "HttpCode.hpp"
#include "logutil/logutil.hpp"

namespace ft {

ErrorPage::ErrorPage(int code, int acceptfd, RequestHandler& parent): _code(code), _acceptfd(acceptfd), _parent(parent){
	this->SetPage();
	ft::clog << log::info << &_parent << " Error Page created." << std::endl;
}

ErrorPage::ErrorPage(const ErrorPage &other):_parent(other._parent) { this->operator=(other); }

ErrorPage::~ErrorPage(void) {
	ft::clog << log::info << &_parent << " Error Page destroyed." << std::endl;
}

ErrorPage	&ErrorPage::operator=(const ErrorPage& other) {
	_code = other._code;
	return (*this);
}
void	ErrorPage::SetPage() {
	ft::ResponseHeader	header(_code, ".html");
	_page << header.ToString();
	_title = strhttp(_code);
	_msg   = deschttp(_code);
	_page << \
	"<!DOCTYPE html>\n\
	<html>\n\
		<title>" + _title + "</title>\n\
		<body>\n\
			<div>\n\
				<H1>" + _title + "</H1>\n\
				<p>" + _msg + "<br><br><br></p>\n\
			</div>\n\
			<hr>\n\
			<p> abaur | WEBSERV | apitoise<br></p>\n\
		</body>\n\
	</html>\n\
	";
	_strPage = _page.str();
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
