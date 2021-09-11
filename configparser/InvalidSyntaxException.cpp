/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidSyntaxException.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:53:54 by abaur             #+#    #+#             */
/*   Updated: 2021/09/11 14:42:27 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InvalidSyntaxException.hpp"

namespace ft
{
	InvalidSyntaxException::InvalidSyntaxException(const InvalidSyntaxException&) throw() : logic_error(*this) {}
	InvalidSyntaxException::InvalidSyntaxException(const std::string& what) throw() : logic_error(what) {}
	InvalidSyntaxException::InvalidSyntaxException(const std::string& lead, char punc) throw() : logic_error("Unexpected syntax: \""+lead+"\" '"+punc+"'") {}
	InvalidSyntaxException::InvalidSyntaxException(const std::string& what, const std::string& lead, char punc) throw() : logic_error(what+" \""+lead+"\" '"+punc+"'") {}
	InvalidSyntaxException::~InvalidSyntaxException() throw() {}
}
