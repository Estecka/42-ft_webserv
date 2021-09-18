/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clibft.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 19:07:13 by abaur             #+#    #+#             */
/*   Updated: 2021/09/18 14:56:53 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clibft.hpp"

namespace ft
{
	std::string	trim(const std::string& str){
		size_t begin = 0;
		size_t end   = str.length();

		for(; begin<end; begin++)
			if (!std::isspace(str[begin]))
				break;

		for(; begin<end; end--)
			if (!std::isspace(str[end-1]))
				break;

		return str.substr(begin, end-begin);
	}
}
