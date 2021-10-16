/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:24:17 by abaur             #+#    #+#             */
/*   Updated: 2021/10/16 16:57:54 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"

#include <stdexcept>

namespace ft
{
	bool	PollManager::_dirty;
	std::vector<struct pollfd> 	PollManager::_pollfds;
	std::vector<IPollListener*>	PollManager::_listeners;


	void	PollManager::AddListener(IPollListener& listener) {
		for (size_t i=0; i<_listeners.size(); i++)
		if (_listeners[i] == &listener) {
			std::cerr << "[ERR] Attempted to add a listener to the PollManagers,"
			             " but this listener was already registered." << std::endl;
		}

		_dirty = true;
		_listeners.push_back(&listener);
	}

	void	PollManager::RemoveListener(IPollListener& listener) {
		for (std::vector<IPollListener*>::iterator it=_listeners.begin(); it!=_listeners.end(); it++)
		if (*it == &listener) {
			_dirty = true;
			_listeners.erase(it);
			return;
		}
		std::cerr << "[ERR] Attempted to remove a IPollListener that wasn't "
		             "registered to the PollManager"  << std::endl;
	}

	void	PollManager::SetDirty() {
		_dirty = true;
	}

	void	PollManager::DeleteAll() {
		for (std::vector<IPollListener*>::iterator it=_listeners.begin(); it!=_listeners.end(); it++)
		if (*it != NULL) {
			delete *it;
			*it = NULL;
		}
		_listeners.clear();
	}

	void	PollManager::RecreatePollArray() {
		if (!_dirty)
			return;

		_pollfds.resize(_listeners.size());
		for (size_t i=0; i<_listeners.size(); i++)
			_listeners[i]->GetPollFd(_pollfds[i]);

		_dirty = false;
	}

	void	PollManager::PollLoop (int timeout)
	{
		RecreatePollArray();
		const std::vector<IPollListener*> listeners = _listeners;

		std::cerr << "[INFO] Awaiting Poll..." << std::endl;
		int r = poll(&_pollfds[0], _pollfds.size(), 1000*timeout);
		if (r < 0) {
		std::cerr << "[FATAL] Poll error: " << errno << ' ' << std::strerror(errno) << std::endl;
			abort();
		}
		else for (size_t i=0; i<_pollfds.size(); i++)
		if (_pollfds[i].revents) {
			std::cerr << "\n[INFO] Poll Event" << std::endl;
			try {
				listeners[i]->OnPollEvent(_pollfds[i]);
			} catch (const std::exception& e){
				std::cerr << "[ERR] Uncaught exception on a IPollListener. This listener will be evicted. \n"
				          << "      " << e.what()
				          << std::endl;
				RemoveListener(*listeners[i]);
				delete listeners[i];
			}
		}
	}

}
