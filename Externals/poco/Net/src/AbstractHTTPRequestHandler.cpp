//
// AbstractHTTPRequestHandler.cpp
//
// $Id: //poco/1.4/Net/src/AbstractHTTPRequestHandler.cpp#1 $
//
// Library: Net
// Package: HTTPServer
// Module:  AbstractHTTPRequestHandler
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Net/AbstractHTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"


using Poco::NumberFormatter;


namespace Poco {
namespace Net {


AbstractHTTPRequestHandler::AbstractHTTPRequestHandler():
	_pRequest(0),
	_pResponse(0),
	_pForm(0)
{
}


AbstractHTTPRequestHandler::~AbstractHTTPRequestHandler()
{
	delete _pForm;
}


void AbstractHTTPRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	_pRequest  = &request;
	_pResponse = &response;
	if (authenticate())
	{
		try
		{
			run();
		}
		catch (Poco::Exception& exc)
		{
			if (!response.sent())
			{
				sendErrorResponse(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.displayText());
			}
		}
		catch (std::exception& exc)
		{
			if (!response.sent())
			{
				sendErrorResponse(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.what());
			}
		}
	}
	else
	{
		sendErrorResponse(HTTPResponse::HTTP_UNAUTHORIZED, "");
	}
}


bool AbstractHTTPRequestHandler::authenticate()
{
	return true;
}


HTMLForm& AbstractHTTPRequestHandler::form()
{
	if (!_pForm)
		_pForm = new HTMLForm(request(), request().stream());

	return *_pForm;
}


void AbstractHTTPRequestHandler::sendErrorResponse(HTTPResponse::HTTPStatus status, const std::string& message)
{
	response().setStatusAndReason(status);
	std::string statusAndReason(NumberFormatter::format(static_cast<int>(response().getStatus())));
	statusAndReason += " - ";
	statusAndReason += response().getReason();
	std::string page("<HTML><HEAD><TITLE>");
	page += statusAndReason;
	page += "</TITLE></HEAD><BODY><H1>";
	page += statusAndReason;
	page += "</H1>";
	page += "<P>";
	page += message;
	page += "</P></BODY></HTML>";
	response().sendBuffer(page.data(), page.size());
}


} } // namespace Poco::Net
