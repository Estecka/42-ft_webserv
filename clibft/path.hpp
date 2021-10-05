/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 18:00:24 by abaur             #+#    #+#             */
/*   Updated: 2021/10/04 18:54:05 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIBFT_PATH_HPP
#define CLIBFT_PATH_HPP

#include <string>

namespace ft
{
	/**
	 * Extract the filename from a path.
	 */
	std::string	GetFilename(const std::string& path);

	/**
	 * Get the extension of a file.
	 * @return The extension, without the '.', or an empty string if the file has no extension.
	 */
	std::string	GetExtension(const std::string& path);

	/**
	 * Extract the directory from a path.
	 */
	std::string	GetDirectory(const std::string& path);

}

#endif
