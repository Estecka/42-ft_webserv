/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 16:59:29 by abaur             #+#    #+#             */
/*   Updated: 2021/10/07 14:02:40 by apitoise         ###   ########.fr       */
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
		std::string	req = request.GetRequestPath().substr(1, request.GetRequestPath().size());

		outarray.push_back(strdup(req.c_str()));
		outarray.push_back(NULL);
	}
	static void	SetEnvp(std::vector<char*>& outarray, const HttpRequest& request, const UriConfig& conf) {
		(void)conf;
		std::string	global = "REQUEST_METHOD=" + request.GetMethod();
		char	cwd[PATH_MAX];

		outarray.push_back(strdup(global.c_str()));
		global = "PATH_INFO=" + std::string(getcwd(cwd, sizeof(cwd)));
		outarray.push_back(strdup(global.c_str()));
		global = "SCRIPT_FILENAME=" + request.GetRequestPath().substr(1, request.GetRequestPath().size());
		outarray.push_back(strdup(global.c_str()));
		outarray.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
		outarray.push_back(strdup("REDIRECT_STATUS=CGI"));
		outarray.push_back(NULL);
	}

	static void	CGI2HTTP(std::istream& incgi, std::ostream& outhttp){
		std::stringstream	headBuffer;
		std::string	line;

		// Try parsing the header to identify the status code.
		while(std::getline(incgi, line), !incgi.fail()) 
		{
			if (line.empty()) {
				headBuffer << '\n';
				outhttp << "HTTP/1.1 200 OK\n";
				break;
			}
			else if (ft::StartsWith(line, "Status:")) {
				outhttp << "HTTP/1.1 " << line.substr(7);
				break;
			}
			else {
				headBuffer << line << '\n';
				continue;
			}
		}

		// Dump everything else as-is
		outhttp << headBuffer.str();

		char	bodyBuffer[1024];
		while (incgi.read(bodyBuffer, 1024), 0 < incgi.gcount())
			outhttp.write(bodyBuffer, incgi.gcount());
	}

	static noreturn void	CGIMain(const char* CgiPath, int outputfd, const HttpRequest& request, const UriConfig& conf){
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
		SetEnvp(envp, request, conf);

		close(STDIN_FILENO);
		std::cerr << "[DEBUG] Starting CGI: " << CgiPath << std::endl;
		err = execve(CgiPath, &argv[0], &envp[0]);
		std::cerr << "[ERR] execve error: "
		          << errno << ' ' << std::strerror(errno) << '\n'
		          << std::endl;
		abort();
	}

	void	LaunchCGI(const char* CgiPath, int acceptfd, const HttpRequest& request, const UriConfig& conf) {
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
			CGIMain(CgiPath, pipefd[1], request, conf);
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
