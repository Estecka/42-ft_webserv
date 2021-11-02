/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 16:56:26 by abaur             #+#    #+#             */
/*   Updated: 2021/10/29 20:13:28 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logutil.hpp"

using namespace ft;

extern int	main(int argc, char** argv){
	if (argc > 1)
		ft::clog.SetMask(log::StrToMask(argv[1]));

	ft::clog << log::fatal   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::error   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::warning << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::notice   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::event   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::info    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::debug   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::dump    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::none    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;

	ft::clog.IsFork(true);
	ft::clog << log::error   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::none    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;

	ft::clog.IsFork(false);
	ft::clog << log::none << "\n\n";
	ft::clog << log::debug << "Lorem ipsum dolor sit amet," << std::endl
	         << std::endl
	         << "consectetur adipiscing elit.";
	ft::clog << log::warning << "Sed non risus." << std::endl;
}