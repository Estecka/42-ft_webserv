/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidSyntaxException.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:50:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/06 15:03:49 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVALIDSYNTAXEXCEPTION_HPP
#define INVALIDSYNTAXEXCEPTION_HPP

#include <stdexcept>

namespace ft
{
	class InvalidSyntaxException : public std::logic_error
	{
	public:
		InvalidSyntaxException() throw();
		InvalidSyntaxException(const InvalidSyntaxException&) throw();
		InvalidSyntaxException(const std::string& what) throw();
		InvalidSyntaxException(const std::string& lead, char punc) throw();
		~InvalidSyntaxException() throw();
	};
}

#endif
