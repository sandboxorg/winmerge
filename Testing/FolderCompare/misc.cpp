#include <iostream>
#include "UnicodeString.h"
#include "LogFile.h"
#include "unicoder.h"

CLogFile g_log;

String GetSysError(int nerr /* =-1 */)
{
	if (nerr == -1)
		nerr = GetLastError();
	LPVOID lpMsgBuf;
	String str = _T("?");
	if (FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		nerr,
		0, // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		))
	{
		str = (LPCTSTR)lpMsgBuf;
	}
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return str;
}

String LoadResString(unsigned id)
{
	return _T("Nothing");
}

CLogFile * GetLog()
{
	return &g_log;
}

void LogErrorStringUTF8(const std::string& sz)
{
	std::cout << sz;
}

void LogErrorString(const String& sz)
{
	std::cout << ucr::toUTF8(sz);
}

void AppErrorMessageBox(const String& msg)
{
	MessageBox(NULL, msg.c_str(), NULL, MB_ICONSTOP);
}
