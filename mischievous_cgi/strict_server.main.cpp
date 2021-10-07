/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strict_server.main.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:13:58 by abaur             #+#    #+#             */
/*   Updated: 2021/10/07 17:56:30 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#define BUFFERSIZE	4

extern int	main(){
	pollfd pfd;
	pfd.fd = STDIN_FILENO;
	pfd.events = POLLIN;

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	int err;
	while (true)
	{
		std::cerr << "[SRV] Awaiting poll..." << std::endl;
		err = poll(&pfd, 1, -1);
		if (err < 1) {
			std::cerr << "[SRV] Poll error: " << err << std::endl;
			return EXIT_FAILURE;
		}

		std::cerr << "[SRV] Receiving datas..." << std::endl;
		char	buffer[BUFFERSIZE];
		err = read(STDIN_FILENO, buffer, BUFFERSIZE);
		if (err > 0)
			std::cerr << "[SRV] Received " << err << " bytes" << std::endl;
		else {
			std::cerr << "[SRV] Read error: " << err << std::endl;
			std::cerr << "[SRV] errno: " << errno << ' ' << strerror(errno) << std::endl;
			return EXIT_SUCCESS;
		}
	}
}