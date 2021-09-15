/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/09/15 16:17:31 by abaur            ###   ########.fr       */
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

#include <poll.h>
#include <stdnoreturn.h>

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
			if (int err = sock.GetErrStatus()) {
				std::cerr << "[ERR] Unable to bind socket to port " << port << " :\n"
				          << "      " << err << ' ' << std::strerror(err) << std::endl;
				outsockets.erase(port);
			}
			else
				std::cout << "[INFO] Socket bound on port " << port << std::endl;
		}
	}

	return outsockets.size();
}

static inline noreturn void	RunSockets(SockMap& ports){
	int          	nfd = ports.size();
	struct pollfd	pollfds[nfd];
	std::map<int, ft::Socket*> sockets;

	int i=0;
	for (SockMap::iterator it=ports.begin(); it!=ports.end(); it++, i++){
		ft::Socket& s = it->second;
		sockets[s.GetSocketFD()] = &s;
		pollfds[i].fd = s.GetSocketFD();
		pollfds[i].events = POLLIN;
		pollfds[i].revents = 0;
	}

	while (true) {
		int r = poll(pollfds, nfd, -1);
		if (r < 0) {
			std::cerr << "[FATAL] Poll error" << '\n'
			          << "        " << errno << ' ' << std::strerror(errno) << std::endl;
			abort();
 		}
		else for (int i=0; i<nfd; i++) {
			struct pollfd& pfd = pollfds[i];
			if (pfd.revents & POLLIN)
				sockets[pfd.fd]->Accept();
		}
	}
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

	RunSockets(sockets);
}
