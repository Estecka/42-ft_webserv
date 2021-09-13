/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 16:26:01 by abaur             #+#    #+#             */
/*   Updated: 2021/09/12 19:51:09 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

namespace ft
{
	typedef std::map<std::string, std::string>	PropertyMap;

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
		static std::vector<ServerConfig*>	ParseAll(std::istream& conf);

		/**
		 * Extracts the next ServerVonfig from the stream.
		 * If an invalid syntax occurs, this function throws, and `conf` is undefined.
		 * @throw	ft::InvalidSyntaxException
		 * @param conf	A stream to read the config file from.
		 * @return A heap-allocated ServerConfig object, or NULL if end-of-file is encountered.
		 */
		static ServerConfig*	ParseOne(std::istream& conf);

		/**
		 * Gets the port this server should listen to, or 0 if no valid port was found.
		 */
		int	GetPort() const;

		std::ostream&	ToStream(std::ostream& dst) const;


	private:
		PropertyMap	_defaultProperties;
		/**
		 * All the "location { ... }" blocks of this server.
		 * The keys of the map are the location path.
		 */
		std::map<std::string, PropertyMap>	_locations;


		/**
		 * Parses the content of a Server block.
		 * @param input	A stream pointing to the first character after the opening '{'.
		 * @throw ft::InvalidSyntaxException
		 */
		void	ParseServerBlock(std::istream& input);

		/**
		 * Parses the content of a Location block.
		 * This only parses the content of the block, and not the leading text.
		 * @param input	A stream pointing to the first character after the opening '{'.
		 * @param output
		 * @throw ft::InvalidSyntaxException
		 */
		static void	ParseLocationBlock(std::istream& input, PropertyMap& output);
	};
	
}

std::ostream&	operator<<(std::ostream& dst, const ft::ServerConfig& src);

#endif
