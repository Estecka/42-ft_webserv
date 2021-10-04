/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 13:39:14 by abaur             #+#    #+#             */
/*   Updated: 2021/10/04 13:51:34 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGILAUNCHER_HPP
#define CGILAUNCHER_HPP

#include "HttpRequest.hpp"

namespace ft
{
	/**
	 * Sends the given request to a CGI for processing.
	 * The CGI should take care of everything from then on, so no job is left to be done outside of the function.
	 * @param CgiPath	The absolute path to the CGI's executable.
	 * @param acceptfd	The file descriptor where to output the response.
	 */
	void	LaunchCGI(const char* CgiPath, int acceptfd, const HttpRequest& request);
} 


#endif