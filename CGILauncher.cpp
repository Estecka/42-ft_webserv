/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 16:59:29 by abaur             #+#    #+#             */
/*   Updated: 2021/10/05 19:24:04 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGILauncher.hpp"

#include "HttpHeader.hpp"
#include "clibft/clibft.hpp"

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

	static void	CGI2HTTP(std::istream& incgi, std::ostream& outhttp){
		// std::stringstream	headBuffer;
		std::string	headline;

		while(std::getline(incgi, headline), !incgi.fail()) {
			outhttp << headline << '\n';
		}

	}

	static noreturn void	CGIMain(const char* CgiPath, int outputfd, const HttpRequest& request){
		int err = 0;

		err = dup2(outputfd, STDOUT_FILENO);
		if (err < 0) {
			std::cerr << "[ERR] dup2 error: " 
			          << errno << ' ' << std::strerror(errno) << '\n' 
			          << std::endl;
			HttpHeader::SendErrCode(500, outputfd);
			close(outputfd);
			abort();
		}
		close(outputfd);

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
		ft::ofdstream outHttp(acceptfd);
		pid_t	pid;
		int	pipefd[2];

		if (pipe(pipefd)) {
			std::cerr << "[ERR] Pipe error: " << errno << ' ' << strerror(errno) << std::endl;
			HttpHeader::SendErrCode(500, acceptfd);
			return;
		}

		pid = fork();
		if (pid == 0)
			CGIMain(CgiPath, pipefd[1], request);
		else if (pid == -1) {
			std::cerr << "[ERR] Fork error: " << errno << ' ' << strerror(errno) << std::endl;
			HttpHeader::SendErrCode(500, acceptfd);
			close(pipefd[0]);
			close(pipefd[1]);
			return;
		}

		close(pipefd[1]);

		// TBD, might need to stream the request body into a pipe here.

		ft::ifdstream	inCgi(pipefd[0]);
		CGI2HTTP(inCgi, outHttp);

		return;
	}
}
