/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clibft.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 19:07:13 by abaur             #+#    #+#             */
/*   Updated: 2021/09/28 15:31:09 by abaur            ###   ########.fr       */
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

	void	ExtractWord(const std::string& src, std::string& outWord, std::string& outRemain) {
		size_t begin, end;

		for (begin=0; begin<src.length(); begin++)
			if (!std::isspace(src[begin]))
				break;

		for (end=begin; end<src.length(); end++)
			if (std::isspace(src[end]))
				break;

		outWord   = src.substr(begin, end-begin);
		outRemain = ft::trim(src.substr(end));
	}
}
