/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:04:38 by abaur             #+#    #+#             */
/*   Updated: 2021/08/16 19:05:05 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <sys/socket.h>

namespace ft
{
	class server
	{
	public:
		server(int port=8080);
		~server();

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
