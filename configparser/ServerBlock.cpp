/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:42:58 by abaur             #+#    #+#             */
/*   Updated: 2021/09/29 16:01:00 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

#include "InvalidSyntaxException.hpp"
#include "../clibft/clibft.hpp"

#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

namespace ft
{

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

	std::vector<int>	ServerBlock::GetPorts() const {
		std::vector<int> r;
		for (PropertyList::const_iterator it=defaultProperties.begin(); it!=defaultProperties.end(); it++)
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

	std::string	ServerBlock::GetName() const
	{
		std::string	name;
		for (PropertyList::const_iterator it=defaultProperties.begin(); it!=defaultProperties.end(); it++)
		if (it->first == "server_name")
		{
			if (!name.empty())
				std::cerr << "[WARN] Duplicate server_name. The last one will overwrite the formers." << std::endl;
			name = it->second;
		}
		return name;
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

	std::vector<ServerBlock*>	ServerBlock::ParseAll(std::istream& conf)
	{
		std::vector<ServerBlock*> all;
		ServerBlock* one;

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


	ServerBlock*	ServerBlock::ParseOne(std::istream& conf)
	{
		std::string	lead;
		char       	punctuation;

		DetectPunctuation(conf, lead, punctuation);
		lead = ft::trim(lead);
		if (punctuation == EOF && lead == "")
			return NULL;
		if (punctuation != '{' || lead != "server")
			throw InvalidSyntaxException("Unexpected syntax while looking for a server block: ", lead, punctuation);

		ServerBlock& server = *new ServerBlock();
		try {
			server.ParseServerBlock(conf);
		}
		catch (const InvalidSyntaxException&){
			delete &server;
			throw;
		}

		return &server;
	}

	void	ServerBlock::ParseServerBlock(std::istream& input)
	{
		std::string	lead;
		char       	punctuation;

		while (1) {
			DetectPunctuation(input, lead, punctuation);
			lead = ft::trim(lead);

			if (punctuation == ';')
			{
				std::string name, value;
				ft::ExtractWord(lead, name, value);
				this->defaultProperties.push_back(StrPair(name, value));
			}
			else if (punctuation == '{') 
			{
				std::string	prefix, path;
				ft::ExtractWord(lead, prefix, path);
				if (prefix != "location")
					throw InvalidSyntaxException("Unexpected block: " + prefix);

				this->locations.resize(locations.size()+1);
				LocationBlock& loc = locations.back();
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

	void	ServerBlock::ParseLocationHandle(const std::string& rawHandle, LocationHandle& outhandle) {
		std::string word1;
		std::string word2;

		ft::ExtractWord(rawHandle, word1, word2);
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
	
	void	 ServerBlock::ParseLocationBlock(std::istream& input, PropertyList& output)
	{
		std::string	lead;
		char       	punc;

		while (1) {
			DetectPunctuation(input, lead, punc);
			lead = ft::trim(lead);

			if (punc == ';') {
				std::string name, value;
				ft::ExtractWord(lead, name, value);
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

	std::ostream&	ServerBlock::ToStream(std::ostream& dst) const 
	{
		dst << "server {" << std::endl;
		LocationToStream(std::cout, this->defaultProperties, 1);
		for (LocationList::const_iterator it=locations.begin(); it!=locations.end(); it++){
			std::cout << "\tLocation: " << it->handle << " {" << std::endl;
			LocationToStream(std::cout, it->properties, 2);
			std::cout << "\t} #End Location" << std::endl;
		}
		dst << "} #End Server" << std::endl;
		return dst;
	}

} // End namespace


std::ostream&	operator<<(std::ostream& dst, const ft::ServerBlock& src){
	return src.ToStream(dst);
}

std::ostream&	operator<<(std::ostream& dst, const ft::LocationHandle& src){
	if (src.prefix)
		dst << src.prefix << ' ';
	dst << src.path;
	return dst;
}
