/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:01:07 by abaur             #+#    #+#             */
/*   Updated: 2021/10/09 19:52:42 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "IPollListener.hpp"
#include "clibft/fdstream.hpp"
#include "HttpRequest.hpp"

#include <iostream>

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

		RequestHandler(int acceptFd);
		~RequestHandler();

		void	GetPollFd(pollfd&);
		void	OnPollEvent(const pollfd&);

	private:
		pollfd	_pollfd;
		void (RequestHandler::*_onPollEvent)(const pollfd&);

		HttpRequest*	_header;

		void	PollInit();
		void	ExtractRequestHeader(const pollfd&);
		void	ExtractRequestBody  (const pollfd&);
		void	DispatchRequest     (const pollfd&);
	};
	
}

#endif
