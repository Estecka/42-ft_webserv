/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 15:32:17 by abaur             #+#    #+#             */
/*   Updated: 2021/10/20 14:53:23 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

namespace ft
{
	ServerConfig::ServerConfig(void){
		this->ports.clear();
		this->servername = "";
		this->defaultLocation.handle.prefix = 0;
		this->defaultLocation.handle.path = "";
		this->locations.clear();
	}
	ServerConfig::ServerConfig(const ServerBlock& block) {
		this->ports      = block.GetPorts();
		this->servername = block.GetName();

		{
			PropertyList defprop = block.defaultProperties;
			for (PropertyList::iterator it=defprop.begin(); it!=defprop.end(); ){
				PropertyList::iterator current = it++;
				if (current->first == "listen" || current->first == "server_name")
					defprop.erase(current);
			}

			this->defaultLocation.handle.prefix = 0;
			this->defaultLocation.handle.path = "";
			this->defaultLocation.AddProperties(defprop);
		}

		for (LocationList::const_iterator it=block.locations.begin(); it!=block.locations.end(); it++) {
			this->locations.push_back(this->defaultLocation);
			this->locations.back().AddProperties(it->properties);
			this->locations.back().handle = it->handle;
		}
	}
	ServerConfig::~ServerConfig(){
	}



	const UriConfig&	ServerConfig::GetUriConfig(const std::string& uri) const {
		const UriConfig*	bestMatch = NULL;
		size_t          	bestScore = 0;

		for (std::list<UriConfig>::const_iterator it=this->locations.begin(); it!=this->locations.end(); it++)
		{
			// std::clog << "[DEBUG] \"" << it->handle << "\" match against \"" << uri << "\": ";
			if (UriConfig::UriMatchHandle(uri, it->handle)){
				if (bestScore < it->handle.path.size()) {
					bestScore = it->handle.path.size();
					bestMatch = &*it;
				}
				// std::clog << "true";
			}
			else {
				// std::clog << "false";
			}
			// std::clog << std::endl;
		}

		return bestMatch ? *bestMatch : this->defaultLocation;
	}
}