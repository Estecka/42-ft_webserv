/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeoutManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 18:04:53 by abaur             #+#    #+#             */
/*   Updated: 2021/10/23 16:12:06 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TimeoutManager.hpp"

#include "logutil/logger.hpp"
#include <stdexcept>

namespace ft
{
	TimeoutManager::ListenerMap TimeoutManager::_listeners;

	static time_t	GetWakeTime(unsigned int timeout_seconds){
		return std::time(NULL) + timeout_seconds;
	}


	void	TimeoutManager::AddListener(RequestHandler& listener, unsigned int timeout){
		if (_listeners.count(&listener))
			ft::clog << "[WARN] Listener already registered to the timeout manager,"
			             "it will be replaced." << std::endl;

		_listeners[&listener] = GetWakeTime(timeout);
	}

	void	TimeoutManager::RemoveListener(RequestHandler& listener){
		if (!_listeners.erase(&listener))
			ft::clog << "[ERR] Attempted to remove a listener which was not "
			             "registered to the timeout manager." << std::endl;
	}

	bool	TimeoutManager::TimeLoop(){
		const ListenerMap	listeners(_listeners);
		bool r = false;

		for (ListenerMap::const_iterator it=listeners.begin(); it!=listeners.end(); it++)
		{
			RequestHandler& ls = *it->first;
			if (it->second < std::time(NULL)) {
				r = true;
				ft::clog << "[INFO] Timeout Event" << std::endl;
				try {
					ls.OnTimeout();
				}
				catch (const std::exception& e){
					ft::clog << "[ERR] Uncaught exception during timeout event: \n"
					          << "      " << e.what()
					          << std::endl;
				}
				TimeoutManager::RemoveListener(ls);
			}
		}
		return r;
	}
}
