/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clibft.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 19:06:25 by abaur             #+#    #+#             */
/*   Updated: 2021/09/18 14:57:50 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIBFT_HPP
#define CLIBFT_HPP

#include <string>

namespace ft
{
	/**
	 * Removes all whitespaces from both ends of a string.
	 * @return	A trimmed copy of the input parameter
	 */
	std::string	trim(const std::string&);
}

#endif
