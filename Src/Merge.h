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
/** 
 * @file  Merge.h
 *
 * @brief main header file for the MERGE application
 *
 */
// ID line follows -- this is updated by SVN
// $Id$

#if !defined(AFX_MERGE_H__BBCD4F88_34E4_11D1_BAA6_00A024706EDC__INCLUDED_)
#define AFX_MERGE_H__BBCD4F88_34E4_11D1_BAA6_00A024706EDC__INCLUDED_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <boost/scoped_ptr.hpp>
#include "resource.h"       // main symbols
#include "MergeApp.h"
#include "MergeDoc.h"
#include "OptionsMgr.h"
#include "RegOptionsMgr.h"
#include "FileFilterHelper.h"

struct FileFilter;
class CAssureScriptsForThread;
class CMainFrame;
class CLanguageSelect;
class MergeCmdLineInfo;
class CLogFile;
class ProjectFile;

/////////////////////////////////////////////////////////////////////////////
// CMergeApp:
// See Merge.cpp for the implementation of this class
//

enum { IDLE_TIMER = 9754 };

/** 
 * @brief WinMerge application class
 */
class CMergeApp : public CWinApp
{
public:
	BOOL m_bNeedIdleTimer;
	CMultiDocTemplate* m_pOpenTemplate;
	CMultiDocTemplate* m_pDiffTemplate;
	CMultiDocTemplate* m_pHexMergeTemplate;
	CMultiDocTemplate* m_pDirTemplate;
	boost::scoped_ptr<CLanguageSelect> m_pLangDlg;
	FileFilterHelper m_globalFileFilter;

	WORD GetLangId() const;
	void SetIndicators(CStatusBar &, const UINT *, int) const;
	void TranslateMenu(HMENU) const;
	void TranslateDialog(HWND) const;
	String LoadString(UINT) const;
	std::wstring LoadDialogCaption(LPCTSTR) const;

	CMergeApp();
	~CMergeApp();

public:
	void AddToRecentProjectsMRU(LPCTSTR sPathName);
	void SetNeedIdleTimer();
	void SetLastCompareResult(int nResult) { m_nLastCompareResult = nResult; }
	String GetDefaultEditor();
	String GetDefaultFilterUserPath(BOOL bCreate = FALSE);

	COptionsMgr * GetMergeOptionsMgr() { return static_cast<COptionsMgr *> (m_pOptions.get()); }
	void OptionsInit();
	void ResetOptions() { OptionsInit(); }
	void SetFontDefaults();

	CLogFile * GetMergeLog() { return m_pLog.get(); }

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
	BOOL ParseArgsAndDoOpen(MergeCmdLineInfo& cmdInfo, CMainFrame* pMainFrame);
	void UpdateDefaultCodepage(int cpDefaultMode, int cpCustomCodepage);

	// End MergeArgs.cpp

	bool LoadProjectFile(const String& sProject, ProjectFile &project);
	bool SaveProjectFile(const String& sProject, const ProjectFile &project);
	bool LoadAndOpenProjectFile(const String& sFilepath);
	bool IsProjectFile(const String& filepath) const;

	void ReloadMenu();

	//@{
	/**
	 * @name Active operations counter.
	 * These functions implement counter for active operations. We need to
	 * track active operations during whose user cannot exit the application.
	 * E.g. copying files in folder compare is such an operation.
	 */
	/**
	 * Increment the active operation counter.
	 */
	void AddOperation() { InterlockedIncrement(&m_nActiveOperations); }
	/**
	 * Decrement the active operation counter.
	 */
	void RemoveOperation()
	{
		ASSERT(m_nActiveOperations > 0);
		InterlockedDecrement( &m_nActiveOperations);
	}
	/**
	 * Get the active operations count.
	 */
	LONG GetActiveOperations() const { return m_nActiveOperations; }
	//@}

	//{{AFX_MSG(CMergeApp)
	afx_msg BOOL OnOpenRecentFile(UINT nID);
	afx_msg void OnAppAbout();
	afx_msg void OnViewLanguage();
	afx_msg void OnUpdateViewLanguage(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	boost::scoped_ptr<CRegOptionsMgr> m_pOptions;
	CAssureScriptsForThread * m_mainThreadScripts;
	boost::scoped_ptr<CLogFile> m_pLog;
	int m_nLastCompareResult;
	bool m_bNonInteractive;
	LONG m_nActiveOperations; /**< Active operations count. */
};

extern CMergeApp theApp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MERGE_H__BBCD4F88_34E4_11D1_BAA6_00A024706EDC__INCLUDED_)
