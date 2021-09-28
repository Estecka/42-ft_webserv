/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:22:25 by abaur             #+#    #+#             */
/*   Updated: 2021/09/28 11:15:40 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADER_HPP
# define HTTPHEADER_HPP

# include "includes/webserv.hpp"

namespace ft
{
	class HttpHeader
	{
	public:
		HttpHeader(int code = 204, std::string extension = ".txt");
		HttpHeader(const HttpHeader&);
		~HttpHeader();
		HttpHeader& operator=(const HttpHeader&);

		static void	SendErrCode(int code, int fd);

		void	Setcode(int code);
		void	SetContentType(std::string extension);

		/**
		 * Writes the header to the given stream.
		 */
		std::ostream&	ToStream(std::ostream& dst) const;
		std::string  	ToString() const;
		void         	ToFd(int fd) const;

	private:
		int	_code;
		const char*	_codeMsg;
		const char*	_contentType;
	};
}

std::ostream&	operator<<(std::ostream& dst, const ft::HttpHeader& src);

#endif
