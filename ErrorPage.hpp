/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:16:21 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/01 14:14:04 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
# define ERRORPAGE_HPP

# include "includes/webserv.hpp"
# include "HttpHeader.hpp"

class	ErrorPage
{
	public:

		ErrorPage(int code, int acceptfd);
		ErrorPage(const ErrorPage &other);
		~ErrorPage(void);
		ErrorPage	&operator=(const ErrorPage &other);

	private:

		int			_code;
		std::string	_page;
		std::string	_title;
		std::string	_msg;

		void	SetPage(int code);
		void	SendPage(int acceptfd);
};

#endif
