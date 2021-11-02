/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqBodyExtractor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:27:11 by abaur             #+#    #+#             */
/*   Updated: 2021/11/01 15:10:13 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUBODYEXTRACTOR_HPP
#define REQUBODYEXTRACTOR_HPP

#include "RequestHandler.hpp"
#include "InputPollListener.hpp"
#include "OutputPollListener.hpp"

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
		const RequestHeader&  _request;
		std::FILE*	_body;
		InputPollListener 	_input;
		OutputPollListener	_output;
		size_t	_chunkToRead;


		IPollListener*	_activeListener;
		bool	(ReqBodyExtractor::*_pollAction)(const pollfd&);
		bool	Read (const pollfd&);
		bool	ChunkedRead (const pollfd&);
		bool	Write(const pollfd&);

		bool	PrepareToRead();
		bool	PrepareToWrite();
		bool	PrepareToQuit();
	};
	
}

#endif
