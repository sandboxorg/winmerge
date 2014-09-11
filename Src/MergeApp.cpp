#include "StdAfx.h"
#include "MergeApp.h"
#include "Merge.h"
#include "version.h"
#include "paths.h"
#include "Environment.h"
#include "Constants.h"
#include "unicoder.h"

// Get user language description of error, if available
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

/**
 * @brief Get Options Manager.
 * @return Pointer to OptionsMgr.
 */
COptionsMgr * GetOptionsMgr()
{
	CMergeApp *pApp = static_cast<CMergeApp *>(AfxGetApp());
	return pApp->GetMergeOptionsMgr();
}

// Send message to log and debug window
void LogErrorString(const String& sz)
{
	if (sz.empty()) return;
	CString now = COleDateTime::GetCurrentTime().Format();
	TRACE(_T("%s: %s\n"), (LPCTSTR)now, sz.c_str());
}

// Send message to log and debug window
void LogErrorStringUTF8(const std::string& sz)
{
	if (sz.empty()) return;
	String str = ucr::toTString(sz);
	CString now = COleDateTime::GetCurrentTime().Format();
	TRACE(_T("%s: %s\n"), (LPCTSTR)now, str.c_str());
}

/**
 * @brief Load string resource and return as CString.
 * @param [in] id Resource string ID.
 * @return Resource string as CString.
 */
String LoadResString(unsigned id)
{
	return theApp.LoadString(id);
}

String tr(const std::string &str)
{
	String translated_str;
	theApp.TranslateString(str, translated_str);
	return translated_str;
}

void AppErrorMessageBox(const String& msg)
{
	AppMsgBox::error(msg);
}

namespace AppMsgBox
{

namespace detail
{
	int convert_to_winflags(int flags)
	{
		int newflags = 0;

		if ((flags & (YES | NO | CANCEL)) == (YES | NO | CANCEL)) newflags |= MB_YESNOCANCEL;
		else if ((flags & (YES | NO)) == (YES | NO)) newflags |= MB_YESNO;
		else if ((flags & (OK | CANCEL)) == (OK | CANCEL)) newflags |= MB_OKCANCEL;
		else if ((flags & OK) == OK) newflags |= MB_OK;
	
		if (flags & YES_TO_ALL) newflags |= MB_YES_TO_ALL;
		if (flags & DONT_DISPLAY_AGAIN) newflags |= MB_DONT_DISPLAY_AGAIN;

		return newflags;
	}

	int convert_resp(int resp)
	{
		switch (resp)
		{
		case IDOK:
			return OK;
		case IDCANCEL:
			return CANCEL;
		case IDNO:
			return NO;
		case IDYES:
			return YES;
		case IDYESTOALL:
			return YES_TO_ALL;
		default:
			return OK;
		}
	}
}

int error(const String& msg, int type)
{
	return detail::convert_resp(AfxMessageBox(msg.c_str(), detail::convert_to_winflags(type) | MB_ICONSTOP));
}

int warning(const String& msg, int type)
{
	return detail::convert_resp(AfxMessageBox(msg.c_str(), detail::convert_to_winflags(type) | MB_ICONWARNING));
}

int information(const String& msg, int type)
{
	return detail::convert_resp(AfxMessageBox(msg.c_str(), detail::convert_to_winflags(type) | MB_ICONINFORMATION));
}

}

AboutInfo::AboutInfo()
{
	CVersionInfo verinfo;
	version = string_format_string1(_("Version %1"), verinfo.GetProductVersion());

#ifdef _UNICODE
	version += _T(" ");
	version += _("Unicode");
#endif

#if defined _M_IX86
	version += _T(" x86");
#elif defined _M_IA64
	version += _T(" IA64");
#elif defined _M_X64
	version += _T(" ");
	version += _("X64");
#endif

	copyright = verinfo.GetLegalCopyright();

	private_build = verinfo.GetPrivateBuild();
	if (!private_build.empty())
	{
		private_build = string_format_string1(_("Private Build: %1"), private_build);
	}

	website = WinMergeURL;
}
