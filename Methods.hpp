/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:43:42 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/14 11:34:45 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
# define METHODS_HPP

# include "includes/webserv.hpp"
# include "HttpRequest.hpp"
# include "configparser/UriConfig.hpp"
# include "IPollListener.hpp"
# include "RequestHandler.hpp"

namespace ft {

class	Methods : public IPollListener
{
	public:

		Methods(const UriConfig& conf, const HttpRequest& ret, int fd, RequestHandler& parent);
		~Methods(void);
		void	GetPollFd(pollfd& poll_fd);
		void	OnPollEvent(const pollfd&);

	private:

		int						_acceptfd;
		std::string				_method;
		std::string				_reqPath;
		const UriConfig&		_config;
		RequestHandler&			_parent;

		void	Delete(void);
		void	Get_Post(void);
		void	Redirection(void);
		bool	IsDir(const std::string path) const;
		bool	MatchPath(void);
		void	GetFileData(void);
		void	GetIndex(void);
		void	AutoIndex(std::string path);
};
}
#endif
