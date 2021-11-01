/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 18:01:21 by abaur             #+#    #+#             */
/*   Updated: 2021/10/31 20:01:46 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

#include <stdexcept>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>

namespace ft
{
/******************************************************************************/
/* # Constructors                                                             */
/******************************************************************************/

	Socket::Socket() {
		this->isBound   = false;
		this->errstatus = 0;
		this->port = -1;
		this->sockfd = -1;
		this->addr.sin_family      = AF_INET;
		this->addr.sin_addr.s_addr = INADDR_ANY;
		this->addr.sin_port        = 0;
	}

	Socket::Socket(int port) {
		new(this) Socket();
		this->SetPort(port);
		this->Bind();
	}

	Socket::~Socket(){
		if (this->IsBound())
			close(this->sockfd);
		this->isBound = 0;
		this->sockfd  = 0;
	}


/******************************************************************************/
/* # Accessors                                                                */
/******************************************************************************/

	bool	Socket::IsBound()      const { return this->isBound; }
	int 	Socket::GetErrStatus() const { return this->errstatus; }
	int 	Socket::GetPort()      const { return this->port; }
	int 	Socket::GetSocketFD()  const { return this->sockfd; }

	void	Socket::SetPort(int port) {
		if (this->IsBound())
			throw std::logic_error("Cannot change the port of a bound socket.");
		this->port = port;
		this->addr.sin_port = htons(port);
	}


/******************************************************************************/
/* # Methods                                                                  */
/******************************************************************************/

	int	Socket::Bind(){
		int optval = 1;
		this->errstatus = 0;

		if (0 <= (this->sockfd = socket(AF_INET, SOCK_STREAM, 0))
		&&  0 == setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &optval, sizeof(optval))
		&&  0 == bind(this->sockfd, (struct sockaddr*)&this->addr, sizeof(this->addr))
		&&  0 == listen(this->sockfd, 3)
		) {
			this->isBound = true;
			return 0;
		}
		else
			return this->errstatus = (errno ?: -1);
	}

	Socket::fd_ip	Socket::Accept() {
		socklen_t	socklen = sizeof(this->addr);
		fd_ip		ret;

		ret.acceptfd = accept(sockfd, (struct sockaddr*)&this->addr, &socklen);
		if (ret.acceptfd < 0) 
		{
			this->errstatus = errno ?: -1;
			ret.acceptfd = -1;
			return ret;
		}
		inet_ntop(AF_INET, &(this->addr.sin_addr.s_addr), ret.ip, INET_ADDRSTRLEN);
		return ret;
	}

}
