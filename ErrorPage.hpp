/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:16:21 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/11 17:47:24 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
# define ERRORPAGE_HPP

# include "RequestHandler.hpp"
# include "includes/webserv.hpp"
# include "ResponseHeader.hpp"

namespace	ft {

class	ErrorPage: public IPollListener
{
	public:

		ErrorPage(int code, int acceptfd, RequestHandler& parent);
		ErrorPage(int code, int acceptfd, RequestHandler& parent, const std::string& msg);
		ErrorPage(const ErrorPage &other);
		~ErrorPage(void);
		ErrorPage	&operator=(const ErrorPage &other);
		void		GetPollFd(pollfd& poll_fd);
		void		OnPollEvent(const pollfd&);

	private:

		std::string			_strPage;
		int					_acceptfd;
		RequestHandler&		_parent;

		void	SetPage(int code, const char* title, const char* msg);
};
}
#endif
