/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 13:25:39 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/29 16:44:30 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserv.hpp"

bool	IsDir(const std::string path, bool slash) {
	struct stat	statbuf;

	stat(path.c_str(), &statbuf);
	if (S_ISDIR(statbuf.st_mode) && !slash)
		return true;
	if (S_ISDIR(statbuf.st_mode) && slash && path[path.size() - 1] == '/')
		return true;
	return false;
}
