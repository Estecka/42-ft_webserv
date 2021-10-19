/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 16:59:29 by abaur             #+#    #+#             */
/*   Updated: 2021/10/19 17:35:53 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGILauncher.hpp"

#include "clibft/ErrnoException.hpp"


#include <cerrno>
#include <cstring>
#include <stdnoreturn.h>
#include <unistd.h>

namespace ft
{
	static void	SetArgv(std::vector<char*>& outarray, const RequestHandler& request) {
		std::string	req = conf.root + request.GetRequestPath().substr(1, request.GetRequestPath().size());

		outarray.push_back(strdup(req.c_str()));
		outarray.push_back(NULL);
	}
	static void	SetEnvp(std::vector<char*>& outarray, const RequestHandler& request) {
		ResponseHeader	header(200);
		if (request.GetRequestPath().rfind(".") == std::string::npos)
			header.SetContentType("");
		else
			header.SetContentType(request.GetRequestPath().substr(request.GetRequestPath().rfind(".")));

		char	cwd[PATH_MAX];

		std::string	global = "REQUEST_METHOD=" + request.GetMethod();
		outarray.push_back(strdup(global.c_str()));
		
		chdir(conf.root.c_str());
		global = "PATH_INFO=" + std::string(getcwd(cwd, sizeof(cwd)));
		outarray.push_back(strdup(global.c_str()));

		global = "PATH_TRANSLATED=" + std::string(getcwd(cwd, sizeof(cwd))); //??
		outarray.push_back(strdup(global.c_str()));

		global = "SCRIPT_FILENAME=" + request.GetRequestPath().substr(1, request.GetRequestPath().size());
		outarray.push_back(strdup(global.c_str()));
		
		outarray.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
		
		outarray.push_back(strdup("REDIRECT_STATUS=CGI"));
		
		if (!request.GetQueryString().empty()) {
			global = "QUERY_STRING=" + request.GetQueryString().substr(1, request.GetQueryString().size());
			outarray.push_back(strdup(global.c_str()));
		}
		
		global = "CONTENT_TYPE=" + header.GetContentType();
		outarray.push_back(strdup(global.c_str()));	
		
		std::string	strPort;
		std::stringstream ss;
		ss << request.GetHostPort();
		ss >> strPort;
		global = "SERVER_PORT=" + strPort;
		outarray.push_back(strdup(global.c_str()));
		
		global = "REMOTE_ADDR=" + clientIP;
		outarray.push_back(strdup(global.c_str()));

		global = "REMOTE_HOST=" + request.GetHostname();
		outarray.push_back(strdup(global.c_str()));

		global = "SERVER_NAME=" + request.GetHostname();
		outarray.push_back(strdup(global.c_str()));

		outarray.push_back(strdup("SERVER_SOFTWARE=ft_webserv/1.0"));

		outarray.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
		
		outarray.push_back(NULL);
	}

	static bool	SetStdin(const FILE* body){
		if (!body)
			return close (STDOUT_FILENO), false;
		return 0 > dup2(fileno((FILE*)body), STDIN_FILENO);
	}
	static bool	SetStdout(int outputfd){
		return 0 > dup2(outputfd, STDOUT_FILENO);
	}

	static noreturn void	CGIMain(const RequestHandler& request, int outputfd){
		bool err = false;

		err |= SetStdin(request.GetReqBody());
		err |= SetStdout(outputfd);
		if (err) {
			std::clog << "[ERR] dup2 error: " 
			          << errno << ' ' << std::strerror(errno) << '\n' 
			          << std::endl;
			close(outputfd);
			abort();
		}
		close(outputfd);

		const char*       	cgiPath;
		std::vector<char*>	argv;
		std::vector<char*>	envp;
		cgiPath = request.GetConfig().cgiPath.c_str();
		SetArgv(argv, request);
		SetEnvp(envp, request);

		std::clog << "[INFO] Starting CGI: " << cgiPath << std::endl;
		err = execve(cgiPath, &argv[0], &envp[0]);
		std::clog << "[ERR] execve error: "
		          << errno << ' ' << std::strerror(errno) << '\n'
		          << std::endl;
		abort();
	}


	void	LaunchCGI(RequestHandler& parent, pid_t& outPid, int& outPipe) {
		pid_t	pid;
		int	pipefd[2];

		if (pipe(pipefd))
			throw ft::ErrnoException("Pipe error");

		pid = fork();
		if (pid == 0)
			CGIMain(parent, pipefd[1]);
		else if (pid == -1) {
			close(pipefd[0]);
			close(pipefd[1]);
			throw ft::ErrnoException("Fork error");
		}

		outPid  = pid;
		outPipe = pipefd[0];
		return;
	}
}
