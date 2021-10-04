/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 18:04:36 by abaur             #+#    #+#             */
/*   Updated: 2021/10/04 18:53:58 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path.hpp"

#include "clibft.hpp"

#define NPOS	std::string::npos

namespace ft
{
	std::string	GetFilename(const std::string& path) {
		size_t i = path.rfind('/');
		if (i == NPOS)
			return path;
		else
			return path.substr(i+1);
	}

	std::string	GetExtension(const std::string& path) {
		size_t dot   = path.rfind('.');
		size_t slash = path.rfind('/');

		if (dot == NPOS)
			return "";
		else if (slash != NPOS && dot < slash) // the dot is actually part of a folder name.
			return "";
		else
			return path.substr(dot+1);
	}

	std::string	GetDirectory(const std::string& path) {
		size_t i = path.rfind('/');
		if (i == NPOS)
			return "";
		else
			return path.substr(0, i);
	}

}
