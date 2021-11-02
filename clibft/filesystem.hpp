/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesystem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 19:31:12 by abaur             #+#    #+#             */
/*   Updated: 2021/10/31 19:41:03 by abaur            ###   ########.fr       */
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
	 * @param slash	??? (@apitoise, fill this in please)
	 */
	bool	IsDir(std::string path, bool slash);
}


#endif
