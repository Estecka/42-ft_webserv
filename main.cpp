/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/10/23 16:08:03 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configparser/configparser.hpp"
#include "logutil/logger.hpp"
#include "PollManager.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "SocketPollListener.hpp"
#include "TimeoutManager.hpp"

typedef std::vector<ft::ServerBlock*>	BlockArray;

typedef std::list<ft::Server>	ServList;
typedef std::list<ft::Socket>	SockList;
typedef std::list<ft::ServerConfig>	ConfList;

static inline bool	GetConfig(const char* path, ConfList& output)
{
	std::ifstream file(path);
	if (file.fail()) {
		ft::clog << "[FATAL] Unable to read \""<<path<<"\"\n"
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
		ft::clog << "[FATAL] Invalid syntax in config file :" << std::endl
			      << excp.what() << std::endl;
		ft::DeleteContent(blocks);
		return false;
	}
	catch (std::exception&) {
		ft::clog << "[FATAL] Unmanaged exception whilst parsing config file." << std::endl;	
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
			ft::clog << "[WARN] No port found on server n°" << i << ". "
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
				ft::clog << "[ERR] Unable to bind socket to port " << ports[i] 
				<< ": " << err << ' ' << std::strerror(err) << std::endl;
				outsockets.pop_back();
			}
			else {
				ft::clog << "[INFO] Socket bound on port " << ports[i] << std::endl;
				socketCreated[ports[i]] = true;
			}
		}

	}

	return outsockets.size();
}

extern int	main(int argc, char** argv)
{
	if (argc > 2) {
		ft::clog << "[ERR] Too many arguments" << std::endl;
		return EXIT_FAILURE;
	}

	const char* confpath = (argc >= 2) ? argv[1] : "./conf/default.conf";
	ConfList configs;
	if (!GetConfig(confpath, configs))
		return EXIT_FAILURE;


	SockList sockets;
	if (0 == CreateServers(configs, sockets, ft::Server::availableServers)){
		ft::clog << "[FATAL] No socket was able to be created." << std::endl;
		return EXIT_FAILURE;
	}

	for (SockList::iterator it=sockets.begin(); it!=sockets.end(); it++)
		ft::PollManager::AddListener(*new ft::SocketPollListener(*it));

	try 
	{
		bool event = true;
		while (true)
		{
			if (event)
				ft::clog << "[INFO] Awaiting Poll..." << std::endl;
			event = false;
			event |= ft::PollManager::PollLoop(5);
			event |= ft::TimeoutManager::TimeLoop();
		}
	}
	catch (const ft::CleanExitException& e) {
		ft::PollManager::DeleteAll();
		exit(e.GetStatus());
	}
}
