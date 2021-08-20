/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 16:49:25 by abaur             #+#    #+#             */
/*   Updated: 2021/08/21 17:28:39 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

#include "clibft.hpp"

#include <sstream>
#include <cstring>

namespace ft
{
	HttpRequest::HttpRequest(const std::string& requestContent){
		std::stringstream input(requestContent);
		new(this) HttpRequest(input);
	}

	HttpRequest::HttpRequest(std::istream& requestInput){
		this->_ok = true;
		if (ParseFirstLine(requestInput))
		while (!requestInput.eof())
			ParseProperty(requestInput);
	}

	HttpRequest::~HttpRequest(){}


	bool	HttpRequest::IsOk() const { return this->_ok; }

	bool	HttpRequest::HasProperty(const std::string& name) const {
		return this->_properties.count(name) < 0 && _properties.at(name) != "";
	}
	const std::string&	HttpRequest::operator[](const std::string& name) {
		return this->_properties[name];
	}

/******************************************************************************/
/* ## Parsing                                                                 */
/******************************************************************************/

	bool	HttpRequest::ParseFirstLine(std::istream& input){
		std::string	line;
		std::string	versionFull;

		std::getline(input, line);
		if (!ExtractWord(line, this->_method)
		||  !ExtractWord(line, this->_requestPath)
		||  !ExtractWord(line, versionFull)
		) {
			std::cerr << "[ERR] Request is missing a component." << std::endl;
			return this->_ok = false;
		}
		if ("" != ft::trim(line)){
			std::cerr << "[ERR] Request has extraneous component." << std::endl;
			return this->_ok = false;
		}

		if (!this->ValidateMethod()
		||  !this->ValidatePath()
		||  !ValidateVersionFull(versionFull)
		) {
			std::cerr << "[ERR] Request has invalid component." << std::endl;
			return this->_ok = false;
		}

		this->_majorHttpVersion = versionFull[5] - '0';
		this->_minorHttpVersion = versionFull[7] - '0';
		return true;
	}

	bool	HttpRequest::ParseProperty(std::istream& input){
		std::string	line;
		std::string	name;
		std::string	value;

		std::getline(input, line);
		if (line.empty() || line[0] == '\r')
			return false;

		ExtractWord(line, name);
		value = ft::trim(line);

		if (name[name.length()-1] != ':'){
			std::cerr << "[WARN] Missing ':' separator: " << name << std::endl;
			return false;
		}
		name = name.substr(0, name.length()-1);
		if (!ValidatePropertyName(name)) {
			std::cerr << "[WARN] Invalid property name: " << name << std::endl;
			return false;
		}

		if (this->HasProperty(name))
			std::cerr << "[WARN] Duplicate property: " << name << std::endl;
		this->_properties[name] = value;
		return true;
	}

	bool	HttpRequest::ExtractWord(std::string& line, std::string& output){
		size_t	begin = 0;
		size_t	len = 0;

		size_t	i = 0;
		for(; i<line.length(); i++)
			if (isspace(line[i]))
				begin++;
			else
				break;
		for(; i<line.length(); i++)
			if (!isspace(line[i]))
				len++;
			else
				break;

		if (len==0)
			return false;
		output = line.substr(begin, len);
		begin += len;
		len = line.length() - begin;
		line = line.substr(begin, len);
		return true;
	}

	bool	HttpRequest::ValidateMethod() const {
		if (this->_method.empty())
			return false;

		for (size_t i=0; i<this->_method.length(); i++)
			if (!isupper(_method[i]))
				return false;

		return true;
	}
	bool	HttpRequest::ValidatePath() const {
		if (this->_requestPath.empty()
		||  this->_requestPath[0] != '/')
			return false;

		return true;
	}
	bool	HttpRequest::ValidateVersionFull(const std::string& version) {
		if (version.length() != 8)
			return false;

		if (strncmp("HTTP/", version.c_str(), 5))
			return false;

		if (!isdigit(version[5])
		||  version[6] != '.'
		||  !isdigit(version[7])
		)
			return false;

		return true;
	}
	bool	HttpRequest::ValidatePropertyName(const std::string& name){
		for (size_t i=0; i<name.length(); i++) 
			if (!isalpha(name[i])
			&&  name[i] != '-')
				return false;

		return true;
	}
}

std::ostream&	operator<<(std::ostream& out, const ft::HttpRequest& src){
	out << src._method << '\t'
		<< src._requestPath << '\t'
		<< "HTTP/" << src._majorHttpVersion<<'.'<<src._minorHttpVersion
		<< std::endl
		;

	for ( std::map<std::string, std::string>::const_iterator it=src._properties.begin();
		it!=src._properties.end();
		it++ ) 
	{
		out << it->first << ":\t" 
			<< it->second << std::endl;
	}

	return out;
}
