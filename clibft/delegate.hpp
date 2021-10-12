/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delegate.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 03:11:47 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 14:03:08 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELEGATE_HPP
#define DELEGATE_HPP

namespace ft
{
	template<typename RET, typename ARG>
	class delegate
	{
	public:
		typedef RET (*c_func)(ARG);

		delegate(void);
		delegate(const delegate&);
		~delegate();

		delegate& operator=(const delegate&);
		delegate& operator=(c_func);

		void	SetDelegate(c_func);
		template <typename CLASS>
		void	SetDelegate(RET (CLASS::*)(ARG), CLASS&);

		RET	operator()(ARG);

	private:
		class IDelegate;
		IDelegate*	_currentDelegate;

	private:
		class IDelegate
		{
		public:
			IDelegate();
			virtual ~IDelegate();
			RET	operator()(ARG) const = 0;
			IDelegate*	clone() const = 0;
		};

		template<typename CLASS>
		class MemberDelegate : public IDelegate {
		public:
			typedef	RET (CLASS::*m_func)(ARG);
			MemberDelegate(CLASS&, m_func);
			~MemberDelegate();
			RET	operator()(ARG) const;
			IDelegate*	clone() const;
		private:
			CLASS&	_target;
			m_func	_function;
		};

		class CDelegate : public IDelegate {
		public:
			CDelegate(c_func);
			~CDelegate();
			RET	operator()(ARG) const;
			IDelegate*	clone() const;
		private:
			c_func	_function;
		};
	};
}

#include "delegate.cpp.hpp"

#endif
