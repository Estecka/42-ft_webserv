/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerDispatchPollListener.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 18:05:41 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 18:52:58 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERDISPATCHPOLLLISTENER_HPP
#define SERVERDISPATCHPOLLLISTENER_HPP

#include "IPollListener.hpp"
#include "Server.hpp"

#include <list>

namespace ft
{
	/**
	 * Listener that awaits for an accepted fd to be writeable before handling 
	 * the http request.
	 * 
	 * After its job is done, this listener will delete the HttpRequest and 
	 * close the file descriptor.
	 */
	class ServerDispatchPollListener : public IPollListener
	{
	public:
		typedef std::list<ft::Server>	ServList; 
		static ServList*	_availableServers;

		ServerDispatchPollListener(int acceptfd, int port, const std::string& rawRequest);
		~ServerDispatchPollListener();

		void	GetPollFd(struct pollfd& outpfd);
		void	OnPollEvent(const struct pollfd& pfd);

	private:
		int	_acceptfd;
		int _port;
		std::string	_rawRequest;
	};

}


#endif
