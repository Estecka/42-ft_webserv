/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:42:58 by abaur             #+#    #+#             */
/*   Updated: 2021/09/10 19:38:35 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

#include "InvalidSyntaxException.hpp"
#include "../clibft/clibft.hpp"

#include <sstream>
#include <stdexcept>
#include <cstdio>

namespace ft
{
	/**
	 * Extract the next element from the stream, regardless of its validity.
	 * This will extract any text terminated by ';', '{', '}', or EOF.
	 * @param input	Stream pointing to the beginning of next element.
	 * @param outlead	Output the raw texts of the element, excluding the terminator.
	 * @param outpunc	Outputs the value of the terminator.
	 * @return
	 * 	true 	Success.
	 * 	false	Nothing was extracted.
	 */
	static bool	DetectPunctuation(std::istream& input, std::string& outlead, char& outpunc)
	{
		std::stringstream	lead;

		while (1) {
			char c = input.get();

			if (c==EOF || c==';' || c=='{' || c =='}') 
			{
				lead << std::flush;
				lead.str(outlead);
				outpunc = c;
				return c!=EOF || outlead!="";
			}
			else
				lead << c;
		}
	}

	/**
	 * Splits a raw instruction into a name and value.
	 * @param raw	The raw text of the instruction, without the terminating ';'.
	 * @param outname	Outputs the name of the instruction.
	 * @param outvalue	Outputs the raw value of the instruction.
	 */
	static void	ParseInstruction(const std::string& _raw, std::string& outname, std::string outvalue)
	{
		std::string raw = ft::trim(_raw);

		size_t i = 0;
		while (!std::isspace(raw[i]))
			i++;
		outname  = raw.substr(0, i);

		while (std::isspace(raw[i]))
			i++;
		outvalue = raw.substr(i, raw.length());
	}

	/**
	 * Add a property to the list.
	 * This log a warning in case of duplicata.
	 */
	static inline void	AddInstruction(PropertyMap& dst, const std::string& name, const std::string& value)
	{
		if (dst.count(name)) {
			std::cerr << "[WARN] Duplicate property " << name << ". "
				<< "The last one will overwrite the former." << std::endl;
		}
		dst[name] = value;
	}


/******************************************************************************/
/* # ServerConfig functions                                                   */
/******************************************************************************/

	ServerConfig::ServerConfig(){}
	ServerConfig::ServerConfig(const ServerConfig& other){
		this->_defaultProperties = other._defaultProperties;
		this->_locations = other._locations;
	}
	ServerConfig::~ServerConfig(){}


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

		DetectPunctuation(conf, lead, punctuation);
		ft::trim(lead);
		if (punctuation == EOF && lead == "")
			return NULL;
		if (punctuation != '{' || lead != "server")
			throw InvalidSyntaxException(lead, punctuation);

		ServerConfig& server = *new ServerConfig();
		try {
			server.ParseServerBlock(conf);
		}
		catch (const InvalidSyntaxException&){
			delete &server;
			throw;
		}

		return &server;
	}

	std::ostream&	ServerConfig::ToStream(std::ostream& dst) const {
		dst << "server {" << std::endl;

		dst << "} #End Server" << std::endl;
		return dst;
	}

	void	ServerConfig::ParseServerBlock(std::istream& input)
	{
		std::string	lead;
		char       	punctuation;

		while (1) {
			DetectPunctuation(input, lead, punctuation);
			lead = ft::trim(lead);

			if (punctuation == ';')
			{
				std::string name, value;
				ParseInstruction(lead, name, value);
				AddInstruction(this->_defaultProperties, name, value);
			}
			else if (punctuation == '{') 
			{
				std::string	prefix, value;
				ParseInstruction(lead, prefix, value);
				if (prefix != "location")
					throw InvalidSyntaxException("Unexpected block " + prefix);
				if (this->_locations.count(value))
					std::cerr << "[WARN] Duplicate location: " << value << "\n"
						<< "Contents of duplicatas will be merged.";
				ParseLocationBlock(input, this->_locations[value]);
			}
			else if (punctuation == '}') {
				if (lead.empty())
					return;
				else
					throw InvalidSyntaxException("Unexpected end of server after: "+lead);
			}
			else if (punctuation == EOF) 
				throw InvalidSyntaxException("Unexpected end-of-file in server bloack after " + lead);
			else
				throw InvalidSyntaxException(lead, punctuation);
		}
	}
	
	void	 ServerConfig::ParseLocationBlock(std::istream& input, PropertyMap& output)
	{
		std::string	lead;
		char       	punc;

		while (1) {
			DetectPunctuation(input, lead, punc);
			ft::trim(lead);

			if (punc == ';') {
				std::string name, value;
				ParseInstruction(lead, name, value);
				AddInstruction(output, name, value);
			}
			else if (punc == '}'){
				if (lead.empty())
					return;
				else
					throw InvalidSyntaxException("Unexpected end of location after " + lead);
			}
			else if (punc == '{')
				throw InvalidSyntaxException("Unexpected block opening in location block after" + lead);
			else if (punc == EOF)
				throw InvalidSyntaxException("Unexpected end of file in location block after after "+ lead);
			else
				throw InvalidSyntaxException(lead, punc);
		}
	}

}


std::ostream&	operator<<(std::ostream& dst, const ft::ServerConfig& src){
	return src.ToStream(dst);
}
