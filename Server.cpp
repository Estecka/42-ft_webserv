/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:01:21 by abaur             #+#    #+#             */
/*   Updated: 2021/08/20 16:52:39 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <iostream>
#include <cstring>
#include <cerrno>

#include <unistd.h>

namespace ft
{
	static char	defaultresponse[] = 
		"HTTP/1.1 200 OK\n"
		"Server: ft_webserv\n"
		"Accept-Ranges: bytes\n"
		"Content-Length: 50\n"
		"Vary: Accept-Encoding\n"
		"Content-Type: text/plain\n"
		"\n"
		"Beep boop. I am a robot, and I make robot noises.\n"
	;

	Server::Server(int port){
		this->errstatus = 0;
		this->sockfd = -1;
		this->addr.sin_family      = AF_INET;
		this->addr.sin_addr.s_addr = INADDR_ANY;
		this->addr.sin_port        = htons(port);
		
		if (!this->Init())
			std::cout << "Server ready on port " << port << std::endl;
		else
			std::cout << errstatus << ' ' << strerror(errstatus) 
				<< "\tFailed to create server on port " << port 
				<< std::endl;
	}

	Server::~Server(){
		if (this->sockfd != 0)
			close(this->sockfd);
	}


	int	Server::GetErrStatus() const { return this->errstatus; }


	int	Server::Init(){
		int optval = 1;

		if (0 <= (this->sockfd = socket(AF_INET, SOCK_STREAM, 0))
		&&  0 == setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &optval, sizeof(optval))
		&&  0 == bind(this->sockfd, (struct sockaddr*)&this->addr, sizeof(this->addr))
		&&  0 == listen(this->sockfd, 3)
		)
			return 0;
		else
			return this->errstatus = (errno ?: -1);
	}

	void	Server::Accept(){
		socklen_t socklen = sizeof(this->addr);
		int acceptfd = accept(sockfd, (struct sockaddr*)&this->addr, &socklen);
		if (acceptfd < 0) 
		{
			this->errstatus = errno ?: -1;
			return;
		}

		char	inbuffer[1025] = {0};
		ssize_t inlen = read(acceptfd, inbuffer, 1024);
		if (inlen <= 0)
			std::cout << "Nothin received ("<<inlen<<")\n" << std::endl;
		else {
			std::cout << "REQUEST:\n" << inbuffer << '\n' << std::endl;
			send(acceptfd, defaultresponse, strlen(defaultresponse), 0);
		}

		close(acceptfd);
	}

}
