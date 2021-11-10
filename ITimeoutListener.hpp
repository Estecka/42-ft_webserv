/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ITimeoutListener.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 18:32:50 by abaur             #+#    #+#             */
/*   Updated: 2021/11/07 18:50:56 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITIMEOUTLISTENER_HPP
#define ITIMEOUTLISTENER_HPP

namespace ft
{
	/**
	 * Base interface for anything that needs to wait a certain amount of time 
	 * before executing an action.
	 */
	class ITimeoutListener
	{
	public:
		virtual	~ITimeoutListener() {};

		virtual void	OnTimeout() = 0;
	};
}

#endif
