/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 16:26:01 by abaur             #+#    #+#             */
/*   Updated: 2021/08/25 19:15:05 by abaur            ###   ########.fr       */
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

		/**
		 * Extract all server configs until he end of stream is reached.
		 * If an invalid syntax occurs, this function throws, and `conf` is undefined.
		 * @throw	ft::InvalidSyntaxException
		 * @param conf	A stream to read the config file from.
		 * @return An array of heap-allocated objects. These should be manually deleted before destroying the array.
		 */
		static std::vector<ServerConfig&>	ParseAll(std::istream& conf);

		/**
		 * Extracts the next ServerVonfig from the stream.
		 * If an invalid syntax occurs, this function throws, and `conf` is undefined.
		 * @throw	ft::InvalidSyntaxException
		 * @param conf	A stream to read the config file from.
		 * @return A heap-allocated ServerConfig object, or NULL if end-of-file is encountered.
		 */
		static ServerConfig*	ParseOne(std::istream& conf);

	private:
		std::string	_serverName;
		std::vector<int>	_ports;
		ServerLocation	_defaultLocation;
		/**
		 * All the "location { ... }" blocks of this server.
		 * The keys of the map are the location path.
		 * All locations should be freed before destroying this map.
		 */
		std::map<std::string, ServerLocation&>	_locations;
	};
	
}

#endif
