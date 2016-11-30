//
// NamePool.h
//
// $Id: //poco/1.4/XML/include/Poco/XML/NamePool.h#1 $
//
// Library: XML
// Package: XML
// Module:  NamePool
//
// Definition of the NamePool class.
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


#ifndef XML_NamePool_INCLUDED
#define XML_NamePool_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/XML/XMLString.h"
#include "Poco/XML/Name.h"


namespace Poco {
namespace XML {


class NamePoolItem;


class XML_API NamePool
	/// A hashtable that stores XML names consisting of an URI, a
	/// local name and a qualified name.
{
public:
	NamePool(unsigned long size = 251);
		/// Creates a name pool with room for up to size strings.
	
	const Name& insert(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName);	
		/// Returns a const reference to an Name for the given names.
		/// Creates the Name if it does not already exist.
		/// Throws a PoolOverflowException if the name pool is full.

	const Name& insert(const Name& name);	
		/// Returns a const reference to an Name for the given name.
		/// Creates the Name if it does not already exist.
		/// Throws a PoolOverflowException if the name pool is full.

	void duplicate();
		/// Increments the reference count.

	void release();
		/// Decrements the reference count and deletes the object if the reference count reaches zero.

protected:
	unsigned long hash(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName);
	~NamePool();

private:
	NamePool(const NamePool&);
	NamePool& operator = (const NamePool&);

	NamePoolItem* _pItems;
	unsigned long _size;
	int           _rc;
};


} } // namespace Poco::XML


#endif // XML_NamePool_INCLUDED
