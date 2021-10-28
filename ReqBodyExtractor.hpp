/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqBodyExtractor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:27:11 by abaur             #+#    #+#             */
/*   Updated: 2021/10/28 15:32:06 by abaur            ###   ########.fr       */
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
		std::FILE*	_body;
		InputPollListener 	_input;
		OutputPollListener	_output;


		IPollListener*	_activeListener;
		bool	(ReqBodyExtractor::*_pollAction)(const pollfd&);
		bool	read (const pollfd&);
		bool	write(const pollfd&);

		bool	PrepareToRead();
		bool	PrepareToWrite();
		bool	PrepareToQuit();
	};
	
}

#endif
