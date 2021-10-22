/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrepackedExecve.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 18:04:13 by abaur             #+#    #+#             */
/*   Updated: 2021/10/22 18:29:06 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrepackedExecve.hpp"

#include <unistd.h>
#include <iostream>

namespace ft
{
	bool	PrepackedExecve::isForkedChild = false;

	PrepackedExecve::PrepackedExecve(const std::string& path, const ArgArray& argv, const ArgArray& envp, bool silenceStderr) :
		_path(path),
		_argv(argv),
		_envp(envp),
		_hasStderr(silenceStderr),
		_stderrBak(dup(STDERR_FILENO))
	{
		if (silenceStderr)
			close(STDERR_FILENO);
	}

	PrepackedExecve::~PrepackedExecve()
	{
		for (size_t i=0; i<_argv.size(); i++)
			if (_argv[i] != NULL)
				delete _argv[i];
		_argv.clear();

		for (size_t i=0; i<_envp.size(); i++)
			if (_envp[i] != NULL)
				delete _envp[i];
		_envp.clear();
	}


	int	PrepackedExecve::Execve(){
		if (_hasStderr)
			dup2(_stderrBak, STDERR_FILENO);
		std::clog << "[INFO] Starting CGI: " << _path << std::endl;
		return ::execve(_path.c_str(), &_argv[0], &_envp[0]);
	}

}
