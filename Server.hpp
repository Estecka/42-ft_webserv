/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:20:56 by abaur             #+#    #+#             */
/*   Updated: 2021/10/10 14:10:09 by abaur            ###   ########.fr       */
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
		void	Accept(int acceptfd, const ft::HttpRequest& req);

	private:
		const ServerConfig&	_config;

		/*
		** These variables could be disused.
		** They reference variable directly in _config.
		*/
		const std::vector<int>&	_ports;
		const std::string&	_hostname;

		void	Delete(int acceptfd, std::string reqPath, const UriConfig& conf) const;
		void	Get(int acceptfd, const HttpRequest& req, std::string reqPath, const UriConfig& conf) const;
		void	Redirection(int acceptfd, const UriConfig& conf) const;
		bool	IsDir(const std::string path) const;
		bool	MatchPath(const std::string reqPath, const UriConfig& conf) const;
		void	GetFileData(int acceptfd, std::string reqPath, const UriConfig& conf) const;
		void	GetIndex(int acceptfd, const HttpRequest& req, const UriConfig& conf, std::string reqPath) const;
		void	AutoIndex(const HttpRequest& req, int acceptfd, std::string path) const;

	};
}


#endif
