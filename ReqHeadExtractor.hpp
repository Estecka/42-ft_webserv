/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqHeadExtractor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 14:22:03 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 14:39:00 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQHEADEXTRACTOR_HPP
#define REQHEADEXTRACTOR_HPP

#include "IPollListener.hpp"
#include "RequestHandler.hpp"

#include <sstream>

namespace ft
{
	class ReqHeadExtractor : public IPollListener
	{
	public:
		ReqHeadExtractor(RequestHandler& parent, ifdstream& httpin);
		~ReqHeadExtractor();

		void	GetPollFd(pollfd&);
		void	OnPollEvent(const pollfd&);

	private:
		RequestHandler&	_parent;
		ifdstream&	_httpin;
		std::stringstream	_rawHeader;
	};
}

#endif
