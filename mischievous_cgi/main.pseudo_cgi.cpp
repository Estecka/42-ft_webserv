/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.pseudo_cgi.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 13:53:05 by abaur             #+#    #+#             */
/*   Updated: 2021/10/07 15:42:37 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>

#include <unistd.h>

extern int	main(int argc, char** argv){
	int sendcount;

	if (argc > 1)
		sendcount = std::atoi(argv[1]);
	else
		sendcount = 3;

	for (int i=0; i<sendcount; i++){
		sleep(2);
		std::cerr << "[CGI] Sending packet " << i << " out of " << sendcount << std::endl;
		std::cout << "Boop\n";
	}

	std::cerr << "[CGI] Finished, shutting down." << std::endl;
}