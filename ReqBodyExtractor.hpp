/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqBodyExtractor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:27:11 by abaur             #+#    #+#             */
/*   Updated: 2021/10/15 13:52:34 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUBODYEXTRACTOR_HPP
#define REQUBODYEXTRACTOR_HPP

#include "RequestHandler.hpp"

#include <cstdio>

namespace ft
{
	class ReqBodyExtractor : public IPollListener
	{
	public:
		ReqBodyExtractor(RequestHandler& parent);
		~ReqBodyExtractor();

		void	GetPollFd(pollfd&);
		void	OnPollEvent(const pollfd&);

	private:
		RequestHandler& _parent;
		ifdstream&	_httpin;

		std::FILE*	_body;
		int       	_bodyfd;
		size_t    	_bodylen;

		bool	_inFail;
		bool	_inEof;
		bool	_outFail;
		bool	_outEof;

		char  	_buffer[1024];
		size_t	_buffstart;
		size_t	_buffend;

		pollfd	_pollfd;
		bool	(ReqBodyExtractor::*_pollAction)(void);
		bool	read();
		bool	write();

		bool	PrepareToRead();
		bool	PrepareToWrite();
		bool	PrepareToQuit();
	};
	
}

#endif
