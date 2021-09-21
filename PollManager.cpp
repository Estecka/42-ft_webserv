/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:24:17 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 10:49:02 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"

#include <stdexcept>

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

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
			struct pollfd& pfd = _pollfds.back();
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
					AcceptSocket(pfd.fd);
			}

		}
	}

	void	PollManager::AcceptSocket(int sockfd) {
		ft::Socket& sock = *_sockets[sockfd];
		int acceptfd = sock.Accept();
		if (acceptfd < 0) {
			std::cerr << "[ERR] Socket acception failed on port " << sock.GetPort() << ": "
			          << errno << ' ' << std::strerror(errno) << std::endl;
			return;
		}

		// If polling accpetfd turns out to be required, 
		// req will need to outlive the scope of this function,
		// and thus allocated on the heap instead of the stack.
		HttpRequest req(acceptfd);
		std::cout << "\n" << req << std::endl;
		bool	serverfound = false;
		if (!req.IsOk()) {
			std::cerr << "[WARN] Invalid request received on port " << sock.GetPort() << std::endl;
			for (size_t i = 0; i < _servers.size(); i++) {
				if (_servers[i]->MatchRequest(req)) {
					_servers[i]->Accept(acceptfd, req);
					serverfound = true;
					break ;
				}
			}
			// Need to return an code 400 to the client here.
			// ...
		}
		else for (size_t i=0; i<_servers.size(); i++) {
			if (_servers[i]->MatchRequest(req)) {
				_servers[i]->Accept(acceptfd, req);
				serverfound = true;
				break;
			}
		}

		if (!serverfound) {
			std::cerr << "[ERR] No server found to answer request at: " << req.GetHost() << std::endl;
		}

		close(acceptfd);
	}

}
