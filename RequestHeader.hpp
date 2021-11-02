/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHeader.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 16:41:49 by abaur             #+#    #+#             */
/*   Updated: 2021/10/31 20:10:26 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include "clibft/clibft.hpp"
#include <map>

namespace ft { class RequestHeader; }
std::ostream&	operator<<(std::ostream&, const ft::RequestHeader&);

namespace ft
{
	class RequestHeader
	{
	public:
		RequestHeader(const std::string&  requestContent);
		RequestHeader(std::istream& requestInput);
		~RequestHeader();

		bool 	IsOk() const;
		short	GetMajorHttpVersion() const;
		short	GetMinorHttpVersion() const;
		const std::string&	GetMethod() const;
		const std::string&	GetRequestPath() const;
		const std::string&	GetQueryString() const;
		const std::string&	GetHost() const;
		const std::string&	GetHostname() const;
		int               	GetHostPort() const;

		size_t	GetContentLength() const;
		bool  	IsChunked() const;

		const std::string&	operator[](const std::string& propertyName);
		std::string       	operator[](const std::string& propertyName) const;
		bool	HasProperty(const std::string& propertyName) const;

		friend	std::ostream&	::operator<<(std::ostream&, const RequestHeader&);

	private:
		bool	_ok;
		std::string	_method;
		std::string	_requestPath;
		std::string	_queryString;
		short	_majorHttpVersion;
		short	_minorHttpVersion;
		std::string	_hostname;
		int        	_port;

		size_t	_bodyLength;
		bool  	_isChuncked;

		std::map<std::string, std::string>	_properties;

		RequestHeader(void);
		/**
		 * Entry point for parsing a raw header.
		 */
		bool	Parse(std::istream&);
		/**
		 * Extract and validate the very first line of the header.
		 */
		bool	ParseFirstLine(std::istream&);
		/**
		 * Extract a line from the stream and separate property name from value.
		 * @return True if the next line belongs to the request header, regardless of its validity.
		 *         False if end of the request header was found.
		 */
		bool	ParsePropertyLine (std::istream&);

		/**
		 * Parses the value of an individual property.
		 * @return
		 * 	true 	The property is either valid or unknown.
		 * 	false	The property is invalid.
		 */
		bool	ParsePropertyValue(const std::string& name, const std::string& value);


		bool	ValidateMethod	() const;
		bool	ValidatePath	() const;
		static bool	ValidateVersionFull (const std::string& version);
		static bool	ValidatePropertyName(const std::string& name);
	};
}

#endif
