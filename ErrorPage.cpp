/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:16:21 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/08 10:44:21 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"

ErrorPage::ErrorPage(int code, int acceptfd): _code(code) {
	this->SetPage(code);
	this->SendPage(acceptfd);
}

ErrorPage::ErrorPage(const ErrorPage &other) { this->operator=(other); }

ErrorPage::~ErrorPage(void) {}

ErrorPage	&ErrorPage::operator=(const ErrorPage& other) {
	this->SetPage(other._code);
	return (*this);
}
void	ErrorPage::SetPage(int code) {
	switch (code) {
		default:	code = 500;

		case 204:	_title = "204 No Content"; _msg = "This request is not returning any content.";	break;
		case 301:	_title = "301 Moved Permanently"; _msg = "This content has been moved permanently.";	break;
		case 302:	_title = "302 Found"; _msg = "This content has been moved temporarily.";	break;
		case 303:	_title = "303 See Other"; _msg = "This content is somewhere else.";	break;
		case 400:	_title = "400 Bad Request"; _msg = "Bad Request.";	break;
		case 401:	_title = "401 Unauthorized"; _msg = "Authorization required.";	break;
		case 403:	_title = "403 Forbidden"; _msg = "Request unauthorized due to invalid permissions or credentials.";	break;
		case 404:	_title = "404 Not Found"; _msg = "Page not found.";	break;
		case 405:	_title = "405 Method Not Allowed"; _msg = "The requested methodis not allowed.";	break;
		case 406:	_title = "406 Not Acceptable"; _msg = "An appropriate representation of the requested resource could not be found on this server.";	break;
		case 410:	_title = "410 Gone"; _msg = "The requested ressource is no longer available and will not be available again.";	break;
		case 413:	_title = "413 Request Entity Too Large"; _msg = "Your client issued a request that was too large.";	break;
		case 415:	_title = "415 Unsupported Media Type"; _msg = "The file type of the request is unsupported.";	break;
		case 418:	_title = "418 I'm a teapot"; _msg = "Just a teapot.";	break;
		case 422:	_title = "422 Unprocessable Entity"; _msg = "Request with semantical errors.";	break;
		case 429:	_title = "429 Too Many Requests"; _msg = "You have sent too many requests in a given amount of time.";	break;
		case 500:	_title = "500 Internal Server Error"; _msg = "Server is not responding, but it is alive ! Try again.";	break;
		case 501:	_title = "501 Not Implemented"; _msg = "The requested method is not implemented by the server.";	break;
		case 503:	_title = "503 Service Unavailable"; _msg = "The server is temporarily busy, try again later.";	break;
		case 505:	_title = "505 HTTP Version Not Supported"; _msg = "HTTP Version not supported.";	break;	
	}
}

void	ErrorPage::SendPage(int acceptfd) {
	ft::HttpHeader	header(_code, ".html");
	_page = \
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
	send(acceptfd, header.ToString().c_str(), header.ToString().size(), 0);
	send(acceptfd, _page.c_str(), _page.size(), 0);
}
