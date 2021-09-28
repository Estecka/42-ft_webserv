/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerDispatchPollListener.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 18:05:41 by abaur             #+#    #+#             */
/*   Updated: 2021/09/28 11:37:48 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERDISPATCHPOLLLISTENER_HPP
# define SERVERDISPATCHPOLLLISTENER_HPP

# include "PollManager.hpp"
# include "HttpHeader.hpp"
# include "includes/webserv.hpp"
# include "IPollListener.hpp"
# include "Server.hpp"
# include "HttpRequest.hpp"

namespace ft
{
	/**
	 * Listener that awaits for an accepted fd to be polled before reading its 
	 * request and dispatching it to the appropriate server.
	 * 
	 * Upon destruction, this listener will close the file descriptor.
	 */
	class ServerDispatchPollListener : public IPollListener
	{
	public:
		typedef std::list<ft::Server>	ServList; 
		static ServList*	_availableServers;

		ServerDispatchPollListener(int acceptfd, int port);
		~ServerDispatchPollListener();

		void	GetPollFd(struct pollfd& outpfd);
		void	OnPollEvent(const struct pollfd& pfd);

	private:
		int	_acceptfd;
		int	_port;
		bool	_requestReceived;
		HttpRequest*	_request;

		void	ReadRequest();
		void	DispatchRequest();
	};

}


#endif
