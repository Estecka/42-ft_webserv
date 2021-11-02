/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:24:17 by abaur             #+#    #+#             */
/*   Updated: 2021/10/31 20:17:23 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"

#include "logutil/logutil.hpp"

#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <cstdlib>

namespace ft
{
	bool	PollManager::_dirty;
	std::vector<struct pollfd> 	PollManager::_pollfds;
	std::vector<IPollListener*>	PollManager::_listeners;


	void	PollManager::AddListener(IPollListener& listener) {
		for (size_t i=0; i<_listeners.size(); i++)
		if (_listeners[i] == &listener) {
			ft::clog << log::error << "Attempted to add a listener to the PollManagers,"
			             " but this listener was already registered." << std::endl;
		}

		_dirty = true;
		_listeners.push_back(&listener);
	}

	void	PollManager::RemoveListener(IPollListener& listener, bool warn) {
		for (std::vector<IPollListener*>::iterator it=_listeners.begin(); it!=_listeners.end(); it++)
		if (*it == &listener) {
			_dirty = true;
			_listeners.erase(it);
			return;
		}
		if (warn)
			ft::clog << log::warning << "Attempted to remove a listener that "
			            "wasn't registered to the PollManager"  << std::endl;
	}

	void	PollManager::SetDirty() {
		_dirty = true;
	}

	void	PollManager::DeleteAll() {
		const ListenerArray lstnr(_listeners);
		for (ListenerArray::const_iterator it=lstnr.begin(); it!=lstnr.end(); it++) {
			if (*it != NULL) {
				// ft::clog << log::debug << "About to delete: " << *it << std::endl;
				PollManager::RemoveListener(**it);
				delete *it;
				// ft::clog << log::debug << "Deleted" << std::endl;
			} else {
				ft::clog << log::error << "NULL pointer found astray in the PollManager." << std::endl;
			}
		}

		if (_listeners.size() > 0){
			ft::clog << log::error << "New PollListeners registered themeselves during cleanup process" << std::endl;
		}
	}

	void	PollManager::RecreatePollArray() {
		if (!_dirty)
			return;

		_pollfds.resize(_listeners.size());
		for (size_t i=0; i<_listeners.size(); i++)
			_listeners[i]->GetPollFd(_pollfds[i]);

		_dirty = false;
	}

	bool	PollManager::PollLoop (int timeout)
	{
		RecreatePollArray();
		const ListenerArray listeners = _listeners;
		bool r = false;

		int err = poll(&_pollfds[0], _pollfds.size(), 1000*timeout);
		if (err < 0) {
		ft::clog << log::fatal << "Poll error: " << errno << ' ' << std::strerror(errno) << std::endl;
			abort();
		}
		else for (size_t i=0; i<_pollfds.size(); i++)
		if (_pollfds[i].revents) {
			ft::clog << log::none << std::endl
			         << log::event << "Poll Event" << std::endl;
			r = true;
			try {
				listeners[i]->OnPollEvent(_pollfds[i]);
			} catch (const std::exception& e){
				ft::clog << log::error << "Uncaught exception on a IPollListener. This listener will be evicted. \n"
				          << e.what()
				          << std::endl;
				RemoveListener(*listeners[i]);
				delete listeners[i];
			}
		}
		return r;
	}

}
