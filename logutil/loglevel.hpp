/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loglevel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 18:28:50 by abaur             #+#    #+#             */
/*   Updated: 2021/10/30 15:09:03 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGLEVEL_HPP
#define LOGLEVEL_HPP

#include <string>

#define	LOGMASK_ALL   (~ft::log::mask_t(0)      )
#define	LOGMASK_NONE  ( ft::log::mask_t(0)      )
#define	LOGMASK_FATAL ( ft::log::mask_t(1) << 0 )
#define	LOGMASK_ERR   ( ft::log::mask_t(1) << 1 )
#define	LOGMASK_WARN  ( ft::log::mask_t(1) << 2 )
#define	LOGMASK_NOTE  ( ft::log::mask_t(1) << 3 )
#define	LOGMASK_EVENT ( ft::log::mask_t(1) << 4 )
#define	LOGMASK_INFO  ( ft::log::mask_t(1) << 5 )
#define	LOGMASK_DEBUG ( ft::log::mask_t(1) << 6 )
#define	LOGMASK_DUMP  ( ft::log::mask_t(1) << 7 )

namespace ft {
namespace log{
	typedef unsigned short	mask_t;

	mask_t	StrToBit(std::string);
	mask_t	StrToMask(std::string);

}}

#endif
