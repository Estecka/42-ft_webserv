/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 14:33:58 by abaur             #+#    #+#             */
/*   Updated: 2021/09/29 17:19:10 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configparser.hpp"

#include <cerrno>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>

extern int main(int argc, char** argv){
	if (argc < 2) {
		std::cerr << "Needs a config file in argument." << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream file(argv[1]);
	if (file.fail()) {
		std::cerr << "Couldn't open: " << argv[1] << std::endl;
		std::cerr << errno << " " << std::strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<ft::ServerBlock*>	blocks;
	try {
		blocks = ft::ServerBlock::ParseAll(file);
	}
	catch (ft::InvalidSyntaxException& e) {
		std::cout << "Invalid Syntax : " << std::endl;
		std::cout << e.what() << std::endl;
		return EXIT_SUCCESS;
	}

	if (!blocks.size()) {
		std::cout << "Config is empty" << std::endl;
		return EXIT_SUCCESS;
	}
	for (size_t i=0; i<blocks.size(); i++) {
		if (blocks[i] == NULL) {
			std::cerr << "NULL ServerBlock Object at index "
				      << i << " out of " << blocks.size() << std::endl;
		}
		else {
			std::cout << '['<<i<<']' << *(blocks[i]);
			std::cout << std::endl;
		}
	}

	try {
		for (size_t i=0; i<blocks.size(); i++){
			ft::ServerConfig	conf;
			conf.FromServerBlock(*(blocks[i]));
		}
	}
	catch (ft::InvalidSyntaxException& e) {
		std::cout << "Invalid Syntax :" << std::endl;
		std::cout << e.what() << std::endl;
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
