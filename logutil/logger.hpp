/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 19:46:46 by abaur             #+#    #+#             */
/*   Updated: 2021/10/23 17:04:46 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "loglabel.hpp"

#include "logcolors.hpp"

#include <iostream>
#include <sstream>


namespace ft
{
	class Logger
	{
	public:
		Logger(std::ostream& out = std::cout);
		~Logger();

		Logger& operator<<(const log::Label& label);
		Logger& operator<<(std::ostream& (*streamManipulator)(std::ostream&));
		template <typename T>
		Logger&	operator <<(const T& item);

	private:
		std::ostream& _output;
		std::stringstream _buffer;
		log::Label	_label;
		bool	_labelShown;
		bool	_hasContent;

		void	Flush();
	};

	extern Logger clog;

	template<typename T>
	Logger&	Logger::operator<< (const T& item) {
		// std::cout << LOG_BLUE << item << LOG_CLEAR;
		_buffer << item;
		_hasContent = true;
		return *this;
	}
}

#endif
