/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 17:07:12 by abaur             #+#    #+#             */
/*   Updated: 2021/11/05 16:34:24 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCODE_HPP
#define HTTPCODE_HPP

#define HTTP_OK               	200
#define HTTP_CREATED          	201
#define HTTP_ACCEPTED         	202
#define HTTP_NO_CONTENT       	204

#define HTTP_MOVED_PERMANENTLY	301
#define HTTP_FOUND            	302
#define HTTP_SEE_OTHER        	303

#define HTTP_BAD_REQUEST      	400
#define HTTP_UNAUTHORIZED     	401
#define HTTP_FORBIDDEN        	403
#define HTTP_NOT_FOUND        	404
#define HTTP_NOT_ALLOWED      	405
#define HTTP_NOT_ACCEPTABLE   	406
#define HTTP_REQ_TIMEOUT      	408
#define HTTP_GONE             	410
#define HTTP_REQ_TOO_LARGE    	413
#define HTTP_UNSUPPORTED_MEDIA	415
#define HTTP_TEAPOT           	418
#define HTTP_UNPROCESSABLE    	422
#define HTTP_TOO_MANY_REQ     	429

#define HTTP_INTERNAL_ERROR   	500
#define HTTP_NOT_IMPLEMENTED  	501
#define HTTP_UNAVAILABLE      	503
#define HTTP_V_UNSUPPORTED    	505

namespace ft
{
	const char* strhttp(int httpno);
}


#endif
