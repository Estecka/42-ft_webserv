/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestReadPollListener.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 17:13:38 by abaur             #+#    #+#             */
/*   Updated: 2021/09/21 18:52:48 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestReadPollListener.hpp"

#include "HttpRequest.hpp"
#include "PollManager.hpp"
#include "ServerDispatchPollListener.hpp"

#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>

namespace ft
{
	RequestReadPollListener::RequestReadPollListener(int acceptfd, int port) {
		this->acceptfd = acceptfd;
		this->port     = port;
	}
	RequestReadPollListener::~RequestReadPollListener(){
	}

	void	RequestReadPollListener::GetPollFd(struct pollfd& outpfd){
		outpfd.fd      = this->acceptfd;
		outpfd.events  = POLLIN;
		outpfd.revents = 0;
	}

	void	RequestReadPollListener::OnPollEvent(const struct pollfd&) {
		PollManager::RemoveListener(*this);

		std::stringstream	input;
		char buffer[1025] = {0};
		ssize_t bufflen;

		fcntl(acceptfd, F_SETFL, O_NONBLOCK);
		while(0 < (bufflen = read(acceptfd, buffer, 1024))) {
			buffer[bufflen] = '\0';
			input << buffer;
		}

		ServerDispatchPollListener& response = *new ServerDispatchPollListener(acceptfd, this->port, input.str());
		PollManager::AddListener(response);
		delete this;
	}

}
