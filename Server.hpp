/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:04:38 by abaur             #+#    #+#             */
/*   Updated: 2021/08/20 16:53:40 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>

namespace ft
{
	class Server
	{
	public:
		Server(int port=8080);
		~Server();

		/**
		 * @return Some flavor of errno.
		 */
		int	GetErrStatus() const;

		/**
		 * @return Treats one request from a client if there is one.
		 */
		void	Accept();

	private:
		/**
		 * Binds the server to its port, making it ready to listen.
		 * @return 0 on success. On error, some flavor of errno.
		 */
		int	Init();

	private:
		int	errstatus;
		int	sockfd;
		struct sockaddr_in addr;
	};
}

#endif
