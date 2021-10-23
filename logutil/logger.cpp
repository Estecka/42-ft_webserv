/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 13:57:16 by abaur             #+#    #+#             */
/*   Updated: 2021/10/23 16:05:30 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.hpp"

#include "logcolors.hpp"

namespace ft
{
	Logger	clog(std::clog);

	Logger::Logger(std::ostream& output) :
	_output(output)
	{}

	Logger::~Logger(){
		this->Flush();
	}

	Logger&	Logger::operator<< (const log::Label& label) {
		this->Flush();
		this->_label = label;
		this->_hasContent = false;
		this->_labelShown = false;
		return *this;
	}

	Logger&	Logger::operator<< (std::ostream& (*item)(std::ostream&)) {
		if (item == std::endl< char, std::char_traits<char> >){
			this->Flush();
			_output << std::endl;
		}
		else
			_buffer << item;

		return *this;
	}

	void	Logger::Flush() {
		if (!_hasContent)
			return;

		std::string line;
		while(std::getline(_buffer, line), !_buffer.eof()) {
			_output << (_labelShown ? _label.label : _label.tab);
			_output << line;
		}
		_output << LOG_CLEAR;

		_hasContent = false;
		_buffer.str("");
		_buffer.clear();
	}
}