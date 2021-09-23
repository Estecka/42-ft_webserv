/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleHttpResponse.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:22:25 by abaur             #+#    #+#             */
/*   Updated: 2021/09/23 15:56:24 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLEHTTPRESPONSE
#define SIMPLEHTTPRESPONSE

#include <string>
#include <sstream>

namespace ft
{
	class SimpleHttpResponse
	{
	public:
		SimpleHttpResponse(int code = 204, std::string extension = ".txt", const std::string& body = "");
		SimpleHttpResponse(const SimpleHttpResponse&);
		~SimpleHttpResponse();
		SimpleHttpResponse& operator=(const SimpleHttpResponse&);


		void	Setcode(int code);
		void	SetContentType(std::string extension);
		/**
		 * @return	A stream that can be used to add content to the response body.
		 */
		std::ostream&	GetStream();

		/**
		 * Writes the entire response (header and body) to the given stream.
		 */
		std::ostream&	ToStream(std::ostream& dst) const;
		std::string  	ToString() const;
		void         	ToFd(int fd) const;

	private:
		int	_code;
		const char*	_codeMsg;
		const char*	_contentType;
		std::stringstream	_body;
	};
}

std::ostream&	operator<<(std::ostream& dst, const ft::SimpleHttpResponse& src);

#endif
