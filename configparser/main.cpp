/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 14:33:58 by abaur             #+#    #+#             */
/*   Updated: 2021/09/06 14:55:50 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

#include <cerrno>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>

extern int main(int argc, char** argv){
	if (argc < 2) {
		std::cerr << "Needs a config path in argument." << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream file(argv[1]);
	if (file.fail()) {
		std::cerr << "Couldn't open: " << argv[1] << std::endl;
		std::cerr << errno << " " << std::strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
}
