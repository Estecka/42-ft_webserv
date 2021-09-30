/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clibft.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 19:06:25 by abaur             #+#    #+#             */
/*   Updated: 2021/09/30 15:07:50 by abaur            ###   ########.fr       */
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

	/**
	 * Extract one word from the given string.
	 * 
	 * All reference should point to different strings.
	 * All outputs are trimmed.
	 * @param src	The string to extract a word from.
	 * @param outWord	Outputs the extracted word.
	 * @param outRemain	Outputs what remains of the string, after the extracted word.
	 * @return	true if a word was extracted.
	 */
	bool	ExtractWord(const std::string& src, std::string& outWord, std::string& outRemain);

	/**
	 * Deletes all pointers in a container.
	 * Null pointers are safely ignored.
	 */
	template <class C>
	void	DeleteContent(C& container){
		for (typename C::iterator it=container.begin(); it!=container.end(); it++)
			if (*it != NULL)
				delete *it;
	}
}

#endif
