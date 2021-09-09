/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 15:12:26 by abaur             #+#    #+#             */
/*   Updated: 2021/09/09 15:32:08 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLocation.hpp"

namespace ft
{
	ServerLocation::ServerLocation(){}
	ServerLocation::ServerLocation(const ServerLocation& other){
		this->_autoindex = other._autoindex;
		this->_root = other._root;
		this->_index = other._index;
	}
	ServerLocation::~ServerLocation(){}

	std::ostream&	ServerLocation::ToStream(std::ostream& dst) const {
		dst << "location {" << std::endl;
		dst << "	autoindex:	" << this->_autoindex << std::endl;
		dst << "	root:	" << this->_root << std::endl;
		dst << "	index:	" << std::endl;
		for (size_t i=0; i<this->_index.size(); i++)
			dst << "		- " << this->_index[i] << std::endl;
		dst << "} #End Location" << std::endl;
	}
}
