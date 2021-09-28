/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriConfig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 15:24:10 by abaur             #+#    #+#             */
/*   Updated: 2021/09/27 18:27:20 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UriConfig.hpp"

#include <iostream>

namespace ft
{
	UriConfig::UriConfig() {
		this->root      = "";
		this->autoindex = false;
	}

	bool	UriConfig::UriMatchHandle(const std::string& uri, const LocationHandle& handle){
		if (handle.prefix) 
		{
			return false;
			// ... To be implemented later
		}
		else 
		{
			if (uri.length() < handle.path.length())
				return false;

			size_t i;
			for (i=0; i<handle.path.length(); i++)
				if (uri[i] != handle.path[i])
					return false;

			if (uri.length() == handle.path.length())
				return true;

			if (handle.path[i-1] == '/')
				return true;
			// handle "/foo" matches with uri "/foo/bar", but not with "/foobar"
			return uri[i] == '/';
		}
	}

	void UriConfig::AddProperties(const PropertyList& properties){
		for (PropertyList::const_iterator it=properties.begin(); it!=properties.end(); it++) {
			if (it->first == "root")
				this->ParseRoot(it->second);
			else if (it->first == "autoindex")
				this->ParseAutoindex(it->second);
			else if (it->first == "index")
				; // ...
			else if (it->first == "return")
				; // ...
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
}
