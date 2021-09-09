/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:42:58 by abaur             #+#    #+#             */
/*   Updated: 2021/09/09 15:08:31 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

#include "InvalidSyntaxException.hpp"
#include "confparsing.hpp"
#include "../clibft/clibft.hpp"

#include <stdexcept>
#include <cstdio>

namespace ft
{
	std::vector<ServerConfig*>	ServerConfig::ParseAll(std::istream& conf)
	{
		std::vector<ServerConfig*> all;
		ServerConfig* one;

		try
		{
			while ((one=ParseOne(conf)) != NULL)
				all.push_back(one);
		} 
		catch (const InvalidSyntaxException&)
		{
			for (size_t i=0; i<all.size(); i++)
				delete all[i];
			throw;
		}

		return all;
	}


	ServerConfig*	ServerConfig::ParseOne(std::istream& conf)
	{
		std::string	lead;
		char       	punctuation;

		confparsing::DetectPunctuation(conf, lead, punctuation);
		ft::trim(lead);
		if (punctuation == EOF && lead == "")
			return NULL;
		if (punctuation != '{' || lead != "server")
			throw InvalidSyntaxException(lead, punctuation);

		ServerConfig& server = *new ServerConfig();
		while (1) {
			confparsing::DetectPunctuation(conf, lead, punctuation);

			if (punctuation == EOF) 
			{
				delete &server;
				throw InvalidSyntaxException("Unexpected end-of-file in \"server{}\".");
			}
			else if (punctuation == ';')
			{
				// [...] Parse instruction;
			}
			else if (punctuation == '{') 
			{
				try {
					// [...] parseLocation
				} catch (const InvalidSyntaxException&) {
					delete &server;
					throw;
				}
				// [...] Add location to server
			}
			else if (punctuation == '}') 
			{
				if (lead != "") {
					delete &server;
					throw InvalidSyntaxException("Unexpected end of server after: "+lead);
				}
				else
					return &server;

			}
			else {
				delete &server;
				throw InvalidSyntaxException(lead, punctuation);
			}
		}
	}

	std::ostream&	ServerConfig::ToStream(std::ostream& dst) const {
		typedef std::map<std::string, ft::ServerLocation*>::const_iterator	locmapiterator;

		dst << "server {" << std::endl;
		dst << "	name:	" << this->_serverName << std::endl;

		dst << "	ports:	" << std::endl;	
		for (size_t i=0; i<this->_ports.size(); i++)
			dst << "		- " << this->_ports[i] << std::endl;

		dst << "	default location:	" << this->_defaultLocation << std::endl;

		dst << "	other locations:	" << std::endl;
		for (locmapiterator it=this->_locations.begin(); it!=this->_locations.end(); ++it)
			dst << "		-" << it->first << '\t' << *(it->second) << std::endl;

		dst << "} #End Server" << std::endl;
		return dst;
	}

}

std::ostream&	operator<<(std::ostream& dst, const ft::ServerConfig& src){
	return src.ToStream(dst);
}
