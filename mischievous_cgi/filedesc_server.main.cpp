/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filedesc_server.main.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 14:02:05 by abaur             #+#    #+#             */
/*   Updated: 2021/10/07 17:11:07 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstring>

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
		std::cout << "[SRV] Awaiting poll..." << std::endl;
		err = poll(&pfd, 1, -1);
		if (err < 1) {
			std::cout << "[SRV] Poll error: " << err << std::endl;
			return EXIT_FAILURE;
		}

		errno = 0;
		std::cout << "[SRV] Receiving datas..." << std::endl;
		char	buffer[BUFFERSIZE];
		while (0 < (err=read(STDIN_FILENO, buffer, BUFFERSIZE)))
			std::cout << "[SRV] Received " << err << " bytes" << std::endl;
		
		std::cout << "[SRV] Read error: " << err << std::endl;
		std::cout << "[SRV] errno: " << errno << ' ' << strerror(errno) << std::endl;
		if (err == 0)
			return EXIT_SUCCESS;
	}
}