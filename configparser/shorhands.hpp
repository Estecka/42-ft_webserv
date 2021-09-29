/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shorhands.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:38:51 by abaur             #+#    #+#             */
/*   Updated: 2021/09/29 15:09:10 by abaur            ###   ########.fr       */
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

	struct LocationBlock
	{
		LocationHandle	handle;
		PropertyList  	properties;
	};
	
	typedef std::list<LocationBlock>	LocationList;
}

#endif
