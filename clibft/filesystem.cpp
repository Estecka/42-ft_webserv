/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesystem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 19:34:44 by abaur             #+#    #+#             */
/*   Updated: 2021/11/02 18:09:04 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filesystem.hpp"

#include <sys/stat.h>
#include "../includes/webserv.hpp"

namespace ft
{
	bool	IsDir(const std::string path, bool slash) {
		struct stat	statbuf;

		stat(path.c_str(), &statbuf);
		if (S_ISDIR(statbuf.st_mode) && !slash)
			return true;
		if (S_ISDIR(statbuf.st_mode) && slash && path[path.size() - 1] == '/')
			return true;
		return false;
	}

	bool	IsFile(const std::string path) {
		if (FILE* file = fopen(path.c_str(), "r")) {
			fclose(file);
			return true;
		}
		else
			return false;
	}
}
