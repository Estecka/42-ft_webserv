/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 15:14:06 by abaur             #+#    #+#             */
/*   Updated: 2021/09/30 14:19:30 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "UriConfig.hpp"
#include "ServerBlock.hpp"

namespace ft
{
	/**
	 * A fully parsed Server config.
	 */
	struct ServerConfig
	{
		std::vector<int>	ports;
		std::string     	servername;
		UriConfig           	defaultLocation;
		std::list<UriConfig>	locations;

		ServerConfig(void);
		ServerConfig(const ServerBlock& src);
		~ServerConfig();

		/**
		 * Finds the location block associated with the given URI.
		 */
		const UriConfig&	GetUriConfig(const std::string& uri) const;
	};
}


#endif
