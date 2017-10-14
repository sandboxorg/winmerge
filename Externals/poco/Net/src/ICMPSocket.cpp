//
// ICMPSocket.cpp
//
// $Id: //poco/1.4/Net/src/ICMPSocket.cpp#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPSocket
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


#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPSocketImpl.h"
#include "Poco/Exception.h"


using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


ICMPSocket::ICMPSocket(IPAddress::Family family, int dataSize, int ttl, int timeout): 
	Socket(new ICMPSocketImpl(family, dataSize, ttl, timeout)),
	_dataSize(dataSize), 
	_ttl(ttl),
	_timeout(timeout)
{
}


ICMPSocket::ICMPSocket(const Socket& socket): 
	Socket(socket)
{
	if (!dynamic_cast<ICMPSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


ICMPSocket::ICMPSocket(SocketImpl* pImpl): 
	Socket(pImpl)
{
	if (!dynamic_cast<ICMPSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


ICMPSocket::~ICMPSocket()
{
}


ICMPSocket& ICMPSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<ICMPSocketImpl*>(socket.impl()))
		Socket::operator = (socket);
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}


int ICMPSocket::sendTo(const SocketAddress& address, int flags)
{
	return impl()->sendTo(0, 0, address, flags);
}


int ICMPSocket::receiveFrom(SocketAddress& address, int flags)
{
	return impl()->receiveFrom(0, 0, address, flags);
}


} } // namespace Poco::Net
