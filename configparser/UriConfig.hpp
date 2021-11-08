/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriConfig.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 15:23:04 by abaur             #+#    #+#             */
/*   Updated: 2021/11/05 16:08:57 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URICONFIG_HPP
#define URICONFIG_HPP

#include "shorhands.hpp"

#include <string>
#include <vector>
#include <map>

namespace ft 
{
	/**
	 * A structure that represents a portion of a ServerConfig. The parts which 
	 * is relevant to a specific request.
	 */
	struct UriConfig
	{
		LocationHandle	handle;
		std::string	rootedUri;
		std::string	root;
		std::string	upload_path;
		bool	autoindex;
		std::vector<std::string>	index;
		std::vector<std::string>	methods;
		std::map<int, std::string>	error_page;
		std::map<std::string, std::string>	cgis;
		int	returnCode;
		std::string	returnPath;
		std::string	cgiPath;

		UriConfig(void);

		void	AddProperties(const PropertyList&);

		static bool	UriMatchHandle(const std::string& uri, const LocationHandle& handle);

		void	ParseRoot(const std::string&);
		void	ParseUploadPath(const std::string&);
		void	ParseAutoindex(const std::string&);
		void	ParseIndex(const std::string&);
		void	ParseMethods(const std::string&);
		void	ParseReturn(const std::string&);
		void	ParseErrorPage(const std::string&);
		void	ParseCgi(const std::string&);
	};
}

#endif
