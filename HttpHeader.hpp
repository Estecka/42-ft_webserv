/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:22:25 by abaur             #+#    #+#             */
/*   Updated: 2021/10/09 13:56:57 by apitoise         ###   ########.fr       */
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
		HttpHeader(void);
		HttpHeader(int code);
		HttpHeader(int code, std::string extension);
		HttpHeader(const HttpHeader&);
		~HttpHeader();
		HttpHeader& operator=(const HttpHeader&);

		static void	SendErrCode(int code, int fd);

		void		Setcode(int code);
		void		SetContentType(std::string extension);
		void		SetLocation(std::string location);

		std::string	GetContentType(void);

		/**
		 * Writes the header to the given stream.
		 */
		std::ostream&	ToStream(std::ostream& dst) const;
		std::string  	ToString() const;
		void         	ToFd(int fd) const;

	private:
		int	_code;
		const char*	_contentType;
		std::string	_location;
	};
}

std::ostream&	operator<<(std::ostream& dst, const ft::HttpHeader& src);

#endif
