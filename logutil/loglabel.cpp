/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loglabel.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 14:29:49 by abaur             #+#    #+#             */
/*   Updated: 2021/10/29 20:03:29 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loglabel.hpp"

#include "logcolors.hpp"

namespace ft {
namespace log{

	const Label none;
	const Label fatal   ( "[FATAL] ", LOG_BOLD_RED,     LOG_RED,     LOGMASK_FATAL );
	const Label error   ( "  [ERR] ", LOG_BOLD_RED,     LOG_RED,     LOGMASK_ERR   );
	const Label warning ( " [WARN] ", LOG_BOLD_YELLOW,  LOG_YELLOW,  LOGMASK_WARN  );
	const Label notice  ( " [NOTE] ", LOG_BOLD_GREEN,   LOG_GREEN,   LOGMASK_NOTE  );
	const Label event   ( "[EVENT] ", LOG_BOLD_CYAN,    LOG_CYAN,    LOGMASK_EVENT );
	const Label info    ( " [INFO] ", LOG_BOLD_CLEAR,   LOG_CLEAR,   LOGMASK_INFO  );
	const Label debug   ( "[DEBUG] ", LOG_BOLD_BLUE,    LOG_BLUE,    LOGMASK_DEBUG );
	const Label dump    ( " [DUMP] ", LOG_BOLD_MAGENTA, LOG_MAGENTA, LOGMASK_DUMP  );


	Label::Label() :
		level(LOGMASK_NONE),
		label(LOG_CLEAR),
		tab(LOG_CLEAR)
	{}

	Label::Label(const Label& other) :
		level(other.level),
		label(other.label),
		tab(other.tab)
	{}

	Label::Label(const std::string& label, const std::string& tab, log::mask_t level) :
		level(level),
		label(label),
		tab(tab)
	{}

	Label::Label(const char* clabel, const char* labelColor, const char* logColor, log::mask_t level) :
		level(level)
	{
		this->label = (std::string)labelColor + clabel + logColor;
		this->tab = logColor;

		for (const char*it=clabel; *it!='\0'; it++)
			this->tab += ' ';
	}

	Label&	Label::operator=(const Label& other){
		this->level = other.level;
		this->label = other.label;
		this->tab   = other.tab;
		return *this;
	}
}}