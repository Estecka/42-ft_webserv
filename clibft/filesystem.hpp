/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesystem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 19:31:12 by abaur             #+#    #+#             */
/*   Updated: 2021/11/05 09:31:39 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIBFT_FILESYSTEM_HPP
#define CLIBFT_FILESYSTEM_HPP

#include <string>

namespace ft
{
	/**
	 * Checks whether a directory with the given name exists.
	 * @param path
	 * @param slash	is set true if your path ended by a slash or false if not
	 */
	bool	IsDir(std::string path, bool slash);
	bool	IsFile(std::string path);
	int		RecursiveMkdir(std::string path, size_t start);
}


#endif
