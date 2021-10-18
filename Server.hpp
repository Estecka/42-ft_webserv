/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:20:56 by abaur             #+#    #+#             */
/*   Updated: 2021/10/18 14:44:41 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "includes/webserv.hpp"
# include "HttpRequest.hpp"
# include "ResponseHeader.hpp"
# include "Socket.hpp"
# include "configparser/configparser.hpp"
# include "configparser/UriConfig.hpp"
# include "ErrorPage.hpp"

namespace ft
{
	class Server
	{
	public:
		static std::list<Server>	availableServers;

		Server(const ServerConfig&);
		~Server();

		/**
		 * Checks whether this server matches the request's hostname and port.
		 */
		bool	MatchRequest(const ft::HttpRequest&) const;

		/**
		 * Respond to a request. The request is assumed to belong to this server.
		 * @param acceptfd	The file descriptor associated with the request.
		 */
		UriConfig	Accept(const ft::HttpRequest& req);

	private:
		const ServerConfig&	_config;

		/*
		** These variables could be disused.
		** They reference variable directly in _config.
		*/
		const std::vector<int>&	_ports;
		const std::string&	_hostname;


	};
}


#endif
