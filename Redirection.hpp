/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redirection.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 16:14:44 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/02 16:26:47 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_HPP
# define REDIRECTION_HPP

# include "includes/webserv.hpp"
# include "IPollListener.hpp"
# include "configparser/UriConfig.hpp"
# include "RequestHandler.hpp"

namespace	ft {

class	Redirection : public IPollListener
{
	public:

		Redirection(const UriConfig& config, int fd, RequestHandler& parent);
		~Redirection(void);

		void	GetPollFd(pollfd& outpfd);
		void	OnPollEvent(const pollfd&);
	
	private:

		const UriConfig&	_config;
		int					_fd;
		RequestHandler&		_parent;
};

}
#endif
