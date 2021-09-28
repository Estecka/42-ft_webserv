/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:20:56 by abaur             #+#    #+#             */
/*   Updated: 2021/09/28 15:31:20 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "includes/webserv.hpp"
# include "HttpRequest.hpp"
# include "HttpHeader.hpp"
# include "Socket.hpp"
# include "configparser/configparser.hpp"
# include "configparser/UriConfig.hpp"
# include "AutoIndex.hpp"

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
	
		bool	IsDir(const std::string path) const;
		bool	MatchPath(const HttpRequest& req, const UriConfig uri) const;
		void	GetFileData(int acceptfd, std::string reqPath, const UriConfig uri) const;
		void	GetIndex(int acceptfd, const HttpRequest& req, const UriConfig uri) const;
	};
}


#endif
