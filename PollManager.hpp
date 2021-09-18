/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:07:56 by abaur             #+#    #+#             */
/*   Updated: 2021/09/18 17:01:58 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLMANAGER_HPP
#define POLLMANAGER_HPP

#include "Socket.hpp"
#include "Server.hpp"

#include <vector>
#include <map>

#include <poll.h>
#include <stdnoreturn.h>

namespace ft
{
	class PollManager
	{
	public:
		static void	AddSocket(ft::Socket&);
		static void	AddServer(ft::Server&);

		static noreturn void	PollLoop();


	private:
		typedef std::map<int, ft::Socket*>	SockMap;

		/**
		 * Wether the pollfd array needs to be reconstructed.
		 */
		static bool	_dirty;
		static std::vector<struct pollfd> _pollfds;

		static SockMap	_sockets;
		static std::vector<ft::Server*>	_servers;

		/**
		 * Regenerates _pollfds if dirty.
		 */
		static void	RecreatePollArray();
	};
}

#endif
