/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiKiller.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 19:29:05 by abaur             #+#    #+#             */
/*   Updated: 2021/11/08 14:13:25 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiKiller.hpp"

#include "clibft/ErrnoException.hpp"
#include "logutil/logutil.hpp"
#include "TimeoutManager.hpp"
#include <cerrno>
#include <cstring>
#include <signal.h>
#include <sys/wait.h>

/**
 * Like kill, but throws in case of error.
 */
static void	KillT(pid_t pid, int sig){
	if (kill(pid, sig) < 0)
		throw ft::ErrnoException("Kill error");
}

/**
 * Like waitpid, but throws in case of error.
 */
static bool	WaitPidT(pid_t pid){
	int err = waitpid(pid, NULL, WNOHANG);
	if (err < 0)
		throw ft::ErrnoException("waitpid error");
	else
		return err != 0;
}

namespace ft
{
	CgiKiller::CgiKiller(pid_t cgiPid, pid_t cgiParentPid) : 
		_cgiPid(cgiPid),
		_cgiParent(cgiParentPid),
		_softkill(false),
		_hardkill(false)
	{
		ft::clog << log::info << "CgiKiller created" << std::endl;
	}

	CgiKiller::~CgiKiller()
	{
		TimeoutManager::RemoveListener(*this, false);
		ft::clog << log::info << "CgiKiller destroyed" << std::endl;
	}

	void	CgiKiller::OnTimeout()
	{
		if (getpid() != _cgiParent){
			ft::clog << log::error << "Attempted to kill a cgi from a diferent process than its genitor." << std::endl;
			delete this;
		}

		try 
		{
			if (WaitPidT(_cgiPid)){
				ft::clog << log::info << "Fork " << _cgiPid << " terminated." << std::endl;
				delete this;
			}
			else if (!_softkill){
				ft::clog << log::warning 
				         << "Fork " << _cgiPid << " didn't terminate after "
				         << "a while. Sending SIGTERM." 
				         << std::endl;
				KillT(_cgiPid, SIGTERM);
				_softkill = true;
				TimeoutManager::AddListener(*this, 5);
			}
			else if (!_hardkill) {
				ft::clog << log::warning
				         << "Fork " << _cgiPid << " didn't terminate on SIGTERM. "
				         << "Sending SIGKILL."
				         << std::endl;
				KillT(_cgiPid, SIGKILL);
				_hardkill = true;
				TimeoutManager::AddListener(*this, 5);
			}
			else {
				ft::clog << log::error 
				         << "Fork " << _cgiPid << " didn't terminate after "
				         << "sending SIGKILL. Only god can save you now."
				         << std::endl;
				delete this;
			}
		}
		catch (const ft::ErrnoException& e){
			ft::clog << log::error << e.what() << std::endl;
			delete this;
		}
		return;
	}

}
