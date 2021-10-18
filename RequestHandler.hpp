/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:01:07 by abaur             #+#    #+#             */
/*   Updated: 2021/10/16 16:15:50 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "IPollListener.hpp"
#include "clibft/fdstream.hpp"
#include "HttpRequest.hpp"
#include "HttpHeader.hpp"
#include "configparser/UriConfig.hpp"
#include <iostream>
#include <sstream>
#include <cstdio>

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
		void	OnTimeout();
		void	SetPollEvent(IPollListener*);

		void	OnHeaderExtracted(HttpRequest*);
		void	OnBodyExtracted(FILE*);
		void	Destroy();

		const HttpRequest*	GetReqHead() const;

	private:
		pollfd			_pollfd;
		IPollListener*	_subPollListener;
		void (RequestHandler::*_onPollEvent)(const pollfd&);

		int         	_port;
		std::string 	_clientIP;
		HttpRequest*	_header;
		std::FILE*  	_body;
		int         	_code;
		UriConfig   	_config;

		void	SetPollEvent(int fd, short event, void (RequestHandler::*function)(const pollfd&));

		void	PollInit();
		void	SetErrorPage();
		void	ExtractRequestBody  (const pollfd&);
		void	CheckRequest		(const pollfd&);
		void	DispatchRequest     (const pollfd&);
	};
	
}

#endif
