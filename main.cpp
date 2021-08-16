/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/08/16 17:42:50 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <fstream>

#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <stdnoreturn.h>
#include <sys/socket.h>
#include <unistd.h>

char	defaultresponse[] = 
	"HTTP/1.1 200 OK\n"
	"Server: ft_webserv\n"
	"Accept-Ranges: bytes\n"
	"Content-Length: 50\n"
	"Vary: Accept-Encoding\n"
	"Content-Type: text/plain\n"
	"\n"
	"Beep boop. I am a robot, and I make robot noises.\n"
	;

static noreturn void	cthrow(int errnum, const char* message = NULL){
	std::cerr << errnum << '\t' << strerror(errnum) << std::endl;
	if (message)
		std::cerr << message << std::endl;
	exit(errnum);
}

extern int	main(){
	int r;
	int sockfd;
	int sockopt = 1;
	struct sockaddr_in addr;
		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port        = htons(8080);


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == 0)
		cthrow(errno, "Socket creation failed.");

	r = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &sockopt, sizeof(sockopt));
	if (r < 0){
		close(sockfd);
		cthrow(errno, "Socket options failed.");
	}

	r = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if (r < 0){
		close(sockfd);
		cthrow(errno, "Socket binding failed.");
	}

	r = listen(sockfd, 3);
	if (r < 0) {
		close(sockfd);
		cthrow(errno, "Listen failed.");
	}

	socklen_t socklen = sizeof(addr);
	int acceptfd = accept(sockfd, (struct sockaddr*)&addr, &socklen);
	if (acceptfd < 0) {
		close(sockfd);
		cthrow(errno, "Accept failed.");
	}

	char	inbuffer[1025] = {0};
	ssize_t	rlen;
	rlen = read(acceptfd, inbuffer, 1024);
	if (0 < rlen)
		std::cout << "Received :\n" << inbuffer << std::endl;
	else
		std::cout << "Nothing received ("<<rlen<<")" << std::endl;

	r = send(acceptfd, defaultresponse, strlen(defaultresponse), 0);
	if (r < 0)
		std::cerr << "Sending response failed." << std::endl;

	close(acceptfd);
	close(sockfd);
}