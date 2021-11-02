/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:26:56 by abaur             #+#    #+#             */
/*   Updated: 2021/10/22 15:41:34 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

#include <stdexcept>

namespace ft
{
	/**
	 * An exception that can be described as an http code and be conveyed to a client.
	 */
	class HttpException : public std::runtime_error
	{
	public:
		HttpException(int httpcode) throw();
		HttpException(int httpcode, const std::string& context) throw();
		virtual ~HttpException() throw();

		int	GetHttpCode() const throw();

	private:
		int	_httpcode;
	};
}

#endif
