//
// HTTPServerRequest.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HTTPServerRequest.h#1 $
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerRequest
//
// Definition of the HTTPServerRequest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Net_HTTPServerRequest_INCLUDED
#define Net_HTTPServerRequest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/SocketAddress.h"
#include <istream>


namespace Poco {
namespace Net {


class HTTPServerSession;
class HTTPServerResponse;
class HTTPServerParams;


class Net_API HTTPServerRequest: public HTTPRequest
	/// This abstract subclass of HTTPRequest is used for
	/// representing server-side HTTP requests.
	///
	/// A HTTPServerRequest is passed to the
	/// handleRequest() method of HTTPRequestHandler.
{
public:
	HTTPServerRequest();
		/// Creates the HTTPServerRequest

	~HTTPServerRequest();
		/// Destroys the HTTPServerRequest.
		
	virtual std::istream& stream() = 0;
		/// Returns the input stream for reading
		/// the request body.
		///
		/// The stream must be valid until the HTTPServerRequest
		/// object is destroyed.
		
	virtual bool expectContinue() const = 0;
		/// Returns true if the client expects a
		/// 100 Continue response.
		
	virtual const SocketAddress& clientAddress() const = 0;
		/// Returns the client's address.

	virtual const SocketAddress& serverAddress() const = 0;
		/// Returns the server's address.
		
	virtual const HTTPServerParams& serverParams() const = 0;
		/// Returns a reference to the server parameters.

	virtual HTTPServerResponse& response() const = 0;
		/// Returns a reference to the associated response.
};


} } // namespace Poco::Net


#endif // Net_HTTPServerRequest_INCLUDED
