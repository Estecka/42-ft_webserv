/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestReadPollListener.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 17:08:38 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 17:35:18 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTREADPOLLLISTENER_HPP
#define REQUESTREADPOLLLISTENER_HPP

#include "IPollListener.hpp"
#include "Socket.hpp"
#include "Server.hpp"

#include <list>

namespace ft
{
	class RequestReadPollListener : public IPollListener
	{
	public:
		typedef std::list<ft::Server>	ServList; 

		static ServList*	_availableServers;

		RequestReadPollListener(int acceptfd, int port);
		~RequestReadPollListener();

		void	GetPollFd(struct pollfd& outpfd);
		void	OnPollEvent(const struct pollfd& pfd);

	private:
		int	acceptfd;
		int	port;
	};

}


#endif
