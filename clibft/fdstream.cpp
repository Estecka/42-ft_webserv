/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdstream.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 14:50:56 by abaur             #+#    #+#             */
/*   Updated: 2021/10/09 15:48:42 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdstream.hpp"

namespace ft
{

	// # ifdstream

	ifdstream::ifdstream(int _fd) :
		filebuf(_fd, std::ios::in),
		std::istream(static_cast<filebuf*>(this)),
		fd(_fd)
	{}

	ifdstream::~ifdstream(){}


	// # ofdstream

	ofdstream::ofdstream(int _fd) :
		filebuf(_fd, std::ios::out),
		std::ostream(static_cast<filebuf*>(this)),
		fd(_fd)
	{}

	ofdstream::~ofdstream(){}

}
