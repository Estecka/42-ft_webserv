/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loglabel.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 14:29:49 by abaur             #+#    #+#             */
/*   Updated: 2021/10/24 00:10:45 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loglabel.hpp"

#include "logcolors.hpp"

namespace ft {
namespace log{

	const Label none;
	const Label fatal   ( "[FATAL] ", LOG_BOLD_RED,     LOG_RED     );
	const Label error   ( "  [ERR] ", LOG_BOLD_RED,     LOG_RED     );
	const Label warning ( " [WARN] ", LOG_BOLD_YELLOW,  LOG_YELLOW  );
	const Label notice  ( " [NOTE] ", LOG_BOLD_GREEN,   LOG_GREEN   );
	const Label event   ( "[EVENT] ", LOG_BOLD_CYAN,    LOG_CYAN    );
	const Label info    ( " [INFO] ", LOG_BOLD_CLEAR,   LOG_CLEAR   );
	const Label debug   ( "[DEBUG] ", LOG_BOLD_BLUE,    LOG_BLUE    );
	const Label dump    ( " [DUMP] ", LOG_BOLD_MAGENTA, LOG_MAGENTA );


	Label::Label() :
		label(LOG_CLEAR),
		tab(LOG_CLEAR)
	{}

	Label::Label(const Label& other) :
		label(other.label),
		tab(other.tab)
	{}

	Label::Label(const std::string& label, const std::string& tab) :
		label(label),
		tab(tab)
	{}

	Label::Label(const char* clabel, const char* labelColor, const char* logColor)
	{
		this->label = (std::string)labelColor + clabel + logColor;
		this->tab = logColor;

		for (const char*it=clabel; *it!='\0'; it++)
			this->tab += ' ';
	}

	Label&	Label::operator=(const Label& other){
		this->label = other.label;
		this->tab   = other.tab;
		return *this;
	}
}}