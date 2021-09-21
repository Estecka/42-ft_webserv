/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollListener.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:32:44 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 16:17:06 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETPOLLLISTENER_HPP
#define SOCKETPOLLLISTENER_HPP

#include "IPollListener.hpp"
#include "Socket.hpp"
#include "Server.hpp"

#include <list>

namespace ft
{
	class SocketPollListener : public IPollListener {
	public:
		typedef std::list<ft::Server>	ServList; 

		static ServList*	_availableServers;

		SocketPollListener(ft::Socket&);
		SocketPollListener(const SocketPollListener&);
		~SocketPollListener();

		void	GetPollFd(struct pollfd&);
		void	OnPollEvent(const struct pollfd&);
	private:
		ft::Socket&	_sock;
	};
	
}

#endif
