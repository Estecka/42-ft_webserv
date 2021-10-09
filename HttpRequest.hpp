/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 16:41:49 by abaur             #+#    #+#             */
/*   Updated: 2021/10/09 15:06:16 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "includes/webserv.hpp"
# include "clibft/clibft.hpp"

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

		bool 	IsOk() const;
		short	GetMajorHttpVersion() const;
		short	GetMinorHttpVersion() const;
		const std::string&	GetMethod() const;
		const std::string&	GetRequestPath() const;
		const std::string&	GetQueryString() const;
		const std::string&	GetHost() const;
		const std::string&	GetHostname() const;
		int               	GetHostPort() const;

		const std::string&	operator[](const std::string& propertyName);
		bool	HasProperty(const std::string& propertyName) const;

		friend	std::ostream&	::operator<<(std::ostream&, const HttpRequest&);

	private:
		bool	_ok;
		std::string	_method;
		std::string	_requestPath;
		std::string	_queryString;
		short	_majorHttpVersion;
		short	_minorHttpVersion;
		std::string	_hostname;
		int        	_port;
		std::map<std::string, std::string>	_properties;

		std::stringstream	_body;

		bool	Parse(std::istream&);
		bool	ParseFirstLine(std::istream&);
		/**
		 * @return True if the next line belongs to the request header, regardless of its validity.
		 *         False if end of the request header was found.
		 */
		bool	ParseProperty (std::istream&);
		/**
		 * Extract the next word from the a string, discarding any whitespace.
		 * The source string is modified to no longer contain that word.
		 * @param line	The source string.
		 * @param output	The destination to store the word.
		 * @return True if a word was found, or False if none were left, in which case the source is not modified.
		 */
		static bool	ExtractWord (std::string& line, std::string& output);

		bool	ValidateMethod	() const;
		bool	ValidatePath	() const;
		static bool	ValidateVersionFull (const std::string& version);
		static bool	ValidatePropertyName(const std::string& name);
		/**
		 * @return The index of the ':' separating host name from port, or 0 if the host is invalid.
		 */
		static int	ValidateHostFull(const std::string& host);
	};
}

#endif
