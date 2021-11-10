/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiKiller.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 19:25:50 by abaur             #+#    #+#             */
/*   Updated: 2021/11/08 14:05:49 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIKILLER_HPP
#define CGIKILLER_HPP

#include "ITimeoutListener.hpp"

#include <unistd.h>

namespace ft
{
	class CgiKiller : public ITimeoutListener
	{
	public:
		CgiKiller(pid_t cgiPid, pid_t cgiParentPid);
		~CgiKiller();

		void	OnTimeout();

	private:
		pid_t	_cgiPid;
		pid_t	_cgiParent;
		bool	_softkill;
		bool	_hardkill;
	};
}

#endif
