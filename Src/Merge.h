/////////////////////////////////////////////////////////////////////////////
//    WinMerge:  an interactive diff/merge utility
//    Copyright (C) 1997  Dean P. Grimm
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
/////////////////////////////////////////////////////////////////////////////
// Merge.h : main header file for the MERGE application
//

#if !defined(AFX_MERGE_H__BBCD4F88_34E4_11D1_BAA6_00A024706EDC__INCLUDED_)
#define AFX_MERGE_H__BBCD4F88_34E4_11D1_BAA6_00A024706EDC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MergeDoc.h"
#include "languageselect.h"

class FileFilterMgr;
struct FileFilter;

/////////////////////////////////////////////////////////////////////////////
// CMergeApp:
// See Merge.cpp for the implementation of this class
//

enum { IDLE_TIMER = 9754 };

class CMergeApp : public CWinApp
{
public:
	BOOL m_bHiliteSyntax;
	BOOL m_bDisableSplash;
	COLORREF	m_clrDiff;				// The difference color
	COLORREF	m_clrSelDiff;			// The selected difference color
	COLORREF	m_clrDiffDeleted;		// The difference deleted color
	COLORREF	m_clrSelDiffDeleted;	// The selected difference deleted color
	COLORREF	m_clrDiffText;			// The difference text color
	COLORREF	m_clrSelDiffText;		// The selected difference text color
	BOOL m_bNeedIdleTimer;
	CMultiDocTemplate* m_pDiffTemplate;
	CMultiDocTemplate* m_pDirTemplate;
	CLanguageSelect m_lang;

	CMergeApp();
	COLORREF GetDiffColor() const { return m_clrDiff; }
	COLORREF GetSelDiffColor() const { return m_clrSelDiff; }
	COLORREF GetDiffDeletedColor() const { return m_clrDiffDeleted; }
	COLORREF GetSelDiffDeletedColor() const { return m_clrSelDiffDeleted; }
	COLORREF GetDiffTextColor() const { return m_clrDiffText; }
	COLORREF GetSelDiffTextColor() const { return m_clrSelDiffText; }
	void SetDiffColor(COLORREF clrValue) { m_clrDiff = clrValue; }
	void SetSelDiffColor(COLORREF clrValue) { m_clrSelDiff = clrValue; }
	void SetDiffDeletedColor(COLORREF clrValue) { m_clrDiffDeleted = clrValue; }
	void SetSelDiffDeletedColor(COLORREF clrValue) { m_clrSelDiffDeleted = clrValue; }
	void SetDiffTextColor(COLORREF clrValue) { m_clrDiffText = clrValue; }
	void SetSelDiffTextColor(COLORREF clrValue) { m_clrSelDiffText = clrValue; }
	void SetNeedIdleTimer();
	CString GetFileFilterName() const { return m_sFileFilterName; }
	void SetFileFilterName(LPCTSTR szFileFilterName);
	void EditFileFilter(LPCTSTR szFileFilterName);
	void GetFileFilterNameList(CStringList & filefilters, CString & selected) const;

	// implement file/directory filtering, because app currently holds the filter manager
	BOOL includeFile(LPCTSTR szFileName);
	BOOL includeDir(LPCTSTR szDirName);

// Implementation
protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMergeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void InitializeFileFilters();
	void ParseArgs(CStringArray & files, UINT & nFiles, BOOL & recurse, DWORD & dwLeftFlags, DWORD & dwRightFlags);


	//{{AFX_MSG(CMergeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnViewLanguage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bEscCloses;
	FileFilter * m_currentFilter;
	FileFilterMgr * m_fileFilterMgr;
	CString m_sFileFilterName;
};

extern CMergeApp theApp;

/////////////////////////////////////////////////////////////////////////////
CMergeDoc *GetDoc();
BOOL SelectFile(CString& path, LPCTSTR root_path = NULL, 
			 LPCTSTR title = _T("Open"), 
			 UINT filterid =0,
			 BOOL is_open =TRUE);

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MERGE_H__BBCD4F88_34E4_11D1_BAA6_00A024706EDC__INCLUDED_)
