/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:07:56 by abaur             #+#    #+#             */
/*   Updated: 2021/10/16 18:14:02 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLMANAGER_HPP
# define POLLMANAGER_HPP

# include "IPollListener.hpp"

namespace ft
{
	class PollManager
	{
	public:
		/**
		 * Performs one round of polling checks.
		 * @return	True if at least one poll event was sent.
		 */
		static bool	PollLoop(int timeout = 5);

		static void	AddListener(IPollListener&);
		static void	RemoveListener(IPollListener&);
		static void	SetDirty();
		static void	DeleteAll();

	private:
		/**
		 * The objects that are waiting for a polled fd.
		 */
		static std::vector<IPollListener*>	_listeners;
		/**
		 * The actual array of fds that will be passed to `poll()`
		 */
		static std::vector<struct pollfd> 	_pollfds;
		/**
		 * Wether the pollfd array needs to be reconstructed.
		 * This should be set to true whenever the list of listeners is modified.
		 */
		static bool	_dirty;

		/**
		 * Reconstructs _pollfds if the manager is dirty.
		 */
		static void	RecreatePollArray();
	};
}

#endif
