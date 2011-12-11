// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B1B69ECB_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
#define AFX_STDAFX_H__B1B69ECB_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcmn.h>			// MFC support for Windows Common Controls

#include <afxtempl.h>//
#include <afxpriv.h> 
#include <afxole.h> 

#include <string>
#include <map>

#pragma warning(disable:4786)

#include "coretypes.h"
#include "string_util.h"

#include "UnicodeString.h"

	/** @brief Load string from string resources; shortcut for CString::LoadString */
String LoadResString(UINT id);

	/** @brief Wrapper around CMergeApp::TranslateDialog() */
void NTAPI LangTranslateDialog(HWND);

inline int GetClipTcharTextFormat()
{
#ifdef _UNICODE
	return CF_UNICODETEXT;
#else
	return CF_TEXT;
#endif // _UNICODE
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B1B69ECB_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
