/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollListener.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:32:44 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 17:31:49 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETPOLLLISTENER_HPP
#define SOCKETPOLLLISTENER_HPP

#include "IPollListener.hpp"
#include "Socket.hpp"

namespace ft
{
	class SocketPollListener : public IPollListener {
	public:
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
