/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:32:09 by abaur             #+#    #+#             */
/*   Updated: 2021/11/11 17:50:21 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpException.hpp"
#include "HttpCode.hpp"

namespace ft
{	
	HttpException::HttpException(int httpcode) throw() :
		std::runtime_error(deschttp(httpcode))
	{
		this->_httpcode = httpcode;
	}

	HttpException::HttpException(int httpcode, const std::string& context) throw() :
		std::runtime_error(context)
	{
		this->_httpcode = httpcode;
	}

	HttpException::~HttpException() throw() 
	{}
	
	int	HttpException::GetHttpCode() const throw() { return this->_httpcode; }


}
