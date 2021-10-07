/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdstream.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 14:46:07 by abaur             #+#    #+#             */
/*   Updated: 2021/10/05 15:56:16 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDSTREAM_HPP
#define FDSTREAM_HPP

#include <iostream>
#include <ext/stdio_filebuf.h>

namespace ft
{
	typedef __gnu_cxx::stdio_filebuf<char>	filebuf;

	class ifdstream : private filebuf,  public std::istream
	{
	public:
		ifdstream(int fd);
		~ifdstream();
	};

	class ofdstream : private filebuf, public std::ostream
	{
	public:
		ofdstream(int fd);
		~ofdstream();
	};
	
}

#endif
