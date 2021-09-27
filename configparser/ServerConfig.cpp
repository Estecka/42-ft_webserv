/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:42:58 by abaur             #+#    #+#             */
/*   Updated: 2021/09/27 17:49:15 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

#include "InvalidSyntaxException.hpp"
#include "../clibft/clibft.hpp"

#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

namespace ft
{
	ServerConfig::ServerConfig(){}
	ServerConfig::ServerConfig(const ServerConfig& other){
		this->_defaultProperties = other._defaultProperties;
		this->_locations = other._locations;
	}
	ServerConfig::~ServerConfig(){}



/******************************************************************************/
/* # Data validation                                                          */
/******************************************************************************/

	static bool	ValidatePort(const std::string& raw) {
		if (raw.empty()) {
			std::cerr << "[WARN] Empty port in config file. \n"
			          << "       This port willl be ignored."
			          << std::endl;
			return false;
		}
		else for (size_t i=0; i<raw.length(); i++) {
			if (!isdigit(raw[i])) {
				std::cerr << "[WARN] Invalid port in config file: " << raw << '\n'
				          << "       This port will be ignored."
				          << std::endl;
				return false;
			}
		}
		return true;
	}



/******************************************************************************/
/* # Accessors                                                                */
/******************************************************************************/

	std::vector<int>	ServerConfig::GetPorts() const {
		std::vector<int> r;
		for (PropertyList::const_iterator it=_defaultProperties.begin(); it!=_defaultProperties.end(); it++)
		if (it->first == "listen")
		{
			if (ValidatePort(it->second))
				r.push_back(std::atoi(it->second.c_str()));
			else
				std::cerr << "[WARN] Invalid port value in config: " << it->second << "\n"
				          << "       This port will be ignored."
				          << std::endl;
		}
		return r;
	}

	std::string	ServerConfig::GetName() const
	{
		std::string	name;
		for (PropertyList::const_iterator it=_defaultProperties.begin(); it!=_defaultProperties.end(); it++)
		if (it->first == "server_name")
		{
			if (!name.empty())
				std::cerr << "[WARN] Duplicate server_name. The last one will overwrite the formers." << std::endl;
			name = it->second;
		}
		return name;
	}

	std::string	ServerConfig::GetRoot() const {
		std::string	root;
		for (PropertyList::const_iterator it=_defaultProperties.begin(); it!=_defaultProperties.end(); it++)
			if (it->first == "root") {
				if (!root.empty())
					std::cerr << "[WARN] Duplicate root. The last one will overwrite the formers." << std::endl;
				root = it->second;
			}
			return (root);
	}

	UriConfig	ServerConfig::GetUriConfig(const std::string& uri) {
		UriConfig	result;
		const PropertyList*	bestMatch = NULL;
		size_t             	bestScore = 0;

		for (LocationList::const_iterator it=_locations.begin(); it!=_locations.end(); it++)
		if (UriConfig::UriMatchHandle(uri, it->handle) && bestScore < it->handle.path.size()) {
			bestScore = it->handle.path.size();
			bestMatch = &it->properties;
		}

		result.AddProperties(this->_defaultProperties);
		if (bestMatch)
			result.AddProperties(*bestMatch);

		return result;
	}



/******************************************************************************/
/* # Parsing                                                                  */
/******************************************************************************/

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
				outlead = lead.str();
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
	static void	ParseInstruction(const std::string& _raw, std::string& outname, std::string& outvalue)
	{
		std::string raw = ft::trim(_raw);

		size_t i = 0;
		while (raw[i] && !std::isspace(raw[i]))
			i++;
		outname  = raw.substr(0, i);

		while (raw[i] && std::isspace(raw[i]))
			i++;
		outvalue = raw.substr(i, raw.length()-i);

	}

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
		lead = ft::trim(lead);
		if (punctuation == EOF && lead == "")
			return NULL;
		if (punctuation != '{' || lead != "server")
			throw InvalidSyntaxException("Unexpected syntax while looking for a server block: ", lead, punctuation);

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
				this->_defaultProperties.push_back(StrPair(name, value));
			}
			else if (punctuation == '{') 
			{
				std::string	prefix, path;
				ParseInstruction(lead, prefix, path);
				if (prefix != "location")
					throw InvalidSyntaxException("Unexpected block: " + prefix);

				this->_locations.resize(_locations.size()+1);
				ServerLocation& loc = _locations.back();
				ParseLocationHandle(path, loc.handle);
				ParseLocationBlock(input, loc.properties);
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
				throw InvalidSyntaxException("Unexpected syntax while parsing a server block: ", lead, punctuation);
		}
	}

	void	ServerConfig::ParseLocationHandle(const std::string& rawHandle, LocationHandle& outhandle) {
		std::string word1;
		std::string word2;

		ParseInstruction(rawHandle, word1, word2);
		word1 = ft::trim(word1);
		word2 = ft::trim(word2);

		if (word1 == "")
			throw InvalidSyntaxException("Location block has no path");
		else if (word2 == "") {
			outhandle.prefix = 0;
			outhandle.path   = word1;
		}
		else if (word1.length() > 1)
			throw InvalidSyntaxException("Location prefix is too long: " + word1);
		else {
			outhandle.prefix = word1[0];
			outhandle.path   = word2;
		}
	}
	
	void	 ServerConfig::ParseLocationBlock(std::istream& input, PropertyList& output)
	{
		std::string	lead;
		char       	punc;

		while (1) {
			DetectPunctuation(input, lead, punc);
			lead = ft::trim(lead);

			if (punc == ';') {
				std::string name, value;
				ParseInstruction(lead, name, value);
				output.push_back(StrPair(name, value));
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
				throw InvalidSyntaxException("Unexpected syntax when parsing a location block: ", lead, punc);
		}
	}



/******************************************************************************/
/* # Printing                                                                 */
/******************************************************************************/

	static std::ostream&	LocationToStream(std::ostream& dst, const PropertyList& loc, int indentLevel = 0)
	{
		for (PropertyList::const_iterator it=loc.begin(); it!=loc.end(); it++) {
			for (int i=0; i<indentLevel; i++)
				dst << '\t';
			dst << it->first << ":\t" << it->second << std::endl;
		}
		return dst;
	}

	std::ostream&	ServerConfig::ToStream(std::ostream& dst) const 
	{
		dst << "server {" << std::endl;
		LocationToStream(std::cout, this->_defaultProperties, 1);
		for (LocationList::const_iterator it=_locations.begin(); it!=_locations.end(); it++){
			std::cout << "\tLocation: " << it->handle << " {" << std::endl;
			LocationToStream(std::cout, it->properties, 2);
			std::cout << "\t} #End Location" << std::endl;
		}
		dst << "} #End Server" << std::endl;
		return dst;
	}

} // End namespace


std::ostream&	operator<<(std::ostream& dst, const ft::ServerConfig& src){
	return src.ToStream(dst);
}

std::ostream&	operator<<(std::ostream& dst, const ft::LocationHandle& src){
	if (src.prefix)
		dst << src.prefix << ' ';
	dst << src.path;
	return dst;
}
