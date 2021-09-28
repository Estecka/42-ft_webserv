/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriConfig.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 15:23:04 by abaur             #+#    #+#             */
/*   Updated: 2021/09/27 18:13:22 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URICONFIG_HPP
#define URICONFIG_HPP

#include "shorhands.hpp"

#include <string>
#include <vector>

namespace ft 
{
	/**
	 * A structure that represents a portion of a ServerConfig. The parts which 
	 * is relevant to a specific request.
	 */
	struct UriConfig
	{
		std::string	root;
		bool	autoindex;

		UriConfig(void);

		void	AddProperties(const PropertyList&);

		static bool	UriMatchHandle(const std::string& uri, const LocationHandle& handle);

		void	ParseRoot(const std::string&);
		void	ParseAutoindex(const std::string&);
		void	ParseIndex(const std::string&);
		void	ParseReturn(const std::string&);
	};
}

#endif
