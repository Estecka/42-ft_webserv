/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 12:31:54 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/29 13:33:05 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "includes/webserv.hpp"
# include "RequestHandler.hpp"
# include "ResponseHeader.hpp"

namespace	ft {

	class	AutoIndex : public IPollListener
	{
		public:
	
			AutoIndex(int acceptfd, std::string path, std::string root, std::string dirName, RequestHandler& parent);
			~AutoIndex(void);

			void	GetPollFd(pollfd& outfd);
			void	OnPollEvent(const pollfd&);

		private:
	
			int					_acceptfd;
			std::string			_path;
			std::string			_root;
			std::string			_dirName;
			RequestHandler&		_parent;

			std::string			_href;
			std::string			_index;
			
			DIR*								_dir;
			struct dirent*						_ent;
			std::list<std::string>				_inDirFile;
			std::list<std::string>::iterator	_it;

			void	SetIndex(void);
	
	};

}

#endif
