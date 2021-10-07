/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filestream_server.main.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 16:12:45 by abaur             #+#    #+#             */
/*   Updated: 2021/10/07 16:48:28 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../clibft/fdstream.hpp"

#include <iostream>
#include <fstream>

#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

extern int	main(){
	pollfd pfd;
	pfd.fd = STDIN_FILENO;
	pfd.events = POLLIN;

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	ft::ifdstream input(STDIN_FILENO);

	int err;
	while (true)
	{
		std::cerr << "[SRV] Awaiting poll..." << std::endl;
		err = poll(&pfd, 1, -1);
		if (err < 1) {
			std::cerr << "[SRV] Poll error: " << err << std::endl;
			return EXIT_FAILURE;
		}

		std::string	line;
		input.clear();
		errno = 0;
		while(true) {
			std::getline(input, line);
			std::cerr << "[SRV] Extracted: " << line << std::endl;
			std::cerr << "[SRV] Errno : " << errno << ' ' << strerror(errno) << std::endl;
			std::cerr << "[SRV] Failed: " << input.fail() << std::endl;
			std::cerr << "[SRV] EOF:    " << input.eof() << std::endl;
			if (input.fail())
				break;
		}


		if (input.eof()){
			std::cerr << "[SRV] Exiting " << std::endl;
			return EXIT_SUCCESS;
		}
	}
}