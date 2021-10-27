/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputPollListener.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 19:09:20 by abaur             #+#    #+#             */
/*   Updated: 2021/10/27 19:50:39 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUTPOLLLISTENER_HPP
#define OUTPUTPOLLLISTENER_HPP

#include "IPollListener.hpp"

#include "clibft/fdstream.hpp"
#include <iostream>
#include <sstream>
#include <cstdio>

namespace ft
{
	class OutputPollListener : public IPollListener
	{
	public:
		OutputPollListener(int fd);
		OutputPollListener(std::FILE& file);
		~OutputPollListener();

		void	GetPollFd(pollfd& outfd);
		void	OnPollEvent(const pollfd& pfd);

		bool	fail;
		bool	eof;
		std::string	buffer;

	private:
		std::FILE*	_file;
		int	_fd;

		pollfd	_pollfd;

		OutputPollListener(void);

		void	(OutputPollListener::*_pollAction)(const pollfd&);
		void	WriteFd(const pollfd&);
		void	WriteFile(const pollfd&);
	};
}

#endif
