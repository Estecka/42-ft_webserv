/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:42:58 by abaur             #+#    #+#             */
/*   Updated: 2021/09/06 15:15:04 by abaur            ###   ########.fr       */
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

}


