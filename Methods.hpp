/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 07:05:15 by apitoise          #+#    #+#             */
/*   Updated: 2021/11/03 07:05:17 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
# define METHODS_HPP

# include "includes/webserv.hpp"
# include "RequestHeader.hpp"
# include "configparser/UriConfig.hpp"
# include "IPollListener.hpp"
# include "RequestHandler.hpp"

namespace ft {

	IPollListener*	Methods(const UriConfig& conf, const RequestHeader& req, int fd, RequestHandler& parent, FILE* body);

}
#endif
