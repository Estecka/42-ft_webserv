/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeoutManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 18:04:53 by abaur             #+#    #+#             */
/*   Updated: 2021/10/15 18:35:58 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TimeoutManager.hpp"

namespace ft
{
	static clock_t	GetWakeTime(unsigned int timeout_seconds){
		return std::clock() + (CLOCKS_PER_SEC * timeout_seconds);
	}


	void	TimeoutManager::AddListener(RequestHandler& listener, unsigned int timeout){
		if (_listeners.count(&listener))
			std::clog << "[WARN] Listener already registered to the timeout manager,"
			             "it will be replaced." << std::endl;

		_listeners[&listener] = GetWakeTime(timeout);
	}

	void	TimeoutManager::RemoveListener(RequestHandler& listener){
		if (!_listeners.erase(&listener))
			std::clog << "[ERR] Attempted to remove a listener which was not "
			             "registered to the timeout manager." << std::endl;
	}

	void	TimeoutManager::TimeLoop(){
		const ListenerMap	listeners(_listeners);

		for (ListenerMap::const_iterator it=listeners.begin(); it!=listeners.end(); it++)
		{
			RequestHandler& ls = *it->first;
			if (it->second < std::clock()) {
				ls.OnTimeout();
				TimeoutManager::RemoveListener(ls);
			}
		}
	}
}
