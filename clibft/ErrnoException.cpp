/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrnoException.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 17:03:53 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 17:14:43 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrnoException.hpp"

#include <cerrno>
#include <cstring>
#include <sstream>

namespace ft
{
	ErrnoException::ErrnoException() throw() : 
		std::runtime_error(what(errno, ""))
		{}

	ErrnoException::ErrnoException(int errnum) throw() :
		std::runtime_error(what(errnum, ""))
		{}

	ErrnoException::ErrnoException(int errnum, const std::string& context) throw() :
		std::runtime_error(what(errnum, context))
		{}

	ErrnoException::ErrnoException(const std::string& context) throw() :
		std::runtime_error(what(errno, context))
		{}

	ErrnoException::~ErrnoException() throw()
		{}


	std::string	ErrnoException::what(int errnum, const std::string& context) throw() {
		std::stringstream r;
		if (!context.empty())
			r << context << ' ';
		r << errnum << ' ' << std::strerror(errnum);

		return r.str();
	}
}
