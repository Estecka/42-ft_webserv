/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confparsing.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 17:52:57 by abaur             #+#    #+#             */
/*   Updated: 2021/08/25 19:32:24 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSING_HPP
#define CONFPARSING_HPP

#include <iostream>

namespace ft 
{
namespace confparsing 
{

	/**
	 * Extract the next element from the stream, regardless of its validity.
	 * This will extract any text terminated by ';', '{', '}', or EOF.
	 * @param input	Stream pointing to the beginning of next element.
	 * @param outlead	Output the raw texts of the element, excluding the terminator.
	 * @param outpunctuation	Outputs the value of the terminator.
	 * @return
	 * 	true 	Success.
	 * 	false	Nothing was extracted.
	 */
	extern bool	DetectPunctuation(std::istream& input, std::string& outlead, char& outpunctuation);

	/**
	 * Checks that the next element is an instruction, and extract it.
	 * An instruction in this context, is an arbitrary text terminated by a ';'. That text may contain any other chararcter, except '{' and '}'.
	 * On success, the input will then point to the first character following the extracted element.
	 * @param input	Stream pointing to the beginning of the presumed instruction, or any leading whitespace.
	 * @param outinstruction	Output the raw texts of the instruction, excluding the terminating ';'
	 * @return
	 * 	true 	An instruction was extracted.
	 * 	false	The next element in the stream is not an instruction.
	 */
	extern bool	DetectInstruction(std::istream& input, std::string& outinstruction);

	/**
	 * Checks whether the next element is the begining of a block. 
	 * A block start in this context, is an arbitrary text terminated by a '{'. That text may contain any other character, except '}', and ';'. 
	 * This function **only** checks for a block start, and does not guarantee that block is properly closed. 
	 * On success, the input will then point to the first character following the extracted element.
	 * @param input	Stream pointing to the beginning of the presumed block start, or any leading whitespace.
	 * @param outname	Outputs the value of the text leading the block.
	 * @return
	 * 	true 	A block entry was extracted.
	 * 	false	The next element in the file is not a block.
	 */
	extern bool	DetectBlockEntry(std::istream& input, std::string& outname);

	/**
	 * Checks whether the current block ends here.
	 * On success, the input will then point to the first character following the extracted element.
	 * @param input	Stream pointing to the presumed block end, or any leading whitespace.
	 * @return
	 * 	true 	The block end was extracted.
	 * 	false	There is still content left to parse within the block.
	 */
	extern bool	DetectBlockEnd(std::istream& input);

}
}

#endif
