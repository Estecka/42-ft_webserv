/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidSyntaxException.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:53:54 by abaur             #+#    #+#             */
/*   Updated: 2021/08/25 19:10:30 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InvalidSyntaxException.hpp"

namespace ft
{
	InvalidSyntaxException::InvalidSyntaxException(const InvalidSyntaxException&) : logic_error(*this) {}
	InvalidSyntaxException::InvalidSyntaxException(const std::string& what) : logic_error(what) {}
	InvalidSyntaxException::InvalidSyntaxException(const std::string& lead, char punc) : logic_error("Unexpected syntax: "+lead+punc) {}
	InvalidSyntaxException::~InvalidSyntaxException(){}
}
