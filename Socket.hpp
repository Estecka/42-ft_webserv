/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:04:38 by abaur             #+#    #+#             */
/*   Updated: 2021/09/14 15:37:53 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>

namespace ft
{
	class Socket
	{
	public:
		/**
		 * Creates an uninitialized socket.
		 */
		Socket();
		/**
		 * Creates a socket on a given port, and immediately attempts to bind it.
		 */
		Socket(int port);

		~Socket();

		/**
		 * @return	True if the socket was succesfully bound
		 */
		bool	IsBound() const;

		/**
		 * @return	Some flavor of errno if the socket failed to bind. 0 If the socket is bound, or never attempted to bind.
		 */
		int	GetErrStatus() const;

		/**
		 * @return	The port this socket intends to listen to, or -1 if the socket was uninitialized.
		 */
		int	GetPort() const;

		/**
		 * Changes the intended port to listen. This only works if the socket isn't already bound.
		 * @throw	std::logic_error
		 */
		void	SetPort(int);

		/**
		 * @return The file descriptor representing the socket, or -1 if the socket failed to bind.
		 */
		int	GetSocketFD() const;


		/**
		 * Binds the socket to its port, making it ready to accetp requests.
		 * @return 0 on success. On error, some flavor of errno.
		 */
		int	Bind();

		/**
		 * Treats one request from a client if there is one.
		 * @return A fd through which to receive a request and send its response, or -1 if acception failed.
		 * 
		 * TODO: As of now, this method immediately sends a default http response and closes the returned fd.
		 * Such behaviour should be scrapped in the future.
		 */
		int	Accept();

	private:
		int	port;
		bool	isBound;
		int	errstatus;
		int	sockfd;
		struct sockaddr_in addr;
	};
}

#endif
