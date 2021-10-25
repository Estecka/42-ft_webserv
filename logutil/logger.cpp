/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 13:57:16 by abaur             #+#    #+#             */
/*   Updated: 2021/10/25 18:34:24 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.hpp"

#include "logcolors.hpp"
#include "unistd.h"

namespace ft
{
	Logger	clog(std::clog);

	Logger::Logger(std::ostream& output) :
	_output(output)
	{}

	Logger::~Logger(){
		this->FlushLine();
	}

	bool Logger::IsFork(){ return this->_isFork; }
	
	void Logger::IsFork(bool value){
		this->_isFork = value;
		if (value) {
			std::stringstream label;
			label << "[FORK-" << getpid() << "] ";
			this->_label = log::Label(label.str().c_str(), LOG_BOLD_MAGENTA, LOG_MAGENTA);
		}
	}

	Logger&	Logger::operator<< (const log::Label& label) {
		if (this->_hasContent)
			this->FlushLine();
		if (!_isFork)
			this->_label = label;
		this->_hasContent = false;
		this->_labelShown = false;
		return *this;
	}

	Logger&	Logger::operator<< (std::ostream& (*item)(std::ostream&)) {
		if (item == std::endl< char, std::char_traits<char> >){
			*this << LOG_CLEAR << '\n';
			this->FlushLine();
		}
		else
			_buffer << item;

		return *this;
	}

	void	Logger::FlushLine() {
		// std::cout << LOG_BLUE "(" << _hasContent << ", " << _buffer.str() << ")" << LOG_CLEAR << std::endl;

		std::string line;
		_buffer.clear();
		_output.clear();
		while (std::getline(_buffer, line), !line.empty() || !_buffer.eof()) {
			line = (!_labelShown ? _label.label : _label.tab) + line;
			_labelShown = true;
			if (!_buffer.eof())
				line += '\n';
			_output << line;
			line = "";
			// std::clog << LOG_GREEN << "line: " << line << ", fail: " << _buffer.fail() << "eof: " << _buffer.eof() << std::endl;
		}
		_output << std::flush;

		_hasContent = false;
		_buffer.str("");
		_buffer.clear();
		_output.clear();
	}
}