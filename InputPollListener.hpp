/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputPollListener.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:54:19 by abaur             #+#    #+#             */
/*   Updated: 2021/10/28 15:08:17 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTPOLLLISTENER_HPP
#define INPUTPOLLLISTENER_HPP

#include "IPollListener.hpp"

#include "clibft/fdstream.hpp"
#include <iostream>
#include <string>
#include <cstdio>

namespace ft
{
	class InputPollListener : public IPollListener
	{
	public:
		InputPollListener(int fd);
		InputPollListener(std::FILE& file);
		InputPollListener(ft::ifdstream& istream);
		~InputPollListener();

		void	GetPollFd(pollfd& outfd);
		void	OnPollEvent(const pollfd& pfd);

		bool	fail;
		bool	eof;
		std::string	buffer;
		/**
		 * After every read operation, this is incremented by the amount of 
		 * characters read.
		 */
		size_t	readAmount;

	private:
		ft::ifdstream*	_istream;
		std::FILE*	_file;
		int	_fd;

		pollfd	_pollfd;

		InputPollListener(void);

		void	(InputPollListener::*_pollAction)(const pollfd&);
		void	ReadFd(const pollfd&);
		void	ReadFile(const pollfd&);
		void	ReadStream(const pollfd&);
	};
}

#endif
