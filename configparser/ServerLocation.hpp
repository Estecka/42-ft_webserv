/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 16:40:07 by abaur             #+#    #+#             */
/*   Updated: 2021/08/23 18:24:08 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include <string>
#include <vector>

namespace ft
{
	/**
	 * Object representing the content of a "location { ... }" block.
	 */
	class ServerLocation
	{
	public:
		ServerLocation();
		ServerLocation(const ServerLocation&);
		~ServerLocation();

	private:
		bool	_autoindex;
		std::string	_root;
		std::vector<std::string>	_index;
	};
}


#endif
