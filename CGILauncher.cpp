/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 16:59:29 by abaur             #+#    #+#             */
/*   Updated: 2021/10/04 15:33:48 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGILauncher.hpp"

#include "HttpHeader.hpp"


#include <sstream>

#include <cerrno>
#include <cstring>
#include <stdnoreturn.h>
#include <unistd.h>

namespace ft
{
	static void	SetArgv(std::vector<char*>& outarray, const HttpRequest& request) {
		(void)request;
		outarray.push_back(strdup("index.php"));
		outarray.push_back(NULL);
	}
	static void	SetEnvp(std::vector<char*>& outarray, const HttpRequest& request) {
		(void)request;
		outarray.push_back(strdup("REQUEST_METHOD=GET"));
		outarray.push_back(strdup("PATH_INFO=PATH_INFO=/home/user42/Desktop/ft_webserv/"));
		outarray.push_back(strdup("SCRIPT_FILENAME=index.php"));
		outarray.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
		outarray.push_back(strdup("REDIRECT_STATUS=CGI"));
		outarray.push_back(NULL);
	}

	static noreturn void	CGIMain(const char* CgiPath, int acceptfd, const HttpRequest& request){
		int err = 0;

		err = dup2(acceptfd, STDOUT_FILENO);
		if (err < 0) {
			std::cerr << "[ERR] dup2 error: " 
			          << errno << ' ' << std::strerror(errno) << '\n' 
			          << std::endl;
			HttpHeader::SendErrCode(500, acceptfd);
			close(acceptfd);
			abort();
		}
		close(acceptfd);

		std::vector<char*>	argv;
		std::vector<char*>	envp;
		SetArgv(argv, request);
		SetEnvp(envp, request);

		close(STDIN_FILENO);
		std::cerr << "[DEBUG] Starting CGI: " << CgiPath << std::endl;
		err = execve(CgiPath, &argv[0], &envp[0]);
		std::cerr << "[ERR] execve error: "
		          << errno << ' ' << std::strerror(errno) << '\n'
		          << std::endl;
		abort();
	}

	void	LaunchCGI(const char* CgiPath, int acceptfd, const HttpRequest& request) {
		pid_t	pid = fork();
		if (pid == 0)
			CGIMain(CgiPath, acceptfd, request);
		else if (pid == -1) {
			HttpHeader::SendErrCode(500, acceptfd);
			close(acceptfd);
			return;
		}

		// TBD, might need to stream the request body into a pipe here.
		return;
	}
}
