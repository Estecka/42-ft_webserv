/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/09/14 16:39:47 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

#include "configparser/configparser.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <cerrno>
#include <cstdlib>
#include <cstring>

typedef std::vector<ft::ServerConfig*>	ConfArray;
typedef std::map<int, ft::Socket>     	SockMap;

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

static inline int	CreateSockets(const ConfArray& configs, SockMap& outsockets)
{
	for (size_t i=0; i<configs.size(); i++) 	{
		int port = configs[i]->GetPort();

		if (!port)
			std::cerr << "[ERR] No port valid port set on server n°" << i << std::endl;
		else if (outsockets.count(port))
			std::cerr << "[WARN] Server n°" << i << " is set to listen to port "
			          << port << ", but this port is already in use." << std::endl;
		else {
			ft::Socket& sock = outsockets[port];
			sock.SetPort(port);
			sock.Bind();
			if (int err = sock.GetErrStatus())
				std::cerr << "[ERR] Unable to bind socket to port " << port << " :\n"
				          << "      " << err << ' ' << std::strerror(err) << std::endl;
			else
				std::cout << "[INFO] Socket bound on port " << port << std::endl;
		}
	}

	return outsockets.size();
}

extern int	main(int argc, char** argv)
{
	if (argc > 2) {
		std::cerr << "[ERR] Too many arguments" << std::endl;
		return EXIT_FAILURE;
	}

	const char* confpath = (argc >= 2) ? argv[1] : "./default.conf";
	ConfArray configs;
	if (!GetConfig(confpath, configs))
		return EXIT_FAILURE;


	SockMap sockets;
	if (0 == CreateSockets(configs, sockets)){
		std::cerr << "[FATAL] No socket was able to be created." << std::endl;
		return EXIT_FAILURE;
	}

	while(1)
	for (SockMap::iterator it=sockets.begin(); it!=sockets.end(); it++)
		it->second.Accept();
}
