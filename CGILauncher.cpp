/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGILauncher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 16:59:29 by abaur             #+#    #+#             */
/*   Updated: 2021/10/18 14:47:45 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGILauncher.hpp"

#include "ResponseHeader.hpp"
#include "clibft/clibft.hpp"

#include <sstream>

#include <cerrno>
#include <cstring>
#include <stdnoreturn.h>
#include <unistd.h>

namespace ft
{
	static void	SetArgv(std::vector<char*>& outarray, const RequestHeader& request, const UriConfig& conf) {
		std::string	req = conf.root + request.GetRequestPath().substr(1, request.GetRequestPath().size());

		outarray.push_back(strdup(req.c_str()));
		outarray.push_back(NULL);
	}
	static void	SetEnvp(std::vector<char*>& outarray, const RequestHeader& request, const UriConfig& conf, std::string clientIP) {
		
		(void)conf;
		
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

	static noreturn void	CGIMain(const char* CgiPath, int outputfd, const RequestHeader& request, const UriConfig& conf, std::string clientIP){
		int err = 0;

		err = dup2(outputfd, STDOUT_FILENO);
		if (err < 0) {
			std::cerr << "[ERR] dup2 error: " 
			          << errno << ' ' << std::strerror(errno) << '\n' 
			          << std::endl;
			ResponseHeader::SendErrCode(500, outputfd);
			close(outputfd);
			abort();
		}
		close(outputfd);

		std::vector<char*>	argv;
		std::vector<char*>	envp;
		SetArgv(argv, request, conf);
		SetEnvp(envp, request, conf, clientIP);

		close(STDIN_FILENO);
		std::cerr << "[DEBUG] Starting CGI: " << CgiPath << std::endl;
		err = execve(CgiPath, &argv[0], &envp[0]);
		std::cerr << "[ERR] execve error: "
		          << errno << ' ' << std::strerror(errno) << '\n'
		          << std::endl;
		abort();
	}

	void	LaunchCGI(const char* CgiPath, int acceptfd, const RequestHeader& request, const UriConfig& conf, std::string clientIP) {
		ft::ofdstream outHttp(acceptfd);
		pid_t	pid;
		int	pipefd[2];

		if (pipe(pipefd)) {
			std::cerr << "[ERR] Pipe error: " << errno << ' ' << strerror(errno) << std::endl;
			ResponseHeader::SendErrCode(500, acceptfd);
			return;
		}

		pid = fork();
		if (pid == 0)
			CGIMain(CgiPath, pipefd[1], request, conf, clientIP);
		else if (pid == -1) {
			std::cerr << "[ERR] Fork error: " << errno << ' ' << strerror(errno) << std::endl;
			ResponseHeader::SendErrCode(500, acceptfd);
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
