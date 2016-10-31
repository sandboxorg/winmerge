//
// AccessExpirationDecorator.h
//
// $Id: //poco/1.4/Foundation/include/Poco/AccessExpirationDecorator.h#2 $
//
// Library: Foundation
// Package: Cache
// Module:  AccessExpirationDecorator
//
// Implementation of the AccessExpirationDecorator template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_AccessExpirationDecorator_INCLUDED
#define Foundation_AccessExpirationDecorator_INCLUDED


#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"


namespace Poco {


template <typename TArgs>
class AccessExpirationDecorator
	/// AccessExpirationDecorator adds an expiration method to values so that they can be used
	/// with the UniqueAccessExpireCache
{
public:
	AccessExpirationDecorator():
		_value(),
		_span()
	{
	}

	AccessExpirationDecorator(const TArgs& p, const Poco::Timespan::TimeDiff& diffInMs):
			/// Creates an element that will expire in diff milliseconds
		_value(p),
		_span(diffInMs*1000)
	{
	}

	AccessExpirationDecorator(const TArgs& p, const Poco::Timespan& timeSpan):
		/// Creates an element that will expire after the given timeSpan
		_value(p),
		_span(timeSpan)
	{
	}


	~AccessExpirationDecorator()
	{
	}
	
	const Poco::Timespan& getTimeout() const
	{
		return _span;
	}

	const TArgs& value() const
	{
		return _value;
	}

	TArgs& value()
	{
		return _value;
	}

private:
	TArgs     _value;
	Timespan  _span;
};


} // namespace Poco


#endif // Foundation_AccessExpirationDecorator_INCLUDED
