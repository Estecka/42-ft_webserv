/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 16:49:25 by abaur             #+#    #+#             */
/*   Updated: 2021/10/01 15:16:00 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

namespace ft
{
	HttpRequest::HttpRequest(const std::string& requestContent){
		std::stringstream input(requestContent);
		this->Parse(input);
	}

	HttpRequest::HttpRequest(std::istream& requestInput){
		this->Parse(requestInput);
	}

	HttpRequest::~HttpRequest(){}


	bool 	HttpRequest::IsOk() const          	{ return this->_ok; }

	short	HttpRequest::GetMajorHttpVersion() const	{ return this->_majorHttpVersion; }
	short	HttpRequest::GetMinorHttpVersion() const	{ return this->_minorHttpVersion; }

	const std::string&	HttpRequest::GetMethod() const     	{ return this->_method;      }
	const std::string&	HttpRequest::GetRequestPath() const	{ return this->_requestPath; }
	const std::string&	HttpRequest::GetQueryString() const	{ return this->_queryString; }
	const std::string&	HttpRequest::GetHost() const    	{ return _properties.at("Host"); }
	const std::string&	HttpRequest::GetHostname() const	{ return this->_hostname;        }
	int               	HttpRequest::GetHostPort() const	{ return this->_port;            }

	bool	HttpRequest::HasProperty(const std::string& name) const {
		return this->_properties.count(name) < 0 && _properties.at(name) != "";
	}
	const std::string&	HttpRequest::operator[](const std::string& name) {
		return this->_properties[name];
	}

/******************************************************************************/
/* ## Parsing                                                                 */
/******************************************************************************/

	bool	HttpRequest::Parse(std::istream& input){
		this->_ok = true;
		if (!ParseFirstLine(input))
			return false;

		while (ParseProperty(input))
			continue;

		std::string& host = this->_properties["Host"];
		if (int sep = ValidateHostFull(host)) {
			this->_hostname = host.substr(0, sep);
			this->_port = std::atoi(host.substr(sep+1).c_str());
		}
		else {
			std::cerr << "[ERR] Request has invalid \"host\" property: " << host << std::endl;
			return this->_ok = false;
		}

		char  	buff[1024] = { 0 };
		while (!input.fail()) {
			input.read (buff, 1024);
			_body.write(buff, input.gcount());
		}

		return this->_ok;
	}

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

		size_t queryPos = this->_requestPath.find('?');
		if (queryPos != std::string::npos) {
			this->_queryString = _requestPath.substr(queryPos);
			this->_requestPath = _requestPath.substr(0, queryPos);
		}
		return true;
	}

	bool	HttpRequest::ParseProperty(std::istream& input){
		std::string	line;
		std::string	name;
		std::string	value;

		std::getline(input, line);
		if (input.fail() || line.empty() || line[0] == '\r')
			return false;

		ExtractWord(line, name);
		value = ft::trim(line);

		if (name[name.length()-1] != ':'){
			std::cerr << "[WARN] Missing ':' separator: " << name << std::endl;
			return true;
		}
		name = name.substr(0, name.length()-1);
		if (!ValidatePropertyName(name)) {
			std::cerr << "[WARN] Invalid property name: " << name << std::endl;
			return true;
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
	int	HttpRequest::ValidateHostFull(const std::string& host){
		size_t sep = 0;

		if (host.length() == 0)
			return 0;

		for (size_t i=0; i<host.length(); i++){
			char c = host[i];
			if (c == ':') {
				sep = i;
				break;
			}
			else if (isalnum(c) || c == '.' || c =='-')
				continue;
			else
				return 0;
		}

		if (sep <= 0 || sep+1 == host.length())
			return 0;

		for (size_t i=sep+1; i<host.length(); i++)
			if (!std::isdigit(host[i]))
				return 0;

		return sep;
	}

} // End Namespace

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
