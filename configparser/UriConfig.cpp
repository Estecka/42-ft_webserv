/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriConfig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 15:24:10 by abaur             #+#    #+#             */
/*   Updated: 2021/11/12 17:47:44 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UriConfig.hpp"

#include "../clibft/clibft.hpp"
#include "../logutil/logutil.hpp"
#include <iostream>
#include <cstdlib>

using namespace ft;

static bool	UriBaseMatch(const std::string& uri, const std::string& loc) {
	size_t locLen = loc.length();
	size_t uriLen = uri.length();

	if (uriLen == locLen)
		return uri == loc;
	// else if (uriLen+1 == locLen) // Matches location "/foo/" with "/foo"
	// 	return uri+'/' == loc;
	else if (locLen < uriLen) // Matches location "/foo" with "/foo/bar", but not with "/foobar"
		return (uri.substr(0, locLen) == loc)
		    && (loc[locLen-1] == '/' || uri[locLen] == '/') 
			;
	else
		return false;
}

static bool	UriExtensionMatch(const std::string& uri, const std::string& loc){
	size_t uriextpos = uri.rfind('.');
	if (uriextpos == std::string::npos)
		return false;

	if (loc.size() < 3 || loc[0]!='*' || loc[1]!='.') {
		ft::clog << log::error << "Invalid extension expression: " << loc << std::endl;
		return false;
	}

	return uri.substr(uriextpos) == loc.substr(1);
}

namespace ft
{
	UriConfig::UriConfig() {
		this->handle.prefix = 0;
		this->handle.path   = "";
		this->root      = "";
		this->upload_path = "";
		this->body_limit = ~0;
		this->autoindex = false;
		this->returnCode = 0;
		this->returnPath = "";
		this->index.clear();
		this->methods.clear();
		this->error_page.clear();
	}

	bool	UriConfig::UriMatchHandle(const std::string& uri, const LocationHandle& handle){
		switch (handle.prefix)
		{
			case  0 :	return UriBaseMatch(uri, handle.path);
			case '~':	return UriExtensionMatch(uri, handle.path);
			case '=':	return uri == handle.path;
			default:
				ft::clog << log::error << "Invalid location prefix: " << handle.prefix << std::endl;
				return false;
		}
	}



	/**************************************************************************/
	/* # Data validation                                                      */
	/**************************************************************************/

	void UriConfig::AddProperties(const PropertyList& properties){
		for (PropertyList::const_iterator it=properties.begin(); it!=properties.end(); it++) {
			if (it->first == "root")
				this->ParseRoot(it->second);
			else if (it->first == "upload_path")
				this->ParseUploadPath(it->second);
			else if (it->first == "autoindex")
				this->ParseAutoindex(it->second);
			else if (it->first == "index")
				this->ParseIndex(it->second);
			else if (it->first == "return")
				this->ParseReturn(it->second);
			else if (it->first == "cgi")
				this->ParseCgi(it->second);
			else if (it->first == "cgi_path")
				this->cgiPath = it->second;
			else if (it->first == "allow_methods")
				this->ParseMethods(it->second);
			else if (it->first == "error_page")
				this->ParseErrorPage(it->second);
			else if (it->first == "body_limit")
				this->ParseBodyLimit(it->second);
			else
				ft::clog << log::warning << "Unknown instruction name: " << it->first << std::endl;
		}
	}

	void	UriConfig::ParseRoot(const std::string& raw) {
		// TODO:
		// Here would be a good place to concatenate the root with the current
		// working directory.
		this->root = raw;
		if (this->handle.prefix == 0)
			this->rootedUri = handle.path;
	}

	void	UriConfig::ParseUploadPath(const std::string& raw) {
		this->upload_path = raw;
	}

	void	UriConfig::ParseAutoindex(const std::string& raw) {
		if (raw == "on")
			this->autoindex = true;
		else if (raw == "off")
			this->autoindex = false;
		else
			ft::clog << log::warning << "Invalid autoindex value: " << raw << std::endl;
	}

	void	UriConfig::ParseIndex(const std::string& raw) {
		std::string	word, remain;

		remain = raw;
		this->index.clear();
		while (ExtractWord(remain, word, remain)) {
			this->index.push_back(word);
		}
	}

	void	UriConfig::ParseMethods(const std::string& raw) {
		std::string	word, remain;

		remain = raw;
		this->methods.clear();
		while (ExtractWord(remain, word, remain)) {
			this->methods.push_back(word);
		}
	}

	void	UriConfig::ParseReturn(const std::string& raw){
		std::string code, path;

		ExtractWord(raw, code, path);

		if (code.empty()) {
			ft::clog << log::warning << "Empty return code" << std::endl;
			return;
		}

		for (size_t i=0; i<code.length(); i++)
		if (!std::isdigit(code[i])) {
			ft::clog << log::warning << "Invalid return code: " << code << std::endl;
			return;
		}

		this->returnCode = std::atoi(code.c_str());
		this->returnPath = path;
	}

	void	UriConfig::ParseErrorPage(const std::string& raw) {
		std::string	code, path;
		
		ExtractWord(raw, code, path);
		if (code.empty()) {
			ft::clog << log::warning << "Empty error code" << std::endl;
			return;
		}
		for (size_t i = 0; i < code.length(); i++) {
			if (!std::isdigit(code[i])) {
				ft::clog << log::warning << "Invalid error code: " << code << std::endl;
				return;
			}
		}
		error_page.insert(std::make_pair(std::atoi(code.c_str()), path));
	}

	void	UriConfig::ParseCgi(const std::string& raw){
		std::string extension, path;

		ExtractWord(raw, extension, path);
		if (extension.empty())
			ft::clog << log::warning << "Empty cgi directive" << std::endl;
		else if (path.empty())
			this->cgis.erase(extension);
		else
			this->cgis[extension] = path;
	}

	void	UriConfig::ParseBodyLimit(const std::string& raw){
		if (raw.empty()){
			this->body_limit = ~0;
			return;
		}

		char* endptr;
		size_t r = std::strtoul(raw.c_str(), &endptr, 10);

		char unit;
		char metric;
		if (endptr[0] == '\0'){
			metric = 0;
			unit   = 'b';
		} else if (endptr[1] == '\0') {
			metric = 0;
			unit   = endptr[0];
		} else if (endptr[2] == '\0'){
			metric = endptr[0];
			unit   = endptr[1];
		} else
			goto invalidformat;

		switch (metric) {
			default: 	goto invalidformat;
			case 'G':	r *= 1000000000; break;
			case 'm':	r *= 1000000; break;
			case 'k':	r *= 1000; break;
			case 0:  	break;
		}

		switch (unit) {
			default: 	goto invalidformat;
			case 'b':	r /= 8; break;
			case 'o':	break;
			case 'B':	break;
			case 0:  	break;
		}

		this->body_limit = r;
		return;

		invalidformat:
		ft::clog << log::warning << "Invalid size format: " << raw << std::endl;
	}

}
