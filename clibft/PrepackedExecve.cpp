/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrepackedExecve.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 18:04:13 by abaur             #+#    #+#             */
/*   Updated: 2021/10/24 17:32:17 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrepackedExecve.hpp"

#include "../logutil/logutil.hpp"
#include <unistd.h>
#include <iostream>

namespace ft
{

	PrepackedExecve::PrepackedExecve(const std::string& path, const ArgArray& argv, const ArgArray& envp) :
		_path(path),
		_argv(argv),
		_envp(envp)
	{}

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
		ft::clog << log::notice << "Starting CGI: " << _path << std::endl;
		return ::execve(_path.c_str(), &_argv[0], &_envp[0]);
	}

}
