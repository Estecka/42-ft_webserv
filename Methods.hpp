/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:43:42 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/13 15:51:49 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
# define METHODS_HPP

# include <iostream>
# include "includes/webserv.hpp"

namespace ft {

class	Methods
{
	public:

		Methods(std::string method, int acceptfd, const UriConfig& config);
		Methods(const Methods &other);
		~Methods(void);
		Methods	&operator=(const Methods &other);

	private:

		int					_acceptfd;
		std::string			_method;
		const UriConfig&	_config;
};
}
#endif
