/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrepackedExecve.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 17:54:10 by abaur             #+#    #+#             */
/*   Updated: 2021/10/24 17:31:39 by abaur            ###   ########.fr       */
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

		PrepackedExecve(const std::string& path, const ArgArray& argv, const ArgArray& envp);
		~PrepackedExecve();

		int	Execve();

	private:
		std::string	_path;
		ArgArray   	_argv;
		ArgArray   	_envp;
	};
	
}

#endif
