/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesystem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 19:34:44 by abaur             #+#    #+#             */
/*   Updated: 2021/11/05 09:38:56 by apitoise         ###   ########.fr       */
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

	int		RecursiveMkdir(std::string path, size_t start) {
	
		int			ret;
		std::string	dirToCheck;
		
		if (start >= path.size())
			return 0;
		else
			dirToCheck = path.substr(0, path.find("/", start));
		if (IsDir(dirToCheck, false)) {
			return RecursiveMkdir(path, path.find("/", start) + 1);
		}
		if ((ret = mkdir(dirToCheck.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
			return ret;
		return RecursiveMkdir(path, path.find("/", start) + 1);
	}

}
