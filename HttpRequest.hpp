/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 16:41:49 by abaur             #+#    #+#             */
/*   Updated: 2021/08/20 21:18:41 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <map>

namespace ft { class HttpRequest; }
std::ostream&	operator<<(std::ostream&, const ft::HttpRequest&);

namespace ft
{
	class HttpRequest
	{
	public:
		HttpRequest(const std::string&  requestContent);
		HttpRequest(std::istream& requestInput);
		~HttpRequest();

		bool	IsOk() const;

		const std::string&	operator[](const std::string& propertyName);
		bool	HasProperty(const std::string& propertyName) const;

		friend	std::ostream&	::operator<<(std::ostream&, const HttpRequest&);

	private:
		bool	_ok;
		std::string	_method;
		std::string	_requestPath;
		short	_majorHttpVersion;
		short	_minorHttpVersion;
		std::map<std::string, std::string>	_properties;

		bool	ParseFirstLine(std::istream&);
		bool	ParseProperty (std::istream&);
		/**
		 * Extract the next word from the a string, discarding any whitespace.
		 * The source string is modified to no longer contain that word.
		 * @param line	The source string.
		 * @param output	The destination to store the word.
		 * @return True if a word was found, or False if none were left, in which case the source is not modified.
		 */
		static bool	ExtractWord (std::string& line, std::string& output);

		bool	ValidateMethod () const;
		bool	ValidatePath   () const;
		static bool	ValidateVersionFull (const std::string& version);
		static bool	ValidatePropertyName(const std::string& name);
	};
}

#endif
