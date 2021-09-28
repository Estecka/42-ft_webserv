/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shorhands.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:38:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/27 17:40:17 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_SHORHANDS_HPP
#define CONFIGPARSER_SHORHANDS_HPP

#include <list>
#include <string>

namespace ft 
{
	typedef std::pair<std::string, std::string>	StrPair;
	typedef std::list<StrPair>	PropertyList;

	struct LocationHandle
	{
		char       	prefix;
		std::string	path;
	};

	struct ServerLocation
	{
		LocationHandle	handle;
		PropertyList  	properties;
	};
	
	typedef std::list<ServerLocation>	LocationList;
}

#endif
