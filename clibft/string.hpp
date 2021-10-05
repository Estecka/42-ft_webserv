/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:57:55 by abaur             #+#    #+#             */
/*   Updated: 2021/10/04 18:00:11 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIBFT_STRING_HPP
#define CLIBFT_STRING_HPP

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
	 * Checks whether `needle` is found at the very beginning of `haystack`
	 */
	bool	StartsWith(const std::string& haystack, const std::string& needle);
}

#endif
