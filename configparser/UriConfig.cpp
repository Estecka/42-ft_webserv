/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriConfig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 15:24:10 by abaur             #+#    #+#             */
/*   Updated: 2021/09/29 14:16:31 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UriConfig.hpp"

#include "../clibft/clibft.hpp"

#include <iostream>
#include <cstdlib>

namespace ft
{
	UriConfig::UriConfig() {
		this->handle.prefix = 0;
		this->handle.path   = "";
		this->root      = "";
		this->autoindex = false;
		this->returnCode = 0;
		this->returnPath = "";
		this->index.clear();
	}

	bool	UriConfig::UriMatchHandle(const std::string& uri, const LocationHandle& handle){
		if (handle.prefix) 
		{
			return false;
			// ... To be implemented later
		}
		else 
		{
			const std::string& loc = handle.path;
			size_t locLen = handle.path.length();
			size_t uriLen = uri.length();

			if (uriLen == locLen)
				return uri == loc;
			// else if (uriLen+1 == locLen) // Matches location "/foo/" with "/foo"
			// 	return uri+'/' == loc;
			else if (locLen < uriLen) // Matches location "/foo" with "/foo/bar", but not with "/foobar"
				return (uri.substr(0, locLen) == loc)
				    && (loc[locLen-1] == '/' || uri[locLen] == '/') 
					;
			else
				return false;
		}
	}

	void UriConfig::AddProperties(const PropertyList& properties){
		for (PropertyList::const_iterator it=properties.begin(); it!=properties.end(); it++) {
			if (it->first == "root")
				this->ParseRoot(it->second);
			else if (it->first == "autoindex")
				this->ParseAutoindex(it->second);
			else if (it->first == "index")
				this->ParseIndex(it->second);
			else if (it->first == "return")
				this->ParseReturn(it->second);
			else
				std::cerr << "[WARN] Unknown instruction name: " << it->first << std::endl;
		}
	}

	void	UriConfig::ParseRoot(const std::string& raw) {
		// TODO:
		// Here would be a good place to concatenate the root with the current
		// working directory.
		this->root = raw;
	}

	void	UriConfig::ParseAutoindex(const std::string& raw) {
		if (raw == "on")
			this->autoindex = true;
		else if (raw == "off")
			this->autoindex = false;
		else
			std::cerr << "[WARN] Invalid autoindex value: " << raw << std::endl;
	}

	void	UriConfig::ParseIndex(const std::string& raw) {
		std::string	word, remain;

		remain = raw;
		this->index.clear();
		while (ExtractWord(remain, word, remain)) {
			this->index.push_back(word);
		}
	}

	void	UriConfig::ParseReturn(const std::string& raw){
		std::string code, path;

		ExtractWord(raw, code, path);

		if (code.empty()) {
			std::cerr << "[WARN] Empty return code" << std::endl;
			return;
		}

		for (size_t i=0; i<code.length(); i++)
		if (!std::isdigit(code[i])) {
			std::cerr << "[WARN] Invalid return code: " << code << std::endl;
			return;
		}

		this->returnCode = std::atoi(code.c_str());
		this->returnPath = path;
	}
}
