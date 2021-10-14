/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CleanExitException.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:25:52 by abaur             #+#    #+#             */
/*   Updated: 2021/10/14 16:28:34 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CleanExitException.hpp"

namespace ft
{
	CleanExitException::CleanExitException(int status) throw() :
		_status(status)
	{}

	CleanExitException::~CleanExitException() throw()
	{}

	int	CleanExitException::GetStatus() const throw() {
		return this->_status;
	}
}