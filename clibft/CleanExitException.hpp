/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CleanExitException.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:20:53 by abaur             #+#    #+#             */
/*   Updated: 2021/10/14 16:28:42 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANEXITEXCEPTION
#define CLEANEXITEXCEPTION

#include <stdexcept>
#include <cstdlib>

namespace ft
{
	class CleanExitException : public std::exception
	{
	public:
		CleanExitException(int status = EXIT_SUCCESS) throw();
		~CleanExitException() throw();

		int	GetStatus() const throw();

	private:
		int	_status;
	};
}
#endif
