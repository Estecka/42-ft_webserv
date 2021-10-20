/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 16:59:29 by abaur             #+#    #+#             */
/*   Updated: 2021/10/20 15:30:14 by abaur            ###   ########.fr       */
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
	static void	SetArgv(std::vector<char*>& outArray, const RequestHandler& request) {
		std::string	reqPath = request.GetConfig().root + request.GetReqHead()->GetRequestPath().substr(1);
			
		outArray.push_back(strdup(reqPath.c_str()));
		outArray.push_back(NULL);
	}
	static void	SetEnvp(std::vector<char*>& outArray, const RequestHandler& request) {
		const RequestHeader&	reqHead = *request.GetReqHead();
		std::string      	global;
		char             	cwd[PATH_MAX];
		std::string      	strPort;
		std::stringstream	ss;
		ss << reqHead.GetHostPort();
		ss >> strPort;

		chdir(request.GetConfig().root.c_str());

		global = "REQUEST_METHOD=" + reqHead.GetMethod();
		outArray.push_back(strdup(global.c_str()));

		global = "PATH_INFO=" + std::string(getcwd(cwd, sizeof(cwd)));
		outArray.push_back(strdup(global.c_str()));

		global = "PATH_TRANSLATED=" + std::string(getcwd(cwd, sizeof(cwd)));
		outArray.push_back(strdup(global.c_str()));

		global = "SCRIPT_FILENAME=" + reqHead.GetRequestPath().substr(1);
		outArray.push_back(strdup(global.c_str()));

		outArray.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));

		outArray.push_back(strdup("REDIRECT_STATUS=CGI"));

		if (!reqHead.GetQueryString().empty()) {
			global = "QUERY_STRING=" + reqHead.GetQueryString().substr(1);
			outArray.push_back(strdup(global.c_str()));
		}

		if (reqHead.HasProperty("Content-Type")){
			global = "CONTENT_TYPE=" + reqHead["Content-Type"];
			outArray.push_back(strdup(global.c_str()));
		}

		global = "SERVER_PORT=" + strPort;
		outArray.push_back(strdup(global.c_str()));

		global = "REMOTE_ADDR=" + request.GetClientIp();
		outArray.push_back(strdup(global.c_str()));

		global = "REMOTE_HOST=" + reqHead.GetHostname();
		outArray.push_back(strdup(global.c_str()));

		global = "SERVER_NAME=" + reqHead.GetHostname();
		outArray.push_back(strdup(global.c_str()));

		outArray.push_back(strdup("SERVER_SOFTWARE=ft_webserv/1.0"));

		outArray.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));

		outArray.push_back(NULL);
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
