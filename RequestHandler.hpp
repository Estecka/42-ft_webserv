/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:01:07 by abaur             #+#    #+#             */
/*   Updated: 2021/10/22 14:36:51 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "IPollListener.hpp"
#include "clibft/fdstream.hpp"
#include "RequestHeader.hpp"
#include "ResponseHeader.hpp"
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

		int                 	GetPort() const;
		const std::string&  	GetClientIp() const;
		const RequestHeader*	GetReqHead() const;
		const FILE*         	GetReqBody() const;
		const UriConfig&    	GetConfig() const;

		void	OnHeaderExtracted(RequestHeader*);
		void	OnBodyExtracted(FILE*);
		void	SendErrCode(int code);
		void	Destroy();

	private:
		pollfd        	_pollfd;
		IPollListener*	_subPollListener;

		int        	_port;
		std::string	_clientIP;
		UriConfig  	_config;
		RequestHeader*	_header;
		std::FILE*   	_body;

		void	PollInit();
		void	DispatchRequest();
	};
	
}

#endif
