/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:20:56 by abaur             #+#    #+#             */
/*   Updated: 2021/09/18 14:06:01 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "configparser/configparser.hpp"

namespace ft
{
	class Server
	{
	public:
		Server();
		~Server();

		void	SetConfig(const ServerConfig&);
		void	SetSocket(ft::Socket&);

		/**
		 * Reads a request from the socket and answers it.
		 * 
		 * TODO:
		 * In the futur the prototype of this function will need to change.
		 * Not all request from the socket may belong to the same server (based on server name), and so the HttpRequest parsing will need to be done outside of the server structure.
		 * For now, until server names are implemented, we'll assume all requests from a port goes to the same server.
		 */
		void	Accept() const;

	private:
		const ServerConfig*	_config;
		ft::Socket*	_socket;

		std::string	_root;
	};
}


#endif
