//
// Debugger.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Debugger.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  Debugger
//
// Definition of the Debugger class.
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


#ifndef Foundation_Debugger_INCLUDED
#define Foundation_Debugger_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class Foundation_API Debugger
	/// The Debugger class provides an interface to the debugger.
	/// The presence of a debugger can be checked for,
	/// messages can be written to the debugger's log window
	/// and a break into the debugger can be enforced.
	/// The methods only work if the program is compiled
	/// in debug mode (the macro _DEBUG is defined).
{
public:
	static bool isAvailable();
		/// Returns true if a debugger is available, false otherwise.
		/// On Windows, this function uses the IsDebuggerPresent()
		/// function.
		/// On Unix, this function returns true if the environment
		/// variable POCO_ENABLE_DEBUGGER is set.
		/// On OpenVMS, this function always returns true in debug,
		/// mode, false otherwise.

	static void message(const std::string& msg);
		/// Writes a message to the debugger log, if available, otherwise to
		/// standard error output.

	static void message(const std::string& msg, const char* file, int line);
		/// Writes a message to the debugger log, if available, otherwise to
		/// standard error output.

	static void enter();
		/// Breaks into the debugger, if it is available.
		/// On Windows, this is done using the DebugBreak() function.
		/// On Unix, the SIGINT signal is raised.
		/// On OpenVMS, the SS$_DEBUG signal is raised.

	static void enter(const std::string& msg);
		/// Writes a debug message to the debugger log and breaks into it.

	static void enter(const std::string& msg, const char* file, int line);
		/// Writes a debug message to the debugger log and breaks into it.

	static void enter(const char* file, int line);
		/// Writes a debug message to the debugger log and breaks into it.
};


} // namespace Poco


#endif // Foundation_Debugger_INCLUDED
