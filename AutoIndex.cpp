/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 10:45:19 by apitoise          #+#    #+#             */
/*   Updated: 2021/09/28 14:13:56 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"

AutoIndex::AutoIndex(const ft::HttpRequest& req, int acceptfd, std::string path): _dirName(req.GetRequestPath()) {
	DIR				*dir;
	struct dirent	*ent;
	ft::HttpHeader		header(200, ".html");

	if ((dir = opendir(path.c_str())) != NULL) {
		_index = \
		"<!DOCTYPE html>\n\
		<html>\n\
		<head>\n\
		<title>" + _dirName + "</title>\n\
		</head>\n\
		<body>\n\
		<h1>Index</h1><p>\n";
		while ((ent = readdir(dir)) != NULL) {
			std::string	inDirFile = ent->d_name;
			_index += \
			"<a href=\"" + _dirName + "/" +  inDirFile + "\">" + inDirFile + "</a><br>\n";
		}
		_index += \
		"<br><br>\n\
		</p>\n\
		<hr>\n\
		<p>\n\
			webserv | apitoise | abaur<br>\n\
		</p>\n\
		</body>\n\
		</html>\n\
		";
		closedir(dir);
		send(acceptfd, header.ToString().c_str(), header.ToString().size(), 0);
		send(acceptfd, _index.c_str(), _index.size(), 0);
	}
	else
		std::cerr << "[WARN] CANNOT OPEN THIS DIRECTORY" << std::endl;
}

AutoIndex::AutoIndex(const AutoIndex &other) { (void)other; }

AutoIndex::~AutoIndex(void) {}

AutoIndex	&AutoIndex::operator=(const AutoIndex& other) {
	(void)other;
	return (*this);
}
