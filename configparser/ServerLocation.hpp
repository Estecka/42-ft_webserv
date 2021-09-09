/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 16:40:07 by abaur             #+#    #+#             */
/*   Updated: 2021/09/09 15:17:27 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include <iostream>
#include <string>
#include <vector>

namespace ft
{
	/**
	 * Object representing the content of a "location { ... }" block.
	 * 
	 * TODO:
	 * This structure does not allow a property be undefined.
	 * Switching to a more elementary `map<name, value>` would be more appropriate.
	 * The same logic applies to the ServerConfig class.
	 */
	class ServerLocation
	{
	public:
		ServerLocation();
		ServerLocation(const ServerLocation&);
		~ServerLocation();

		std::ostream&	ToStream(std::ostream& dst) const;

	private:
		bool	_autoindex;
		std::string	_root;
		std::vector<std::string>	_index;
	};
}

std::ostream&	operator<<(std::ostream& dst, const ft::ServerLocation& src);

#endif
