/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confparsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 18:25:18 by abaur             #+#    #+#             */
/*   Updated: 2021/08/23 20:05:35 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "confparsing.hpp"

#include <sstream>
#include <cstdio>

namespace ft 
{
namespace confparsing
{
	extern bool	DetectPunctuation(std::istream& input, std::string& outlead, char& outpunc){
		if (input.fail())
			return false;

		std::stringstream	lead;
		while(1) {
			char c = input.get();

			if (c==EOF || c==';' || c=='{' || c =='}') 
			{
				lead << std::flush;
				lead.str(outlead);
				outpunc = c;
				return true;
			}
			else
				lead << c;
		}
	}

	extern bool	DetectInstruction(std::istream& input, std::string& outinstruction) {
		std::streampos cancelpos = input.tellg();
		std::string lead;
		char punc;

		if (!DetectPunctuation(input, lead, punc)) {
			return false;
		}
		else if (punc != ';') {
			input.clear();
			input.seekg(cancelpos);
			return false;
		}
		else {
			outinstruction = lead;
			return true;
		}
	}

	extern bool	DetectBlockEntry(std::istream& input, std::string& outname) {
		std::streampos cancelpos = input.tellg();
		std::string lead;
		char punc;

		if (!DetectPunctuation(input, lead, punc)) {
			return false;
		}
		else if (punc != '{') {
			input.clear();
			input.seekg(cancelpos);
			return false;
		}
		else {
			outname = lead;
			return true;
		}
	}

	extern bool	DetectBlockEnd(std::istream& input){
		std::streampos cancelpos = input.tellg();
		std::string lead;
		char punc;

		if (!DetectPunctuation(input, lead, punc)) {
			return false;
		}
		else if (punc != '}' || lead != "") {
			input.clear();
			input.seekg(cancelpos);
			return false;
		}
		else {
			return true;
		}
	}
}
}
