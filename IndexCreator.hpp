/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexCreator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apitoise <apitoise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 10:45:19 by apitoise          #+#    #+#             */
/*   Updated: 2021/09/28 11:15:02 by apitoise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDEXCREATOR_HPP
# define INDEXCREATOR_HPP

# include "includes/webserv.hpp"

class	IndexCreator
{
	public:

		IndexCreator(std::string path);
		IndexCreator(const IndexCreator &other);
		~IndexCreator(void);
		IndexCreator	&operator=(const IndexCreator &other);
};

#endif
