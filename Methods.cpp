/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:43:42 by apitoise          #+#    #+#             */
/*   Updated: 2021/10/13 15:52:24 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Methods.hpp"

namespace ft {

	Methods::Methods(std::string method, int acceptfd): _method(method), _acceptfd(acceptfd), _config(config) {}

	Methods::Methods(const Methods &other): _method(other._method), _acceptfd(other._acceptfd), _config(config) {}

	Methods::~Methods(void) {}

	Methods	&Methods::operator=(const Methods& other) {
		return (*this);
}

}
