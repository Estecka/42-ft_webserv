/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 16:49:48 by abaur             #+#    #+#             */
/*   Updated: 2021/10/30 14:36:30 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clibft/PrepackedExecve.hpp"
#include "clibft/CleanExitException.hpp"
#include "configparser/configparser.hpp"
#include "logutil/logutil.hpp"
#include "PollManager.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "SocketPollListener.hpp"
#include "TimeoutManager.hpp"

using namespace ft;

typedef std::vector<ft::ServerBlock*>	BlockArray;

typedef std::list<ft::Server>	ServList;
typedef std::list<ft::Socket>	SockList;
typedef std::list<ft::ServerConfig>	ConfList;

static inline bool	GetConfig(const char* path, ConfList& output)
{
	std::ifstream file(path);
	if (file.fail()) {
		ft::clog << log::fatal << "Unable to read \""<<path<<"\"\n"
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
		ft::clog << log::fatal << "Invalid syntax in config file :" << std::endl
			      << excp.what() << std::endl;
		ft::DeleteContent(blocks);
		return false;
	}
	catch (std::exception&) {
		ft::clog << log::fatal << "Unmanaged exception whilst parsing config file." << std::endl;	
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
			ft::clog << log::warning << "No port found on server n°" << i << ". "
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
				ft::clog << log::error << "Unable to bind socket to port " << ports[i] 
				<< ": " << err << ' ' << std::strerror(err) << std::endl;
				outsockets.pop_back();
			}
			else {
				ft::clog << log::info << "Socket bound on port " << ports[i] << std::endl;
				socketCreated[ports[i]] = true;
			}
		}

	}

	return outsockets.size();
}

extern int	main(int argc, char** argv)
{
	if (argc > 3) {
		ft::clog << log::error << "Too many arguments" << std::endl;
		return EXIT_FAILURE;
	}

	const char* confpath;
	if (argc < 2 || argv[1] == std::string("--"))
		confpath = "./conf/default.conf";
	else
		confpath = argv[1];

	if (argc == 3) {
		if (ft::StartsWith(argv[2], "--logmask="))
			ft::clog.SetMask(log::StrToMask(argv[2] + sizeof("--logmask")));
		else {
			ft::clog << log::error << "Invalid argument: " << argv[2] << std::endl;
			return EXIT_FAILURE;
		}
	}

	ConfList configs;
	if (!GetConfig(confpath, configs))
		return EXIT_FAILURE;


	SockList sockets;
	if (0 == CreateServers(configs, sockets, ft::Server::availableServers)){
		ft::clog << log::fatal << "No socket was able to be created." << std::endl;
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
				ft::clog << log::event << "Awaiting Poll..." << std::endl;
			event = false;
			event |= ft::PollManager::PollLoop(5);
			event |= ft::TimeoutManager::TimeLoop();
		}
	}
	catch (const ft::CleanExitException& except) {
		ft::PollManager::DeleteAll();
		exit(except.GetStatus());
	}
	catch (ft::PrepackedExecve& exec) {
		ft::PollManager::DeleteAll();
		ft::Server::availableServers.clear();
		sockets.clear();
		configs.clear();
		exec.Execve();
		ft::clog << log::error << "Execve error: " 
		         << errno << ' ' << std::strerror(errno) 
		         << std::endl;
		abort();
	}
}
