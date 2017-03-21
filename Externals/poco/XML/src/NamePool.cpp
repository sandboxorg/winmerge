//
// NamePool.cpp
//
// $Id: //poco/1.4/XML/src/NamePool.cpp#1 $
//
// Library: XML
// Package: XML
// Module:  NamePool
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/XML/NamePool.h"
#include "Poco/Exception.h"


namespace Poco {
namespace XML {


class NamePoolItem
{
public:
	NamePoolItem(): _used(false)
	{
	}
	
	~NamePoolItem()
	{
	}
	
	bool set(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName)
	{
		if (!_used)
		{
			_name.assign(qname, namespaceURI, localName);
			_used = true;
			return true;
		}
		else return _name.equals(qname, namespaceURI, localName);
	}
	
	const Name& get() const
	{
		return _name;
	}
	
	bool used() const
	{
		return _used;
	}
	
private:
	Name _name;
	bool _used;
};


NamePool::NamePool(unsigned long size): 
	_size(size),
	_rc(1)
{
	poco_assert (size > 1);

	_pItems = new NamePoolItem[size];
}


NamePool::~NamePool()
{
	delete [] _pItems;
}


void NamePool::duplicate()
{
	++_rc;
}


void NamePool::release()
{
	if (--_rc == 0)
		delete this;
}


const Name& NamePool::insert(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName)
{
	unsigned long i = 0;
	unsigned long n = hash(qname, namespaceURI, localName) % _size;

	while (!_pItems[n].set(qname, namespaceURI, localName) && i++ < _size) 
		n = (n + 1) % _size;
		
	if (i > _size) throw Poco::PoolOverflowException("XML name pool");

	return _pItems[n].get();
}


const Name& NamePool::insert(const Name& name)
{
	return insert(name.qname(), name.namespaceURI(), name.localName());
}


unsigned long NamePool::hash(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName)
{
	unsigned long h = 0;
	XMLString::const_iterator it  = qname.begin();
	XMLString::const_iterator end = qname.end();
	while (it != end) h = (h << 5) + h + (unsigned long) *it++;
	it =  namespaceURI.begin();
	end = namespaceURI.end();
	while (it != end) h = (h << 5) + h + (unsigned long) *it++;
	it  = localName.begin();
	end = localName.end();
	while (it != end) h = (h << 5) + h + (unsigned long) *it++;
	return h;
}


} } // namespace Poco::XML
