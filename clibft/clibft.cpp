/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clibft.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 19:07:13 by abaur             #+#    #+#             */
/*   Updated: 2021/09/11 15:04:57 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clibft.hpp"

namespace ft
{
	std::string	trim(const std::string& str){
		size_t begin = 0;
		size_t end   = str.length();

		for(; begin<end; begin++)
			if (!isspace(str[begin]))
				break;

		for(; begin<end; end--)
			if (!isspace(str[end-1]))
				break;

		return str.substr(begin, end-begin);
	}
}
