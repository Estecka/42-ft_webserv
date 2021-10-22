/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/22 13:36:50 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTMETHOD_HPP
# define POSTMETHOD_HPP

# include "includes/webserv.hpp"
# include "RequestHandler.hpp"

namespace ft {

	class	PostMethod : public IPollListener
	{
		public:
	
			PostMethod(FILE* body, RequestHandler& parent);
			~PostMethod(void);

			void	GetPollFd(pollfd& outfd);
			void	OnPollEvent(const pollfd& pfd);


		private:
			
			FILE*				_body;
			int					_bodyfd;
			std::stringstream	_ssBody;
			RequestHandler&		_parent;
			std::string			_fileName;
			char				_buffer[1024];
			pollfd				_pollfd;
			FILE*				_tmpFile;
			int					_tmpFileFd;
			bool				(PostMethod::*_pollAction)(void);
			
			bool	PrepareToRead(void);
			bool	PrepareToWrite(void);
			bool	read(void);
			bool	findName(void);
	};

}
#endif
