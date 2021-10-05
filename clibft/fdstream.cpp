/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdstream.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 14:50:56 by abaur             #+#    #+#             */
/*   Updated: 2021/10/05 15:57:18 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdstream.hpp"

namespace ft
{

	// # ifdstream

	ifdstream::ifdstream(int fd) :
		filebuf(fd, std::ios::in),
		std::istream(static_cast<filebuf*>(this))
	{}

	ifdstream::~ifdstream(){}


	// # ofdstream

	ofdstream::ofdstream(int fd) :
		filebuf(fd, std::ios::out),
		std::ostream(static_cast<filebuf*>(this))
	{}

	ofdstream::~ofdstream(){}

}
