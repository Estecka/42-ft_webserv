/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 16:26:01 by abaur             #+#    #+#             */
/*   Updated: 2021/09/26 19:51:04 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <map>
#include <list>
#include <vector>

namespace ft
{
	typedef std::pair<std::string, std::string>	StrPair;
	typedef std::list<StrPair>	PropertyList;

	struct LocationHandle
	{
		char       	prefix;
		std::string	path;
	};

	struct ServerLocation
	{
		LocationHandle	handle;
		PropertyList  	properties;
	};
	
	typedef std::list<ServerLocation>	LocationList;


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

		std::vector<int>	GetPorts() const;
		std::string     	GetName() const;
		std::string			GetRoot() const;

		std::ostream&	ToStream(std::ostream& dst) const;


	private:
		PropertyList	_defaultProperties;
		/**
		 * All the "location { ... }" blocks of this server.
		 * The keys of the map are the location path.
		 */
		LocationList	_locations;


		/**
		 * Parses the content of a Server block.
		 * @param input	A stream pointing to the first character after the opening '{'.
		 * @throw ft::InvalidSyntaxException
		 */
		void	ParseServerBlock(std::istream& input);

		/**
		 * Parses the handle of a location block.
		 * @param rawHandle	The unparsed handle.
		 * @param outHandle	Outputs the resulting handle.
		 * @throw ft::InvalidSyntaxException
		 */
		void	ParseLocationHandle(const std::string& rawHandle, LocationHandle& outHandle);

		/**
		 * Parses the content of a Location block.
		 * This only parses the content of the block, and not the leading text.
		 * @param input	A stream pointing to the first character after the opening '{'.
		 * @param output
		 * @throw ft::InvalidSyntaxException
		 */
		static void	ParseLocationBlock(std::istream& input, PropertyList& output);
	};
	
}

std::ostream&	operator<<(std::ostream& dst, const ft::ServerConfig& src);
std::ostream&	operator<<(std::ostream& dst, const ft::LocationHandle& src);

#endif
