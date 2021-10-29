/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:25:13 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/28 13:22:25 by apitoise         ###   ########.fr       */
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
	
			PostMethod(FILE* body, RequestHandler& parent, std::string upload_path, int acceptfd);
			~PostMethod(void);

			void	GetPollFd(pollfd& outfd);
			void	OnPollEvent(const pollfd& pfd);


		private:
			
			FILE*				_body;
			int					_bodyfd;
			char				_buffer[1024];
			std::string			_strBuff;
			std::string			_eof;
			bool				_firstLoop;
			std::string			_boundary;
			bool				_reachedEoF;

			
			RequestHandler&		_parent;
			pollfd				_pollfd;
			int					_newFd;
			std::string			_upload_path;
			int					_acceptfd;

			bool				_newFile;
			bool				_endOfNewFile;
			std::string			_content;
			std::string			_fileName;
			std::string			_name;
	
			bool				(PostMethod::*_pollAction)(void);

			bool			PrepareToRead(void);
			bool			PrepareToWrite(void);
			bool			PrepareToQuit(void);
			bool			read(void);
			bool			write(void);
			
			void			FirstParsing(void);
			void			ParseHeader(void);
			void			TreatBuffer(void);
			std::string		FindWord(std::string content, std::string toFind, char sep);
	};

}
#endif
