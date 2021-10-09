/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:10:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/09 19:57:46 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include <cstdlib>

namespace ft
{
	RequestHandler::RequestHandler(int _acceptfd)
		: httpin(_acceptfd),
		httpout (_acceptfd)
	{
		fcntl(_acceptfd, F_SETFL, O_NONBLOCK);
		this->PollInit();
	}

	RequestHandler::~RequestHandler()
	{
		if (this->_header)
			delete _header;
	}


	void	RequestHandler::GetPollFd(pollfd& outpfd) {
		outpfd = this->_pollfd;
	}
	void	RequestHandler::OnPollEvent(const pollfd& pfd) {
		(this->*_onPollEvent)(pfd);
	}
}