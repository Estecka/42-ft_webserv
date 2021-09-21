/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestReadPollListener.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 17:08:38 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 18:16:04 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTREADPOLLLISTENER_HPP
#define REQUESTREADPOLLLISTENER_HPP

#include "IPollListener.hpp"
#include "Socket.hpp"

namespace ft
{
	/**
	 * Listeners that waits for an accepted fd to be readable before extracting 
	 * the http request from it. 
	 */
	class RequestReadPollListener : public IPollListener
	{
	public:
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
