/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeoutManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 17:54:38 by abaur             #+#    #+#             */
/*   Updated: 2021/10/25 17:52:00 by abaur            ###   ########.fr       */
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
		 * @param listener	The object to be registered.
		 * Listeners are automatically unregistered after an event has been sent.
		 * @param timeout	The duration in second after which the listener 
		 * wishes to receive an event.
		 * Events are very likely to be received several seconds late. This 
		 * should not be relied upon as a precision tool.
		 */
		static void	AddListener(RequestHandler& listener, unsigned int timeout);
		static void	RemoveListener(RequestHandler&, bool warn=true);

		/**
		 * Performs one round of timeout checks.
		 * @return	true if at least one event was sent.
		 */
		static bool	TimeLoop();

	private:
		typedef std::map<RequestHandler*, std::time_t>	ListenerMap;
		static ListenerMap	_listeners;
	};
}

#endif
