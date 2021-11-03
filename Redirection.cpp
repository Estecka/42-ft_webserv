/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redirection.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 16:14:44 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/02 16:33:17 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Redirection.hpp"
#include "ResponseHeader.hpp"
#include "logutil/logutil.hpp"

namespace	ft {

	Redirection::Redirection(const UriConfig& config, int fd, RequestHandler& parent): _config(config), _fd(fd), _parent(parent) {
		ft::clog << log::info << "Redirection created." << std::endl;
	}

	Redirection::~Redirection(void) {
		ft::clog << log::info << "Redirection destroyed." << std::endl;
	}

	void	Redirection::GetPollFd(pollfd& outpfd) {
		outpfd.fd = _fd;
		outpfd.events = POLLOUT;
	}

	void	Redirection::OnPollEvent(const pollfd&) {
		ResponseHeader	header(_config.returnCode != 0 ? _config.returnCode : 303, ".html");
		std::string		page;
	
		header.SetLocation(_config.returnPath);
	
		page = header.ToString();
		ft::clog << log::info << "ICI" << std::endl;
		while (true) {
			std::size_t	len = write(_fd, page.c_str(), page.size());
			if (len < 0)
				return ;
			else if (len < page.size())
				page = page.substr(len);
			else
				break;
		}
		return _parent.Destroy();
	}

}
