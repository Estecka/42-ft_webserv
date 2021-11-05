/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 17:18:46 by abaur             #+#    #+#             */
/*   Updated: 2021/11/05 16:35:51 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpCode.hpp"

struct httpbinome {
	int	code;
	const char* msg;
};

#define CODECOUNT	(sizeof(http_str)/sizeof(httpbinome))
static const httpbinome	http_str[] = {
	{ HTTP_OK,               	"OK"                         },
	{ HTTP_CREATED,          	"Created"                    },
	{ HTTP_ACCEPTED,         	"Accepted"                   },
	{ HTTP_NO_CONTENT,       	"No Content"                 },
	{ HTTP_MOVED_PERMANENTLY,	"Moved Permanently"          },
	{ HTTP_FOUND,            	"Found"                      },
	{ HTTP_SEE_OTHER,        	"See Other"                  },
	{ HTTP_BAD_REQUEST,      	"Bad Request"                },
	{ HTTP_UNAUTHORIZED,     	"Unauthorized"               },
	{ HTTP_FORBIDDEN,        	"Forbidden"                  },
	{ HTTP_NOT_FOUND,        	"Not Found"                  },
	{ HTTP_NOT_ALLOWED,      	"Not Allowed"                },
	{ HTTP_NOT_ACCEPTABLE,   	"Not Acceptable"             },
	{ HTTP_REQ_TIMEOUT,      	"Request Timeout"            },
	{ HTTP_GONE,             	"Gone"                       },
	{ HTTP_REQ_TOO_LARGE,    	"Request Entity Too Large"   },
	{ HTTP_UNSUPPORTED_MEDIA,	"Unsupported Media Type"     },
	{ HTTP_TEAPOT,           	"I'm a teapot"               },
	{ HTTP_UNPROCESSABLE,    	"Unprocessable Entity"       },
	{ HTTP_TOO_MANY_REQ,     	"Too Many Requests"          },
	{ HTTP_INTERNAL_ERROR,   	"Internal Server Error"      },
	{ HTTP_NOT_IMPLEMENTED,  	"Not Implemented"            },
	{ HTTP_UNAVAILABLE,      	"Service Unavailable"        },
	{ HTTP_V_UNSUPPORTED,    	"HTTP Version Not Supported" },
};

namespace ft
{
	const char*	strhttp(int httpno){
		for (unsigned int i=0; i<CODECOUNT; i++)
			if (httpno == http_str[i].code)
				return http_str[i].msg;

		return "";
	}
}
