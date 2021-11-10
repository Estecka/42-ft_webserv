/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrnoException.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 17:03:53 by abaur             #+#    #+#             */
/*   Updated: 2021/11/07 20:48:49 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrnoException.hpp"

#include <cerrno>
#include <cstring>
#include <sstream>

namespace ft
{
	ErrnoException::ErrnoException() throw() : 
		std::runtime_error(What(errno, ""))
		{}

	ErrnoException::ErrnoException(int errnum) throw() :
		std::runtime_error(What(errnum, ""))
		{}

	ErrnoException::ErrnoException(int errnum, const std::string& context) throw() :
		std::runtime_error(What(errnum, context))
		{}

	ErrnoException::ErrnoException(const std::string& context) throw() :
		std::runtime_error(What(errno, context))
		{}

	ErrnoException::~ErrnoException() throw()
		{}


	std::string	ErrnoException::What(int errnum, const std::string& context) throw() {
		std::stringstream r;
		if (!context.empty())
			r << context << ' ';
		r << errnum << ' ' << std::strerror(errnum);

		return r.str();
	}
}
