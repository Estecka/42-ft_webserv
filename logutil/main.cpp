/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 16:56:26 by abaur             #+#    #+#             */
/*   Updated: 2021/10/23 18:03:23 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.hpp"
#include "loglabel.hpp"

using namespace ft;

extern int	main(){
	ft::clog << log::fatal   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::error   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::warning << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::info    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::debug   << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::dump    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::fork    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
	ft::clog << log::none    << "Lorem ipsum dolor sit amet, \n" << "consectetur adipiscing elit" << std::endl;
}