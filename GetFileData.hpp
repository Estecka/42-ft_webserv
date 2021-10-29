/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetFileData.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:44:34 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/29 12:17:13 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETFILEDATA_HPP
# define GETFILEDATA_HPP

# include "includes/webserv.hpp"
# include "ResponseHeader.hpp"
# include "IPollListener.hpp"
# include "RequestHandler.hpp"

namespace	ft {

	class	GetFileData: public IPollListener
	{
		public:
	
			GetFileData(std::string path, int acceptfd, RequestHandler& parent);
			~GetFileData(void);

			void	GetPollFd(pollfd& outfd);
			void	OnPollEvent(const pollfd& pfd);

		private:

			std::string			_path;
			int					_acceptfd;
			RequestHandler&		_parent;

			std::stringstream	_data;
			std::string			_toWriteData;

			char				_buffer[1024];
			std::size_t			_bufflen;
			ResponseHeader		_head;

			void	ReadFile(void);
	};

}
#endif
