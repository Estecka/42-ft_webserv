/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 17:18:46 by abaur             #+#    #+#             */
/*   Updated: 2021/11/11 17:24:07 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpCode.hpp"


#define CODECOUNT	(sizeof(http_str)/sizeof(*http_str))
static const struct 
	{ int code;              	const char* msg;             	const char* desc; }	http_str[] = {
	{ HTTP_OK,               	"OK",                        	"" },
	{ HTTP_CREATED,          	"Created",                   	"This request has been fulfilled and the ressource has been created successfuly." },
	{ HTTP_ACCEPTED,         	"Accepted",                  	"This request was accepted." },
	{ HTTP_NO_CONTENT,       	"No Content",                	"This request is not returning any content." },
	{ HTTP_MOVED_PERMANENTLY,	"Moved Permanently",         	"This content has been moved permanently." },
	{ HTTP_FOUND,            	"Found",                     	"This content has been moved temporarily." },
	{ HTTP_SEE_OTHER,        	"See Other",                 	"This content is somewhere else." },
	{ HTTP_BAD_REQUEST,      	"Bad Request",               	"Bad Request." },
	{ HTTP_UNAUTHORIZED,     	"Unauthorized",              	"Authorization required." },
	{ HTTP_FORBIDDEN,        	"Forbidden",                 	"Request unauthorized due to invalid permissions or credentials." },
	{ HTTP_NOT_FOUND,        	"Not Found",                 	"Page not found." },
	{ HTTP_NOT_ALLOWED,      	"Not Allowed",               	"The requested method is not allowed." },
	{ HTTP_NOT_ACCEPTABLE,   	"Not Acceptable",            	"An appropriate representation of the requested resource could not be found on this server." },
	{ HTTP_REQ_TIMEOUT,      	"Request Timeout",           	"The request took to long to execute." },
	{ HTTP_GONE,             	"Gone",                      	"The requested ressource is no longer available and will not be available again." },
	{ HTTP_REQ_TOO_LARGE,    	"Request Entity Too Large",  	"Your client issued a request that was too large." },
	{ HTTP_UNSUPPORTED_MEDIA,	"Unsupported Media Type",    	"The file type of the request is unsupported." },
	{ HTTP_TEAPOT,           	"I'm a teapot",              	"Just a teapot." },
	{ HTTP_UNPROCESSABLE,    	"Unprocessable Entity",      	"Request with semantical errors." },
	{ HTTP_TOO_MANY_REQ,     	"Too Many Requests",         	"You have sent too many requests in a given amount of time." },
	{ HTTP_INTERNAL_ERROR,   	"Internal Server Error",     	"Server is not responding, but it is alive ! Try again." },
	{ HTTP_NOT_IMPLEMENTED,  	"Not Implemented",           	"The requested method is not implemented by the server." },
	{ HTTP_UNAVAILABLE,      	"Service Unavailable",       	"The server is temporarily busy, try again later." },
	{ HTTP_V_UNSUPPORTED,    	"HTTP Version Not Supported",	"HTTP Version not supported." },
};

namespace ft
{
	const char*	strhttp(int httpno){
		for (unsigned int i=0; i<CODECOUNT; i++)
			if (httpno == http_str[i].code)
				return http_str[i].msg;

		return "";
	}

	const char*	deschttp(int httpno){
		for (unsigned int i=0; i<CODECOUNT; i++)
			if (httpno == http_str[i].code)
				return http_str[i].desc;

		return "Unknown http error";
	}
}
