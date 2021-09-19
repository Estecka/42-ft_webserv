/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/09/19 17:41:22 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "configparser/configparser.hpp"

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <poll.h>
#include <stdnoreturn.h>

typedef std::vector<ft::ServerConfig*>	ConfArray;
typedef std::list<ft::Server>	ServList;
typedef std::list<ft::Socket>	SockList;

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

static inline int	CreateServers(const ConfArray& configs, SockList& outsockets, ServList& outservers)
{
	std::map<int, bool> socketCreated;

	for (size_t i=0; i<configs.size(); i++) 
	{
		// Create servers
		outservers.resize(outservers.size()+1);
		ft::Server& serv = outservers.back();
		serv.SetConfig(*configs[i]);

		// Create sockets
		int port = configs[i]->GetPort();
		if (!port)
			std::cerr << "[ERR] No port valid port found on server n°" << i << std::endl;
		else if (!socketCreated[port])
		{
			outsockets.resize(outsockets.size()+1);
			ft::Socket& sock = outsockets.back();
			sock.SetPort(port);
			sock.Bind();
			if (int err = sock.GetErrStatus()) 
			{
				std::cerr << "[ERR] Unable to bind socket to port " << port << ": "
				          << err << ' ' << std::strerror(err) << std::endl;
				outsockets.pop_back();
			}
			else {
				std::cerr << "[INFO] Socket bound on port " << port << std::endl;
				socketCreated[port] = true;
			}
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


	SockList sockets;
	ServList servers;
	if (0 == CreateServers(configs, sockets, servers)){
		std::cerr << "[FATAL] No socket was able to be created." << std::endl;
		return EXIT_FAILURE;
	}

	for (SockList::iterator it=sockets.begin(); it!=sockets.end(); it++)
		ft::PollManager::AddSocket(*it);

	for (ServList::iterator it=servers.begin(); it!=servers.end(); it++)
		ft::PollManager::AddServer(*it);

	ft::PollManager::PollLoop();
	abort();
}
