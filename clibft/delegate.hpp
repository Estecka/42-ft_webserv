/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delegate.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 03:11:47 by abaur             #+#    #+#             */
/*   Updated: 2021/10/11 18:30:54 by abaur            ###   ########.fr       */
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
		template<typename CLASS>
		typedef RET (CLASS::*member_func)(ARG);
		typedef RET (*c_func)(ARG);

		delegate(void);
		delegate(const delegate&);
		~delegate();

		delegate& operator=(const delegate&);
		delegate& operator=(c_func);

		void	SetDelegate(c_func);
		template <typename CLASS>
		void	SetDelegate(member_func<CLASS>, CLASS&);

		RET	operator()(ARG);

	private:
		class IDelegate;
		IDelegate*	_currentDelegate;

	private:
		class IDelegate
		{
		public:
			delegate();
			virtual ~delegate();
			RET	operator()(ARG) = 0;
		};

		template<typename CLASS>
		class MemberDelegate : public IDelegate {
			MemberDelegate(CLASS&, member_func<CLASS>);
			~MemberDelegate();
			RET	operator()(ARG);
		}
		template<typename CLASS>
		class CDelegate : public IDelegate {
			MemberDelegate(c_func);
			~MemberDelegate();
			RET	operator()(ARG);
		}
	};
}

#endif
