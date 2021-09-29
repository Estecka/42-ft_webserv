/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 15:14:06 by abaur             #+#    #+#             */
/*   Updated: 2021/09/29 16:10:00 by abaur            ###   ########.fr       */
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

		/**
		 * Finds the location block associated with the given URI.
		 */
		const UriConfig&	GetUriConfig(const std::string& uri) const;

		void	FromServerBlock(const ServerBlock& src);
	};
}


#endif
