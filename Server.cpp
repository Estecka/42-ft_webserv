/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 16:56:51 by abaur             #+#    #+#             */
/*   Updated: 2021/10/18 14:49:25 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include "CGILauncher.hpp"

namespace ft 
{

	std::list<Server>	Server::availableServers;

	Server::Server(const ServerConfig& conf)
	: _config(conf), _ports(conf.ports), _hostname(conf.servername)
	{};

	Server::~Server() {};


/******************************************************************************/
/* # Member functions                                                         */
/******************************************************************************/

	bool	Server::MatchRequest(const RequestHeader& req) const {
		if (!req.IsOk())
			return false;

		if (!this->_hostname.empty() && this->_hostname != req.GetHostname())
			return false;

		for (size_t i=0; i<this->_ports.size(); i++)
			if (_ports[i] == req.GetHostPort())
				return true;

		return false;
	}

	UriConfig	Server::Accept(const RequestHeader& req) {
		UriConfig conf = _config.GetUriConfig(req.GetRequestPath());
		return conf;
	}
	
}
