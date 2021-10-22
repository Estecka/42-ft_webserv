/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrepackedExecve.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 17:54:10 by abaur             #+#    #+#             */
/*   Updated: 2021/10/22 18:04:31 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PREPACKEDEXECVE_HPP
#define PREPACKEDEXECVE_HPP

#include <string>
#include <vector>

namespace ft
{
	class PrepackedExecve
	{
	public:
		typedef std::vector<char*>	ArgArray;
		static bool	isForkedChild;

		PrepackedExecve(const std::string& path, const ArgArray& argv, const ArgArray& envp, bool silenceStderr = true);
		~PrepackedExecve();

		int	Execve();

	private:
		std::string	_path;
		ArgArray   	_argv;
		ArgArray   	_envp;
		bool	_hasStderr;
		int 	_stderrBak;
	};
	
}

#endif
