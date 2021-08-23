/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 16:26:01 by abaur             #+#    #+#             */
/*   Updated: 2021/08/23 18:23:19 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "ServerLocation.hpp"

#include <vector>
#include <map>

namespace ft
{
	/**
	 * Object representing one "server { ... }" block from a config file.
	 */
	class ServerConfig
	{
	public:
		ServerConfig();
		ServerConfig(const ServerConfig&);
		~ServerConfig();

	private:
		std::string	_serverName;
		std::vector<int>	_ports;
		ServerLocation	_defaultLocation;
		std::map<std::string, ServerLocation>	_locations;
	};
	
}

#endif
