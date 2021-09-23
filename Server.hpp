/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:20:56 by abaur             #+#    #+#             */
/*   Updated: 2021/09/22 10:20:03 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "configparser/configparser.hpp"

#include <vector>

namespace ft
{
	class Server
	{
	public:
		Server();
		~Server();

		void	SetConfig(const ServerConfig&);

		/**
		 * Checks whether this server matches the request's hostname and port.
		 */
		bool	MatchRequest(const ft::HttpRequest&) const;

		/**
		 * Respond to a request. The request is assumed to belong to this server.
		 * @param acceptfd	The file descriptor associated with the request.
		 */
		void	Accept(int acceptfd, const ft::HttpRequest& req);

	private:
		const ServerConfig*	_config;

		std::vector<int>	_ports;
		std::string	_hostname;
		std::string	_root;
	
		bool	MatchPath(const HttpRequest& req) const;
		void	GetFileData(int acceptfd, const HttpRequest& req) const;
	};
}


#endif
