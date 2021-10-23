/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loglabel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 14:23:47 by abaur             #+#    #+#             */
/*   Updated: 2021/10/23 15:43:19 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGLABEL_HPP
#define LOGLABEL_HPP

#include <string>

namespace ft {
namespace log {

	struct Label {
		Label();
		Label(const Label&);
		Label(const std::string& label, const std::string& tab);
		Label(const char* label, const char* labelColor, const char* logColor);

		Label&	operator=(const Label&);

		std::string	label;
		std::string	tab;
	};

	extern Label none;
	extern Label fatal;
	extern Label error;
	extern Label warning;
	extern Label info;
	extern Label debug;
	extern Label dump;
	extern Label fork;

}}

#endif
