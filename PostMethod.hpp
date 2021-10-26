/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/26 16:56:03 by apitoise         ###   ########.fr       */
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
			
			FILE*						_body;
			int							_bodyfd;
			std::string					_strBuff;
			RequestHandler&				_parent;
			char						_buffer[1024];
			pollfd						_pollfd;
			int							_newFd;
			bool						(PostMethod::*_pollAction)(void);
			std::string					_boundary;
			std::string					_eof;
			bool						_firstLoop;
			bool						_newFile;
			bool						_reachedEoF;
			bool						_endOfNewFile;

			std::string			_content;
			std::string			_fileName;
			std::string			_name;
			std::string			_contentType;
			std::string			_contentDisposition;

			bool		PrepareToRead(void);
			bool		PrepareToWrite(void);
			bool		PrepareToQuit(void);
			bool		read(void);
			bool		write(void);
			bool		quit(void);
			
			void		FirstParsing(void);
			void		ParseHeader(void);
			void		TreatBuffer(void);
			std::string	FindWord(std::string content, std::string toFind, char sep);
	};

}
#endif
