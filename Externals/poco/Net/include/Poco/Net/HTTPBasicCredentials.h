//
// HTTPBasicCredentials.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HTTPBasicCredentials.h#3 $
//
// Library: Net
// Package: HTTP
// Module:  HTTPBasicCredentials
//
// Definition of the HTTPBasicCredentials class.
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


#ifndef Net_HTTPBasicCredentials_INCLUDED
#define Net_HTTPBasicCredentials_INCLUDED


#include "Poco/Net/Net.h"


namespace Poco {
namespace Net {


class HTTPRequest;


class Net_API HTTPBasicCredentials
	/// This is a utility class for working with
	/// HTTP Basic Authentication in HTTPRequest
	/// objects.
{
public:
	HTTPBasicCredentials();
		/// Creates an empty HTTPBasicCredentials object.
		
	HTTPBasicCredentials(const std::string& username, const std::string& password);
		/// Creates a HTTPBasicCredentials object with the given username and password.

	explicit HTTPBasicCredentials(const HTTPRequest& request);
		/// Creates a HTTPBasicCredentials object with the authentication information
		/// from the given request.
		///
		/// Throws a NotAuthenticatedException if the request does
		/// not contain basic authentication information.

	explicit HTTPBasicCredentials(const std::string& authInfo);
		/// Creates a HTTPBasicCredentials object with the authentication information
		/// in the given string. The authentication information can be extracted
		/// from a HTTPRequest object by calling HTTPRequest::getCredentials().

	~HTTPBasicCredentials();
		/// Destroys the HTTPBasicCredentials.

	void setUsername(const std::string& username);
		/// Sets the username.
		
	const std::string& getUsername() const;
		/// Returns the username.
		
	void setPassword(const std::string& password);
		/// Sets the password.
		
	const std::string& getPassword() const;
		/// Returns the password.
		
	void authenticate(HTTPRequest& request);
		/// Adds authentication information to the given HTTPRequest.

	static const std::string SCHEME;

protected:
	void parseAuthInfo(const std::string& authInfo);
		/// Extracts username and password from Basic authentication info
		/// by base64-decoding authInfo and splitting the resulting
		/// string at the ':' delimiter.

private:
	HTTPBasicCredentials(const HTTPBasicCredentials&);
	HTTPBasicCredentials& operator = (const HTTPBasicCredentials&);
	
	std::string _username;
	std::string _password;
};


//
// inlines
//
inline const std::string& HTTPBasicCredentials::getUsername() const
{
	return _username;
}


inline const std::string& HTTPBasicCredentials::getPassword() const
{
	return _password;
}


} } // namespace Poco::Net


#endif // Net_HTTPBasicCredentials_INCLUDED
