/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 10:45:19 by apitoise          #+#    #+#             */
/*   Updated: 2021/09/28 13:52:42 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "includes/webserv.hpp"
# include "HttpRequest.hpp"
# include "HttpHeader.hpp"

class	AutoIndex
{
	public:

		AutoIndex(const ft::HttpRequest& req, int acceptfd, std::string path);
		AutoIndex(const AutoIndex &other);
		~AutoIndex(void);
		AutoIndex	&operator=(const AutoIndex &other);

	private:
	
		std::string	_dirName;
		std::string	_index;
};

#endif
