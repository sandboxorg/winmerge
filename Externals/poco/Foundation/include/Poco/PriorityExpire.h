//
// PriorityExpire.h
//
// $Id: //poco/1.4/Foundation/include/Poco/PriorityExpire.h#3 $
//
// Library: Foundation
// Package: Events
// Module:  PriorityExpire
//
// Implementation of the PriorityExpire template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_PriorityExpire_INCLUDED
#define Foundation_PriorityExpire_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Timestamp.h"
#include "Poco/AbstractPriorityDelegate.h"


namespace Poco {


template <class TArgs>
class PriorityExpire: public AbstractPriorityDelegate<TArgs>
	/// Decorator for AbstractPriorityDelegate adding automatic 
	/// expiring of registrations to AbstractPriorityDelegate.
{
public:
	PriorityExpire(const AbstractPriorityDelegate<TArgs>& p, Timestamp::TimeDiff expireMilliSec):
		AbstractPriorityDelegate<TArgs>(p),
		_pDelegate(static_cast<AbstractPriorityDelegate<TArgs>*>(p.clone())), 
		_expire(expireMilliSec*1000)
	{
	}
	
	PriorityExpire(const PriorityExpire& expire):
		AbstractPriorityDelegate<TArgs>(expire),
		_pDelegate(static_cast<AbstractPriorityDelegate<TArgs>*>(expire._pDelegate->clone())),
		_expire(expire._expire),
		_creationTime(expire._creationTime)
	{
	}

	~PriorityExpire()
	{
		delete _pDelegate;
	}
	
	PriorityExpire& operator = (const PriorityExpire& expire)
	{
		if (&expire != this)
		{
			delete this->_pDelegate;
			this->_pTarget      = expire._pTarget;
			this->_pDelegate    = expire._pDelegate->clone();
			this->_expire       = expire._expire;
			this->_creationTime = expire._creationTime;
		}
		return *this; 
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		if (!expired())
			return this->_pDelegate->notify(sender, arguments);
		else
			return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		return other.equals(*_pDelegate);
	}

	AbstractPriorityDelegate<TArgs>* clone() const
	{
		return new PriorityExpire(*this);
	}

	void disable()
	{
		_pDelegate->disable();
	}

	const AbstractPriorityDelegate<TArgs>* unwrap() const
	{
		return this->_pDelegate;
	}

protected:
	bool expired() const
	{
		return _creationTime.isElapsed(_expire);
	}

	AbstractPriorityDelegate<TArgs>* _pDelegate;
	Timestamp::TimeDiff _expire;
	Timestamp _creationTime;

private:
	PriorityExpire();
};


} // namespace Poco


#endif // Foundation_PriorityExpire_INCLUDED
