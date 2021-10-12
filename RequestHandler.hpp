/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:01:07 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 15:23:43 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "IPollListener.hpp"
#include "clibft/fdstream.hpp"
#include "HttpRequest.hpp"
#include "HttpHeader.hpp"
#include "Server.hpp"

#include <iostream>
#include <sstream>

namespace ft
{
	/**
	 * An object that will accompany a request during its lifetime.
	 * It should be created when a request is received,
	 * and destroyed when the request is completed.
	 */
	class RequestHandler : private IPollListener
	{
	public:
		ft::ifdstream	httpin;
		ft::ofdstream	httpout;


		RequestHandler(fd_ip ip_fd, int port);
		~RequestHandler();

		void	GetPollFd(pollfd&);
		void	OnPollEvent(const pollfd&);

	private:
		pollfd			_pollfd;
		void (RequestHandler::*_onPollEvent)(const pollfd&);

		int					_port;
		HttpRequest*		_header;
		std::string			_clientIP;
		int					_code;

		std::stringstream	_stringbuff;

		void	PollInit();
		void	SetErrorPage();
		void	ExtractRequestHeader(const pollfd&);
		void	ExtractRequestBody  (const pollfd&);
		void	CheckRequest		(const pollfd&);
		void	DispatchRequest     (const pollfd&);
		void	SendErrorPage		(const pollfd&);
	};
	
}

#endif
