/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loglabel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 14:23:47 by abaur             #+#    #+#             */
/*   Updated: 2021/10/29 20:00:05 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGLABEL_HPP
#define LOGLABEL_HPP

#include "loglevel.hpp"
#include <string>

namespace ft {
namespace log {

	struct Label {
		Label();
		Label(const Label&);
		Label(const std::string& label, const std::string& tab, log::mask_t level);
		Label(const char* label, const char* labelColor, const char* logColor, log::mask_t level);

		Label&	operator=(const Label&);

		log::mask_t	level;
		std::string	label;
		std::string	tab;
	};

	extern const Label none;
	extern const Label fatal;
	extern const Label error;
	extern const Label warning;
	extern const Label notice;
	extern const Label event;
	extern const Label info;
	extern const Label debug;
	extern const Label dump;

}}

#endif
