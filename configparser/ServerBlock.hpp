/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 16:26:01 by abaur             #+#    #+#             */
/*   Updated: 2021/09/29 16:00:28 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include "shorhands.hpp"
#include "UriConfig.hpp"

#include <iostream>
#include <vector>

namespace ft
{
	/**
	 * Rough representation of one "server { ... }" block from a config file.
	 */
	struct ServerBlock
	{
	public:
		PropertyList	defaultProperties;
		LocationList	locations;

		/**
		 * Extract all server blocks until the end of stream is reached.
		 * If an invalid syntax occurs, this function throws, and `conf` is undefined.
		 * @throw	ft::InvalidSyntaxException
		 * @param conf	A stream to read the config file from.
		 * @return An array of heap-allocated objects. These should be manually deleted before destroying the array.
		 */
		static std::vector<ServerBlock*>	ParseAll(std::istream& conf);

		/**
		 * Extracts the next server block from the stream.
		 * If an invalid syntax occurs, this function throws, and `conf` is undefined.
		 * @throw	ft::InvalidSyntaxException
		 * @param conf	A stream to read the config file from.
		 * @return A heap-allocated ServerConfig object, or NULL if end-of-file is encountered.
		 */
		static ServerBlock*	ParseOne(std::istream& conf);

		std::vector<int>	GetPorts() const;
		std::string     	GetName() const;

		std::ostream&	ToStream(std::ostream& dst) const;


	private:
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

std::ostream&	operator<<(std::ostream& dst, const ft::ServerBlock& src);
std::ostream&	operator<<(std::ostream& dst, const ft::LocationHandle& src);

#endif
