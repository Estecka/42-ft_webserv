/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delegate.cpp.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 18:39:41 by abaur             #+#    #+#             */
/*   Updated: 2021/10/12 14:11:06 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELEGATE_CPP_HPP
#define DELEGATE_CPP_HPP

#include "delegate.hpp"

#include <unistd.h>

namespace ft
{
/******************************************************************************/
/* # delegate                                                                 */
/******************************************************************************/

	template <typename R, typename A>
	delegate<R,A>::delegate(void){
		this->_currentDelegate = NULL;
	}
	template <typename R, typename A>
	delegate<R,A>::delegate(const delegate& other){
		this->operator=(other);
	}
	template <typename R, typename A>
	delegate<R,A>::~delegate(){
		if (this->_currentDelegate)
			delete _currentDelegate;
	}


	template <typename R, typename A>
	delegate<R,A>&	delegate<R,A>::operator=(const delegate& other){
		if (other._currentDelegate)
			this->_currentDelegate = other._currentDelegate->clone();
		else
			this->_currentDelegate = NULL;
		return *this;
	}
	template <typename R, typename A>
	delegate<R,A>&	delegate<R,A>::operator=(c_func func){
		this->SetDelegate(func);
	}


	template <typename R, typename A>
	void	delegate<R,A>::SetDelegate(c_func func){
		if (_currentDelegate)
			delete _currentDelegate;
		_currentDelegate = new CDelegate(func);
	}
	template <typename R, typename A>
	template <typename C>
	void	delegate<R,A>::SetDelegate(R (C::*func)(A), C& object){
		if (_currentDelegate)
			delete _currentDelegate;
		_currentDelegate = new MemberDelegate<C>(object, func);
	}



/******************************************************************************/
/* # C Delegate                                                               */
/******************************************************************************/

	template <typename R, typename A>
	delegate<R,A>::CDelegate::CDelegate(c_func func){
		this->_function = func;
	}
	template <typename R, typename A>
	delegate<R,A>::CDelegate::~CDelegate(){
	}


	template <typename R, typename A>
	R	delegate<R,A>::CDelegate::operator()(A arg) const {
		return (*_function)(arg);
	}

	template <typename R, typename A>
	typename delegate<R,A>::IDelegate*	delegate<R,A>::CDelegate::clone() const{
		return new CDelegate(this->_function);
	}



/******************************************************************************/
/* # Member Delegate                                                          */
/******************************************************************************/

	template <typename R, typename A>
	template <typename C>
	delegate<R,A>::MemberDelegate<C>::MemberDelegate(C& target, m_func func){
		this->_target   = target;
		this->_function = func;
	}
	template <typename R, typename A>
	template <typename C>
	delegate<R,A>::MemberDelegate<C>::~MemberDelegate(){
	}

	template <typename R, typename A>
	template <typename C>
	R	delegate<R,A>::MemberDelegate<C>::operator()(A arg) const {
		return (_target.*_function)(arg);
	}

	template <typename R, typename A>
	template <typename C>
	typename delegate<R,A>::IDelegate*	delegate<R,A>::MemberDelegate<C>::clone() const{
		return new MemberDelegate(this->_target, this->_function);
	}

}

#endif