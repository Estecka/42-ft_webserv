/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputPollListener.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 19:12:06 by abaur             #+#    #+#             */
/*   Updated: 2021/10/27 20:39:39 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutputPollListener.hpp"

#include "clibft/ErrnoException.hpp"
#include "logutil/logutil.hpp"

namespace ft
{
	OutputPollListener::OutputPollListener(void) :
		fail(false),
		eof (false),
		buffer(),
		_file(NULL),
		_fd(-1),
		_pollfd(),
		_pollAction(NULL)
	{
		this->_pollfd.events = POLLOUT;
	}

	OutputPollListener::OutputPollListener(int fd){
		new(this) OutputPollListener();
		this->_fd         = fd;
		this->_pollfd.fd  = fd;
		this->_pollAction = &OutputPollListener::WriteFd;
	}

	OutputPollListener::OutputPollListener(std::FILE& file){
		new(this) OutputPollListener();
		this->_file       = &file;
		this->_pollfd.fd  = ::fileno(&file);
		this->_pollAction = &OutputPollListener::WriteFile;

		if (_pollfd.fd < 0)
			throw ft::ErrnoException("Invalid FILE* to fd conversion.");
	}

	OutputPollListener::~OutputPollListener() {}



/******************************************************************************/
/* # PollListener Interface                                                   */
/******************************************************************************/

	void	OutputPollListener::GetPollFd(pollfd& outpfd){
		outpfd = this->_pollfd;
	}

	void	OutputPollListener::OnPollEvent(const pollfd& pfd){
		this->fail = false;
		this->eof  = false;
		if (buffer.empty())
			ft::clog << log::warning
			         << "Attempted to write an empty string through "
			         << "a PollListener"
			         << std::endl;
		else
			(this->*_pollAction)(pfd);
	}

	void	OutputPollListener::WriteFd(const pollfd&){
		size_t	writelen = ::write(_fd, buffer.data(), buffer.size());

		if (writelen < 0)
			this->fail = true;
		else if (writelen == 0)
			this->eof = true;
		else
			buffer.erase(0, writelen);
	}

	void	OutputPollListener::WriteFile(const pollfd&){
		size_t	writelen = std::fwrite(buffer.data(), 1, buffer.size(), _file);

		this->fail = std::ferror(_file);
		this->eof  = std::feof(_file);
		buffer.erase(0, writelen);
	}

}
