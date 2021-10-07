/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.pseudo_server.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 14:02:05 by abaur             #+#    #+#             */
/*   Updated: 2021/10/07 14:18:02 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

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
		char	buffer[1024];
		err = read(STDIN_FILENO, buffer, 1024);
		if (err > 0)
			std::cerr << "[SRV] Received " << err << " bytes" << std::endl;
		else {
			std::cerr << "[SRV] Read error: " << err << std::endl;
			return EXIT_SUCCESS;
		}
	}
}