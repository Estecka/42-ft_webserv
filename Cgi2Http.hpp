/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi2Http.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 17:16:24 by abaur             #+#    #+#             */
/*   Updated: 2021/10/20 16:10:16 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI2HTTP_HPP
#define CGI2HTTP_HPP

#include "IPollListener.hpp"
#include "RequestHandler.hpp"

#include <sstream>

namespace ft
{
	class Cgi2Http : public IPollListener
	{
	public:
		Cgi2Http(RequestHandler& parent, pid_t cigPid, int cgiPipeout);
		~Cgi2Http();

	void	GetPollFd(pollfd&);
	void	OnPollEvent(const pollfd&);

	private:
		RequestHandler&	_parent;
		ft::ifdstream 	_cgiin;
		ft::ofdstream&	_httpout;
		pid_t	_cgiPid;
		bool	_inFail;
		bool	_inEof;
		bool	_outFail;
		bool	_outEof;

		pollfd	_pollfd;
		bool (Cgi2Http::*_pollaction)(void);

		std::stringstream	_headBuffer;
		std::string      	_lineBuffer;

		char  	_buffer[1024];
		size_t	_buffStart;
		size_t	_buffEnd;

		bool	PrepareToReadHead();
		bool	PrepareToReadBody();
		bool	PrepareToWriteHead();
		bool	PrepareToWriteBody();
		bool	PrepareToQuit();

		bool	ReadHead();
		bool	ReadBody();
		bool	WriteHead();
		bool	WriteBody();
	};
} 

#endif
