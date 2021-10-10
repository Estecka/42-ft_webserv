/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 19:07:13 by abaur             #+#    #+#             */
/*   Updated: 2021/10/10 15:25:37 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.hpp"

#include <iostream>
#include <sstream>

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

	bool	ExtractWord(const std::string& _src, std::string& outWord, std::string& outRemain) {
		std::string src = _src;
		size_t begin, end;

		for (begin=0; begin<src.length(); begin++)
			if (!std::isspace(src[begin]))
				break;

		for (end=begin; end<src.length(); end++)
			if (std::isspace(src[end]))
				break;

		outWord   = src.substr(begin, end-begin);
		outRemain = ft::trim(src.substr(end));

		return outWord != "";
	}

	bool	StartsWith(const std::string& haystack, const std::string& needle){
		if (haystack.length() < needle.length())
			return false;

		for (size_t i=0; i<needle.length(); i++)
			if (haystack[i] != needle[i])
				return false;

		return true;
	}

	std::string	BitToString(const std::string bitstring) {
		std::stringstream str;

		for (size_t i=0; i<bitstring.length(); i++)
		{
			char c = bitstring[i];
			if (isprint(c))
				str << c;
			else
				str << '.';
		}

		return str.str();
	}

	 const char*	CharToDeci(unsigned char c){
		static unsigned char buff[5];
		buff[1] = '0' + (c / 100);
		buff[2] = '0' + ((c % 100) / 10);
		buff[3] = '0' + c % 10;

		int start;
		if (buff[1] != '0')
			start = 0;
		else if (buff[2] != '0')
			start = 1;
		else
			start = 2;

		buff[start] = '\\';
		buff[4] = '\0';
		return (const char*)buff + start;
	}

	static const char*	CharToString(char c){
		static char r[2];

		if (isprint(c)) {
			r[0] = c;
			r[1] = 0;
			return r;
		}

		switch (c)
		{
			default:  	return CharToDeci(c);
			case '\n':	return "\\n";
			case '\r':	return "\\r";
			case '\\':	return "\\\\";
			case '\"':	return "\\\"";
		}
	}

	std::string	BitToCString(const std::string& bitstring) {
		return BitToCString(bitstring.data(), bitstring.length());
	}
	std::string	BitToCString(const char* buff, size_t len) {
		std::stringstream str;
		str << '\"';

		for (size_t i=0; i<len; i++)
			str << CharToString(buff[i]);

		str << '\"';
		return str.str();
	}

	
}
