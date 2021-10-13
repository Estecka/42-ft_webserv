/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:16:21 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/13 15:29:17 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
# define ERRORPAGE_HPP

# include "RequestHandler.hpp"
# include "includes/webserv.hpp"
# include "HttpHeader.hpp"

namespace	ft {

class	ErrorPage: public IPollListener
{
	public:

		ErrorPage(int code, int acceptfd, RequestHandler& parent);
		ErrorPage(const ErrorPage &other);
		~ErrorPage(void);
		ErrorPage	&operator=(const ErrorPage &other);
		void		GetPollFd(pollfd& poll_fd);
		void		OnPollEvent(const pollfd&);

	private:

		int					_code;
		std::stringstream	_page;
		std::string			_title;
		std::string			_msg;
		int					_acceptfd;
		RequestHandler&		_parent;

		void	SetPage();
};
}
#endif
