/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:24:17 by abaur             #+#    #+#             */
/*   Updated: 2021/09/18 17:04:55 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"

#include <stdexcept>

#include <cerrno>
#include <cstring>
#include <stdlib.h>

namespace ft
{
	bool PollManager::_dirty;
	std::vector<struct pollfd> PollManager::_pollfds;
	PollManager::SockMap PollManager::_sockets;
	std::vector<ft::Server*> PollManager::_servers;


	void	PollManager::AddSocket(ft::Socket& sock) {
		if (!sock.IsBound()) {
			std::cerr << "[ERR] Tried adding unbound socket to the poll manager, "
			             "this socket will not be added." << std::endl;
		}
		else {
			_sockets[sock.GetSocketFD()] = &sock;
			_dirty = true;
		}
	}

	void	PollManager::AddServer(ft::Server& serv) {
		_servers.push_back(&serv);
	}

	void	PollManager::RecreatePollArray() {
		if (!_dirty)
			return;

		_pollfds.clear();
		for (SockMap::const_iterator it=_sockets.begin(); it!=_sockets.end(); it++) {
			_pollfds.resize(_pollfds.size()+1);
			struct pollfd& pfd = _pollfds[_pollfds.size()-1];

			pfd.fd = it->first;
			pfd.events = POLLIN;
		}
		_dirty = false;
	}

	noreturn void	PollManager::PollLoop () {
		while (true) 
		{
			RecreatePollArray();
			int r = poll(&_pollfds[0], _pollfds.size(), -1);
			if (r < 0) {
				std::cerr << "[FATAL] Poll error: " << errno << ' ' << std::strerror(errno) << std::endl;
				abort();
			}
			else for (size_t i=0; i<_pollfds.size(); i++) {
				struct pollfd& pfd = _pollfds[i];
				if (pfd.revents & POLLIN)
				// This loop will be changed when implementing servernames.
				for (size_t j=0; j<_servers.size(); j++)
				if (pfd.fd == _servers[j]->GetSockFd()) {
					_servers[j]->Accept();
					break;
				}
			}

		}
	}

}
