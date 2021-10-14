/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/10/14 15:58:01 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserv.hpp"

#include "clibft/clibft.hpp"
#include "configparser/configparser.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "PollManager.hpp"
#include "SocketPollListener.hpp"
#include "ServerDispatchPollListener.hpp"

typedef std::vector<ft::ServerBlock*>	BlockArray;

typedef std::list<ft::Server>	ServList;
typedef std::list<ft::Socket>	SockList;
typedef std::list<ft::ServerConfig>	ConfList;
typedef std::list<ft::SocketPollListener*>	SockListenerList;

static inline bool	GetConfig(const char* path, ConfList& output)
{
	std::ifstream file(path);
	if (file.fail()) {
		std::cerr << "[FATAL] Unable to read \""<<path<<"\"\n"
			      << errno << '\t' << std::strerror(errno) << std::endl;
		return false;
	}

	BlockArray blocks;
	try {
		blocks = ft::ServerBlock::ParseAll(file);
		for (size_t i=0; i<blocks.size(); i++)
			output.push_back(ft::ServerConfig(*blocks[i]));
		ft::DeleteContent(blocks);
	} 
	catch (ft::InvalidSyntaxException& excp) {
		std::cerr << "[FATAL] Invalid syntax in config file :" << std::endl
			      << excp.what() << std::endl;
		ft::DeleteContent(blocks);
		return false;
	}
	catch (std::exception&) {
		std::cerr << "[FATAL] Unmanaged exception whilst parsing config file." << std::endl;	
		ft::DeleteContent(blocks);
		throw;
	}

	return true;
}

static inline int	CreateServers(const ConfList& configs, SockList& outsockets, ServList& outservers)
{
	std::map<int, bool> socketCreated;

	int i=-1;
	for (ConfList::const_iterator it=configs.begin(); it!=configs.end(); it++) 
	{
		i++;
		// Create servers
		outservers.push_back(ft::Server(*it));

		// Create sockets
		const std::vector<int>& ports = it->ports;
		if (ports.size() == 0)
			std::cerr << "[WARN] No port found on server n°" << i << ". "
			"This server will be unable to answer any request." << std::endl;
		else for (size_t i=0; i<ports.size(); i++)
		if (!socketCreated[ports[i]])
		{
			outsockets.resize(outsockets.size()+1);
			ft::Socket& sock = outsockets.back();
			sock.SetPort(ports[i]);
			sock.Bind();
			if (int err = sock.GetErrStatus()) 
			{
				std::cerr << "[ERR] Unable to bind socket to port " << ports[i] 
				<< ": " << err << ' ' << std::strerror(err) << std::endl;
				outsockets.pop_back();
			}
			else {
				std::cerr << "[INFO] Socket bound on port " << ports[i] << std::endl;
				socketCreated[ports[i]] = true;
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

	const char* confpath = (argc >= 2) ? argv[1] : "./conf/default.conf";
	ConfList configs;
	if (!GetConfig(confpath, configs))
		return EXIT_FAILURE;


	SockList sockets;
	if (0 == CreateServers(configs, sockets, ft::Server::availableServers)){
		std::cerr << "[FATAL] No socket was able to be created." << std::endl;
		return EXIT_FAILURE;
	}

	SockListenerList sockListeners;
	for (SockList::iterator it=sockets.begin(); it!=sockets.end(); it++)
		ft::PollManager::AddListener(*new ft::SocketPollListener(*it));

	ft::PollManager::PollLoop();
	abort();
}
