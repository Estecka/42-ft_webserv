/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeoutManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 17:54:38 by abaur             #+#    #+#             */
/*   Updated: 2021/10/15 18:29:13 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMEOUTMANAGER
#define TIMEOUTMANAGER

#include "RequestHandler.hpp"

#include <map>
#include <ctime>

namespace ft
{
	class TimeoutManager
	{
	public:
		/**
		 * @param timeout	The duration in second after which the listener 
		 * wishes to receive an event.
		 */
		static void	AddListener(RequestHandler&, unsigned int timeout);
		static void	RemoveListener(RequestHandler&);

		/**
		 * Performs one round of timeout checks.
		 */
		static void	TimeLoop();

	private:
		typedef std::map<RequestHandler*, std::clock_t>	ListenerMap;
		static ListenerMap	_listeners;
	};
}

#endif
