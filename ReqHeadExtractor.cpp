/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqHeadExtractor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 14:27:00 by abaur             #+#    #+#             */
/*   Updated: 2021/10/15 14:54:27 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReqHeadExtractor.hpp"

namespace ft
{
	ReqHeadExtractor::ReqHeadExtractor(RequestHandler& parent, ifdstream& httpin) :
		_parent(parent),
		_httpin(httpin) 
	{
		std::cerr << "[DEBUG] Head Extractor created." << std::endl;
	}
	ReqHeadExtractor::~ReqHeadExtractor()
	{
		std::cerr << "[DEBUG] Head Extractor destroyed." << std::endl;
	}

	void	ReqHeadExtractor::GetPollFd(pollfd& outpfd) {
		outpfd.fd = _httpin.fd;
		outpfd.events = POLLIN;
	}

	void	ReqHeadExtractor::OnPollEvent(const pollfd&) {
		std::string	line;

		_httpin.clear();
		while (true) {
			std::getline(_httpin, line);
			_rawHeader << line;

			// std::cerr << "[DEBUG] Reading Request Header: " << BitToCString(line) << "\n"
			//           << "        Fail: " << _httpin.fail() << ", Eof: " << _httpin.eof()
			//           << std::endl;

			if (_httpin.eof())
				break;
			if (_httpin.fail())
				return;
			else {
				_rawHeader << '\n';
				if (line == "" || line == "\r")
					break;
			}
		}

		HttpRequest*	result;
		if (_rawHeader.str().length() > 0) {
			std::cout << _rawHeader.str() << std::endl;
			result = new HttpRequest(_rawHeader);
		}
		else
			result = NULL;

		_parent.OnHeaderExtracted(result);
	}
}