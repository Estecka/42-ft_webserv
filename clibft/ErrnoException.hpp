/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrnoException.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:57:13 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 17:10:35 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRNOEXCEPTION_HPP
#define ERRNOEXCEPTION_HPP

#include <stdexcept>

namespace ft
{
	class ErrnoException : public std::runtime_error
	{
	public:
		ErrnoException() throw();
		ErrnoException(int errnum) throw();
		ErrnoException(int errnum, const std::string& context) throw();
		ErrnoException(const std::string& context) throw();
		~ErrnoException() throw();
	private:
		static std::string	what(int errnum, const std::string& context) throw();
	};
}

#endif
