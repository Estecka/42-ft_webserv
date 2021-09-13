/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/09/13 14:55:45 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include "configparser/configparser.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <cerrno>
#include <cstdlib>
#include <cstring>

typedef std::vector<ft::ServerConfig*>	ConfArray;
typedef std::vector<ft::Server*>      	ServArray;

static inline bool	GetConfig(const char* path, ConfArray& output)
{
	std::ifstream file(path);
	if (file.fail()) {
		std::cerr << "[FATAL] Unable to read \""<<path<<"\"\n"
			      << errno << '\t' << std::strerror(errno) << std::endl;
		return false;
	}

	try {
		output = ft::ServerConfig::ParseAll(file);
	} 
	catch (ft::InvalidSyntaxException& excp) {
		std::cerr << "[FATAL] Invalid syntax in config file :" << std::endl
			      << excp.what() << std::endl;
		return false;
	}
	catch (std::exception&) {
		std::cerr << "[FATAL] Unmanaged exception whilst parsing config file." << std::endl;		
		throw;
	}

	return true;
}

static inline int	CreateServers(const ConfArray& configs, ServArray& outservers)
{
	outservers.reserve(configs.size());
	for (size_t i=0; i<configs.size(); i++) {
		int port = configs[i]->GetPort();
		if (!port) {
			std::cerr << "[ERR] No port valid port set on server n°" << i << std::endl;
			continue;
		}

		ft::Server& serv = *new ft::Server(port);
		if (int err = serv.GetErrStatus()) {
			std::cerr << "[ERR] Unable to start server n°" << i << " on port " << port << '\n'
			          << err << '\t' << std::strerror(err) << std::endl;
			delete &serv;
			continue;
		}
		else
			outservers.push_back(&serv);
	}

	return outservers.size();
}

extern int	main(int argc, char** argv)
{
	if (argc > 2) {
		std::cerr << "Too many arguments" << std::endl;
		return EXIT_FAILURE;
	}

	const char* confpath = (argc >= 2) ? argv[1] : "./default.conf";
	ConfArray configs;
	if (!GetConfig(confpath, configs))
		return EXIT_FAILURE;


	ServArray servers;
	if (0 == CreateServers(configs, servers)){
		std::cerr << "[ERR] No server was started." << std::endl;
		return EXIT_FAILURE;
	}

	while(1)
	for (size_t i=0; i<servers.size(); i++)
		servers[i]->Accept();
}
