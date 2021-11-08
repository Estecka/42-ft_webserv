/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 13:39:14 by abaur             #+#    #+#             */
/*   Updated: 2021/11/08 11:44:10 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGILAUNCHER_HPP
#define CGILAUNCHER_HPP

#include "RequestHandler.hpp"
#include "configparser/UriConfig.hpp"

namespace ft
{
	/**
	 * Sends the given request to a CGI for processing.
	 * @param outPid 	Outputs the pid of the cgi.
	 * @param outPipe	Outputs a file descriptor from which to read the cgi's output.
	 * @throw 	ft::ErrnoException, if the Cgi couldn't be started.
	 */
	void	LaunchCGI(RequestHandler& request, pid_t& outPid, int& outPipe, std::string cgiPath, std::string reqPath);
} 


#endif
