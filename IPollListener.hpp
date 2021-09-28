/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPollListener.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:19:32 by abaur             #+#    #+#             */
/*   Updated: 2021/09/28 11:33:13 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPOLLLISTENER_HPP
#define IPOLLLISTENER_HPP

#include "includes/webserv.hpp"

namespace ft
{
	/**
	 * Base interface for anything that needs to wait for a fd to be polled.
	 */
	class IPollListener
	{
	public:
		virtual ~IPollListener() {};

		/**
		 * @param outpfd	Outputs a polling request for the fd and event this 
		 * listener whishes to wait onto.
		 */
		virtual void	GetPollFd(struct pollfd& outpfd) = 0;

		/**
		 * Function that will be called anytime the polled fd receives an event.
		 * @param pfd	The poll request representing the fd this listener has subscribed to.
		 */
		virtual void	OnPollEvent(const struct pollfd& pfd) = 0;
	};

}


#endif
