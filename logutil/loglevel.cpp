/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loglevel.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 19:02:32 by abaur             #+#    #+#             */
/*   Updated: 2021/10/30 14:18:29 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loglevel.hpp"

namespace ft {
namespace log {

	#define MASKBITCOUNT	(sizeof(bitnames)/sizeof(*bitnames))
	static const struct { mask_t bit; const char* name; }	bitnames[] = {
		{ LOGMASK_NONE, 	"none"    },
		{ LOGMASK_ALL,  	"all"     },
		{ LOGMASK_FATAL,	"fatal"   },
		{ LOGMASK_ERR,  	"err"     },
		{ LOGMASK_ERR,  	"error"   },
		{ LOGMASK_WARN, 	"warn"    },
		{ LOGMASK_WARN, 	"warning" },
		{ LOGMASK_NOTE, 	"note"    },
		{ LOGMASK_NOTE, 	"notice"  },
		{ LOGMASK_EVENT,	"event"   },
		{ LOGMASK_INFO, 	"info"    },
		{ LOGMASK_DEBUG,	"debug"   },
		{ LOGMASK_DUMP, 	"dump"    },
	};

	mask_t	StrToBit(const std::string& name) {
		for (size_t i=0; i<MASKBITCOUNT; i++)
			if (name == bitnames[i].name)
				return bitnames[i].bit;
		return LOGMASK_NONE;
	}



	mask_t	StrToMask(std::string sequence){
		mask_t r = LOGMASK_NONE;

		while (!sequence.empty()){
			char op = sequence[0];
			if (op=='-' || op=='+')
				sequence.erase(0 ,1);

			size_t wordend = sequence.find_first_of("+-");
			mask_t bit = StrToBit(sequence.substr(0, wordend));
			sequence.erase(0, wordend);

			if (op == '-')
				r &= ~bit;
			else
				r |= bit;
		}
		return r;
	}

}}
