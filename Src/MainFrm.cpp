/////////////////////////////////////////////////////////////////////////////
//    WinMerge:  an interactive diff/merge utility
//    Copyright (C) 1997-2000  Thingamahoochie Software
//    Author: Dean Grimm
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
 * @file  MainFrm.cpp
 *
 * @brief Implementation of the CMainFrame class
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#include "stdafx.h"
#include "Merge.h"

#include <direct.h>
#include "MainFrm.h"
#include "DirFrame.h"		// Include type information
#include "ChildFrm.h"
#include "DirView.h"
#include "DirDoc.h"
#include "OpenDlg.h"
#include "MergeEditView.h"
#include "MergeDiffDetailView.h"

#include "diff.h"
#include "coretools.h"
#include "Splash.h"
#include "VssPrompt.h"
#include "CCPrompt.h"
#include "PropVss.h"
#include "PropGeneral.h"
#include "PropFilter.h"
#include "PropColors.h"
#include "PropRegistry.h"
#include "PropCompare.h"
#include "PropEditor.h"
#include "RegKey.h"
#include "logfile.h"
#include "ssapi.h"      // BSP - Includes for Visual Source Safe COM interface
#include "multimon.h"
#include "paths.h"
#include "WaitStatusCursor.h"
#include "PatchTool.h"
#include "FileTransform.h"
#include "SelectUnpackerDlg.h"
#include "files.h"
#include "ConfigLog.h"
#include "7zCommon.h"
#include <shlwapi.h>
#include "OptionsMgr.h"
#include "OptionsDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLogFile gLog;
CMainFrame *mf = NULL;

// add a 
static void add_regexp PARAMS((struct regexp_list **, char const*));
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_OPTIONS_SHOWDIFFERENT, OnOptionsShowDifferent)
	ON_COMMAND(ID_OPTIONS_SHOWIDENTICAL, OnOptionsShowIdentical)
	ON_COMMAND(ID_OPTIONS_SHOWUNIQUELEFT, OnOptionsShowUniqueLeft)
	ON_COMMAND(ID_OPTIONS_SHOWUNIQUERIGHT, OnOptionsShowUniqueRight)
	ON_COMMAND(ID_OPTIONS_SHOWBINARIES, OnOptionsShowBinaries)
	ON_COMMAND(ID_OPTIONS_SHOWSKIPPED, OnOptionsShowSkipped)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWDIFFERENT, OnUpdateOptionsShowdifferent)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWIDENTICAL, OnUpdateOptionsShowidentical)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWUNIQUELEFT, OnUpdateOptionsShowuniqueleft)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWUNIQUERIGHT, OnUpdateOptionsShowuniqueright)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWBINARIES, OnUpdateOptionsShowBinaries)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWSKIPPED, OnUpdateOptionsShowSkipped)
	ON_WM_CREATE()
	ON_WM_MENUCHAR()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_HIDE_BACKUP_FILES, OnUpdateHideBackupFiles)
	ON_COMMAND(ID_HELP_GNULICENSE, OnHelpGnulicense)
	ON_COMMAND(ID_OPTIONS, OnOptions)
	ON_COMMAND(ID_HIDE_BACKUP_FILES, OnHideBackupFiles)
	ON_COMMAND(ID_VIEW_SELECTFONT, OnViewSelectfont)
	ON_COMMAND(ID_VIEW_USEDEFAULTFONT, OnViewUsedefaultfont)
	ON_UPDATE_COMMAND_UI(ID_VIEW_USEDEFAULTFONT, OnUpdateViewUsedefaultfont)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_UPDATE_COMMAND_UI(ID_HELP_CONTENTS, OnUpdateHelpContents)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_WHITESPACE, OnViewWhitespace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WHITESPACE, OnUpdateViewWhitespace)
	ON_COMMAND(ID_TOOLS_GENERATEPATCH, OnToolsGeneratePatch)
	ON_WM_DROPFILES()
	ON_WM_SETCURSOR()
	ON_COMMAND_RANGE(ID_UNPACK_MANUAL, ID_UNPACK_AUTO, OnPluginUnpackMode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_UNPACK_MANUAL, ID_UNPACK_AUTO, OnUpdatePluginUnpackMode)
	ON_COMMAND(ID_HELP_GETCONFIG, OnSaveConfigData)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 * @brief MainFrame statusbar panels/indicators
 */
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

/**
 * @brief MainFrame constructor. Loads settings from registry.
 * @todo Preference for logging?
 */
CMainFrame::CMainFrame()
{
#if defined (_DEBUG) || defined (ENABLE_LOG)
	gLog.SetFile(_T("WinMerge.log"));
	gLog.EnableLogging(TRUE);
	// Not interested about compare data (very noisy!)
	gLog.SetMaskLevel(LOGLEVEL::LALL & ~LOGLEVEL::LCOMPAREDATA);
#endif

// Only log errors and warnings for release!
//#ifndef _DEBUG
//	gLog.SetMaskLevel(LOGLEVEL::LERROR | LOGLEVEL::LWARNING);
//#endif

	m_bFontSpecified=FALSE;
	m_strSaveAsPath = _T("");
	m_bFirstTime = TRUE;

	m_options.SetRegRootKey(_T("Thingamahoochie\\WinMerge\\"));

	// Initialise options (name, default value)
	m_options.InitOption(OPT_SHOW_UNIQUE_LEFT, TRUE);
	m_options.InitOption(OPT_SHOW_UNIQUE_RIGHT, TRUE);
	m_options.InitOption(OPT_SHOW_DIFFERENT, TRUE);
	m_options.InitOption(OPT_SHOW_IDENTICAL, TRUE);
	m_options.InitOption(OPT_SHOW_BINARIES, TRUE);
	m_options.InitOption(OPT_SHOW_SKIPPED, TRUE);
	m_options.InitOption(OPT_HIDE_BACKUP, TRUE);

	m_options.InitOption(OPT_CREATE_BACKUPS, TRUE);
	m_options.InitOption(OPT_VIEW_WHITESPACE, FALSE);
	m_options.InitOption(OPT_SCROLL_TO_FIRST, FALSE);

	m_options.InitOption(OPT_AUTOMATIC_RESCAN, FALSE);
	m_options.InitOption(OPT_ALLOW_MIXED_EOL, FALSE);

	m_options.InitOption(OPT_USE_RECYCLE_BIN, TRUE);

	m_bShowErrors = TRUE;
	m_nVerSys = theApp.GetProfileInt(_T("Settings"), _T("VersionSystem"), 0);
	m_strVssProjectBase = theApp.GetProfileString(_T("Settings"), _T("VssProject"), _T(""));
	m_strVssUser = theApp.GetProfileString(_T("Settings"), _T("VssUser"), _T(""));
	m_strVssPassword = theApp.GetProfileString(_T("Settings"), _T("VssPassword"), _T(""));
	m_strVssPath = theApp.GetProfileString(_T("Settings"), _T("VssPath"), _T(""));
	m_strVssDatabase = theApp.GetProfileString(_T("Settings"), _T("VssDatabase"),_T(""));
	m_nTabSize = theApp.GetProfileInt(_T("Settings"), _T("TabSize"), 4);
	m_nTabType = theApp.GetProfileInt(_T("Settings"), _T("TabType"), 0);
	m_bIgnoreRegExp = theApp.GetProfileInt(_T("Settings"), _T("IgnoreRegExp"), FALSE);
	m_sPattern = theApp.GetProfileString(_T("Settings"), _T("RegExps"), NULL);
	theApp.SetFileFilterPath(theApp.GetProfileString(_T("Settings"), _T("FileFilterPath"), _T("")));
	m_sExtEditorPath = theApp.GetProfileString(_T("Settings"), _T("ExternalEditor"), _T(""));
	m_bUnpackerMode = theApp.GetProfileInt(_T("Settings"), _T("UnpackerMode"), UNPACK_MANUAL);
	m_bPredifferMode = theApp.GetProfileInt(_T("Settings"), _T("PredifferMode"), PREDIFF_MANUAL);

    // Load the compare method and protect it from manual modifs in registry!
    m_nCompMethod = theApp.GetProfileInt(_T("Settings"),_T("CompMethod"),0);
    if (m_nCompMethod > 1)
        m_nCompMethod = 0;

	m_bReuseDirDoc = TRUE;
	// TODO: read preference for logging

	if (m_sExtEditorPath.IsEmpty())
		m_sExtEditorPath = GetDefaultEditor();

	if (m_strVssPath.IsEmpty())
	{
		CRegKeyEx reg;
		if (reg.QueryRegMachine(_T("SOFTWARE\\Microsoft\\SourceSafe")))
		{
			TCHAR temp[_MAX_PATH] = {0};
			reg.ReadChars(_T("SCCServerPath"), temp, _MAX_PATH, _T(""));
			CString spath = GetPathOnly(temp);
			m_strVssPath = spath + _T("\\Ss.exe");
		}
	}
}

CMainFrame::~CMainFrame()
{
	// destroy the reg expression list
	FreeRegExpList();
	// Delete all temporary folders belonging to this process
	CTempPath(0);
}

// This is a bridge to implement IStatusDisplay for WaitStatusCursor
// by forwarding all calls to the main frame
class StatusDisplay : public IStatusDisplay
{
public:
	StatusDisplay() : m_pfrm(0) { }
	void SetFrame(CMainFrame * frm) { m_pfrm = frm; }
// Implement IStatusDisplay
	virtual CString BeginStatus(LPCTSTR str) { return m_pfrm->SetStatus(str); }
	virtual void ChangeStatus(LPCTSTR str) { m_pfrm->SetStatus(str); }
	virtual void EndStatus(LPCTSTR str, LPCTSTR oldstr) { m_pfrm->SetStatus(oldstr); }

protected:
	CMainFrame * m_pfrm;
};

static StatusDisplay myStatusDisplay;

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	mf = this;
	
	// build the initial reg expression list
	RebuildRegExpList();
	GetFontProperties();
	
	if (!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_GRIPPER|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo(1, ID_DIFFNUM, 0, 150); 

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	// Start handling status messages from CustomStatusCursors
	myStatusDisplay.SetFrame(this);
	CustomStatusCursor::SetStatusDisplay(&myStatusDisplay);

	return 0;
}

HMENU CMainFrame::NewDefaultMenu()
{
	m_default.LoadMenu(IDR_MAINFRAME);
	m_default.LoadToolbar(IDR_MAINFRAME);
	return(m_default.Detach());
}

//This handler ensures that keyboard shortcuts work
LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, 
	CMenu* pMenu) 
{
	LRESULT lresult;
	if(m_default.IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
	return(lresult);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnFileOpen() 
{
	DoFileOpen();
}

/// Creates new MergeDoc instance and shows documents
void CMainFrame::ShowMergeDoc(CDirDoc * pDirDoc, LPCTSTR szLeft, LPCTSTR szRight,
	BOOL bROLeft, BOOL bRORight,  int cpleft, int cpright,
	PackingInfo * infoUnpacker /*= NULL*/)
{
	BOOL docNull;
	BOOL bOpenSuccess = FALSE;
	CMergeDoc * pMergeDoc = GetMergeDocToShow(pDirDoc, &docNull);

	ASSERT(pMergeDoc);		// must ASSERT to get an answer to the question below ;-)
	if (!pMergeDoc) return; // when does this happen ?

	// if an unpacker is selected, it must be used during LoadFromFile
	pMergeDoc->SetUnpacker(infoUnpacker);

	bOpenSuccess = pMergeDoc->OpenDocs(szLeft, szRight,
			bROLeft, bRORight, cpleft, cpright);

	if (bOpenSuccess)
	{
		if (docNull)
		{
			CWnd* pWnd = pMergeDoc->GetParentFrame();
			MDIActivate(pWnd);
		}
		else
			MDINext();
	}
}

void CMainFrame::RedisplayAllDirDocs()
{
	DirDocList dirdocs;
	GetAllDirDocs(&dirdocs);
	while (!dirdocs.IsEmpty())
	{
		CDirDoc * pDirDoc = dirdocs.RemoveHead();
		pDirDoc->Redisplay();
	}
}

/**
 * @brief Show/Hide different files/directories
 */
void CMainFrame::OnOptionsShowDifferent() 
{
	varprop::VariantValue val;
	val = m_options.Get(OPT_SHOW_DIFFERENT);
	val.SetInt(val.getInt() ? 0 : 1);
	m_options.SaveOption(OPT_SHOW_DIFFERENT, val);
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide identical files/directories
 */
void CMainFrame::OnOptionsShowIdentical() 
{
	varprop::VariantValue val;
	val = m_options.Get(OPT_SHOW_IDENTICAL);
	val.SetInt(val.getInt() ? 0 : 1);
	m_options.SaveOption(OPT_SHOW_IDENTICAL, val);
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide left-only files/directories
 */
void CMainFrame::OnOptionsShowUniqueLeft() 
{
	varprop::VariantValue val;
	val = m_options.Get(OPT_SHOW_UNIQUE_LEFT);
	val.SetInt(val.getInt() ? 0 : 1);
	m_options.SaveOption(OPT_SHOW_UNIQUE_LEFT, val);
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide right-only files/directories
 */
void CMainFrame::OnOptionsShowUniqueRight() 
{
	varprop::VariantValue val;
	val = m_options.Get(OPT_SHOW_UNIQUE_RIGHT);
	val.SetInt(val.getInt() ? 0 : 1);
	m_options.SaveOption(OPT_SHOW_UNIQUE_RIGHT, val);
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide binary files
 */
void CMainFrame::OnOptionsShowBinaries()
{
	varprop::VariantValue val;
	val = m_options.Get(OPT_SHOW_BINARIES);
	val.SetInt(val.getInt() ? 0 : 1);
	m_options.SaveOption(OPT_SHOW_BINARIES, val);
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide skipped files/directories
 */
void CMainFrame::OnOptionsShowSkipped()
{
	varprop::VariantValue val;
	val = m_options.Get(OPT_SHOW_SKIPPED);
	val.SetInt(val.getInt() ? 0 : 1);
	m_options.SaveOption(OPT_SHOW_SKIPPED, val);
	RedisplayAllDirDocs();
}

void CMainFrame::OnUpdateOptionsShowdifferent(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_SHOW_DIFFERENT));
}

void CMainFrame::OnUpdateOptionsShowidentical(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_SHOW_IDENTICAL));
}

void CMainFrame::OnUpdateOptionsShowuniqueleft(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_SHOW_UNIQUE_LEFT));
}

void CMainFrame::OnUpdateOptionsShowuniqueright(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_SHOW_UNIQUE_RIGHT));
}

void CMainFrame::OnUpdateOptionsShowBinaries(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_SHOW_BINARIES));
}

void CMainFrame::OnUpdateOptionsShowSkipped(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_SHOW_SKIPPED));
}

/**
 * @brief Show/Hide backup files
 */
void CMainFrame::OnHideBackupFiles() 
{
	varprop::VariantValue val;
	val = m_options.Get(OPT_HIDE_BACKUP);
	val.SetInt(val.getInt() ? 0 : 1);
	m_options.SaveOption(OPT_HIDE_BACKUP, val);
	RedisplayAllDirDocs();
}

void CMainFrame::OnUpdateHideBackupFiles(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_HIDE_BACKUP));
}

/**
 * @brief Show GNU licence information in notepad (local file) or in Web Browser
 */
void CMainFrame::OnHelpGnulicense() 
{
	CString spath = GetModulePath() + _T("\\Copying");
	CString url = _T("http://www.gnu.org/copyleft/gpl.html");
	
	CFileStatus status;
	if (CFile::GetStatus(spath, status))
		ShellExecute(m_hWnd, _T("open"), _T("notepad.exe"), spath, NULL, SW_SHOWNORMAL);
	else
		ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
}

/**
 * @brief Reads words from a file deliminated by charset
 *
 * Reads words from a file deliminated by charset with one slight twist.
 * If the next char in the file to be read is one of the characters inside the delimiter,
 * then the word returned will be a word consisting only of delimiters.
 * 
 * @note pfile is not incremented past the word returned
 */
BOOL CMainFrame::GetWordFile(HANDLE pfile, TCHAR * buffer, TCHAR * charset)
{
	TCHAR cbuffer[1024];
	TCHAR ctemp = '\0';
	TCHAR * pcharset;
	int buffercount = 0;
	DWORD numread = sizeof(ctemp);
	BOOL delimword = FALSE;
	BOOL FirstRead = FALSE;
	BOOL delimMatch = FALSE;

	ZeroMemory(&cbuffer, sizeof(cbuffer));
	
	while (numread == sizeof(ctemp) && pfile != INVALID_HANDLE_VALUE && buffercount < sizeof(cbuffer))
	{
		if (ReadFile(pfile, (LPVOID)&ctemp, sizeof(ctemp), &numread, NULL) == TRUE)
		{
			//first read:
			if (!FirstRead && charset)
			{
				for (pcharset = charset; *pcharset; pcharset++)
				{
					if (ctemp == *pcharset)
						break;
				}
				if (*pcharset != NULL)//means that cbuffer[0] is a delimiter character
					delimword = TRUE;
				FirstRead = TRUE;
			}

			if (numread == sizeof(ctemp))
			{
				if (!charset)
				{
					if (ctemp != ' ' && ctemp != '\n' && ctemp != '\t' && ctemp != '\r')
					{
						cbuffer[buffercount] = ctemp;
						buffercount++;
					}
					else
					{
						SetFilePointer(pfile,-1,NULL,FILE_CURRENT);
						break;
					}
				}
				else if (delimword == FALSE)
				{
					for (pcharset = charset;*pcharset;pcharset++)
					{						
						//if next char is equal to a delimiter or we want delimwords stop the adding
						if (ctemp == *pcharset)
						{
							SetFilePointer(pfile,-1,NULL,FILE_CURRENT);
							break;
						}
					}
					if (*pcharset == NULL)
					{
						cbuffer[buffercount] = ctemp;
						buffercount++;
					}
					else
						break;
				}
				else if (delimword == TRUE)
				{
					delimMatch = FALSE;
					for (pcharset = charset;*pcharset;pcharset++)
					{						
						//if next char is equal to a delimiter or we want delimwords stop the adding
						if (ctemp == *pcharset)
						{
							delimMatch = TRUE;
							break;
						}
					}
					if (delimMatch == TRUE)
					{
						cbuffer[buffercount] = ctemp;
						buffercount++;
					}
					else
					{
						SetFilePointer(pfile,-1,NULL,FILE_CURRENT);
						break;
					}
				}
			}
		}
		else
		{
			DWORD err = GetLastError();
			return FALSE;
		}
	}
	_tcscpy(buffer, cbuffer);
	if (pfile == INVALID_HANDLE_VALUE || buffercount >= sizeof(cbuffer) || numread == 0)
		return FALSE;
	return TRUE;
}

/**
* @brief Check if file is read-only and save to version control if one is used.
* @param strSavePath Path where to save including filename
* @return Tells if caller can continue (no errors happened)
* @note If user selects "Cancel" FALSE is returned and caller must assume file
* is not saved.
* @sa SaveToVersionControl()
*/
BOOL CMainFrame::CheckSavePath(CString& strSavePath)
{
	CFileStatus status;
	UINT userChoice = 0;
	BOOL bRetVal = TRUE;
	BOOL bFileRO = FALSE;
	BOOL bFileExists = FALSE;
	CString s;

	bFileRO = files_isFileReadOnly(strSavePath, &bFileExists);
	
	if (bFileExists && bFileRO)
	{
		// Version control system used?
		if (m_nVerSys > 0)
			bRetVal = SaveToVersionControl(strSavePath);
		else
		{
			CString title;
			VERIFY(title.LoadString(IDS_SAVE_AS_TITLE));
			
			// Prompt for user choice
			AfxFormatString1(s, IDS_SAVEREADONLY_FMT, strSavePath);
			userChoice = AfxMessageBox(s, MB_YESNOCANCEL |
					MB_ICONQUESTION | MB_DEFBUTTON2);
			switch (userChoice)
			{
			// Overwrite read-only file
			case IDYES:
				CFile::GetStatus(strSavePath, status);
				status.m_mtime = 0;		// Avoid unwanted changes
				status.m_attribute &= ~CFile::Attribute::readOnly;
				CFile::SetStatus(strSavePath, status);
				break;
			
			// Save to new filename
			case IDNO:
				if (SelectFile(s, strSavePath, title, NULL, FALSE))
					strSavePath = s;
				else
					bRetVal = FALSE;
				break;
			
			// Cancel saving
			case IDCANCEL:
				bRetVal = FALSE;
				break;
			}
		}
	}
	return bRetVal;
}

/**
* @brief Saves file to selected version control system
*
* @param strSavePath Path where to save including filename
*
* @return Tells if caller can continue (no errors happened)
*
* @note
*
* @sa CheckSavePath()
*
*/
BOOL CMainFrame::SaveToVersionControl(CString& strSavePath)
{
	CFileStatus status;
	CString s;
	UINT userChoice = 0;

	switch(m_nVerSys)
	{
	case 0:	//no versioning system
		// Already handled in CheckSavePath()
		break;
	case 1:	// Visual Source Safe
	{
			// prompt for user choice
		CVssPrompt dlg;
			dlg.m_strMessage.FormatMessage(IDS_SAVE_FMT, strSavePath);
		dlg.m_strProject = m_strVssProjectBase;
		dlg.m_strUser = m_strVssUser;          // BSP - Add VSS user name to dialog box
		dlg.m_strPassword = m_strVssPassword;
		userChoice = dlg.DoModal();
		// process versioning system specific action
		if (userChoice==IDOK)
		{
			WaitStatusCursor waitstatus(_T(""));
			m_strVssProjectBase = dlg.m_strProject;
			theApp.WriteProfileString(_T("Settings"), _T("VssProject"), mf->m_strVssProjectBase);
			CString spath, sname;
			SplitFilename(strSavePath, &spath, &sname, 0);
			if (!spath.IsEmpty())
			{
				_chdrive(_totupper(spath[0])-'A'+1);
				_tchdir(spath);
			}
			CString args;
			args.Format(_T("checkout \"%s/%s\""), m_strVssProjectBase, sname);
			HANDLE hVss = RunIt(m_strVssPath, args, TRUE, FALSE);
			if (hVss!=INVALID_HANDLE_VALUE)
			{
				WaitForSingleObject(hVss, INFINITE);
				DWORD code;
				GetExitCodeProcess(hVss, &code);
				CloseHandle(hVss);
				if (code != 0)
				{
					AfxMessageBox(IDS_VSSERROR, MB_ICONSTOP);
					return FALSE;
				}
			}
			else
			{
				AfxMessageBox(IDS_VSS_RUN_ERROR, MB_ICONSTOP);
				return FALSE;
			}
		}
	}
	break;
	case 2: // CVisual SourceSafe 5.0+ (COM)
	{
		// prompt for user choice
		CVssPrompt dlg;
		CRegKeyEx reg;
		CString spath, sname;

		dlg.m_strMessage.FormatMessage(IDS_SAVE_FMT, strSavePath);
		dlg.m_strProject = m_strVssProjectBase;
		dlg.m_strUser = m_strVssUser;          // BSP - Add VSS user name to dialog box
		dlg.m_strPassword = m_strVssPassword;
		dlg.m_strSelectedDatabase = m_strVssDatabase;
		dlg.m_bVCProjSync = TRUE;

		if (!m_CheckOutMulti)
		{
			dlg.m_bMultiCheckouts = FALSE;
			//this is when we get the multicheck variable
			userChoice = dlg.DoModal();
			m_CheckOutMulti = dlg.m_bMultiCheckouts;
		}
		else//skip
		{
			userChoice = IDOK;
		}

		// process versioning system specific action
		if (userChoice == IDOK)
		{
			WaitStatusCursor waitstatus(_T(""));
			BOOL bOpened = FALSE;
			m_strVssProjectBase = dlg.m_strProject;
			m_strVssUser = dlg.m_strUser;
			m_strVssPassword = dlg.m_strPassword;
			m_strVssDatabase = dlg.m_strSelectedDatabase;
			m_bVCProjSync = dlg.m_bVCProjSync;					

			theApp.WriteProfileString(_T("Settings"), _T("VssDatabase"), m_strVssDatabase);
			theApp.WriteProfileString(_T("Settings"), _T("VssProject"), m_strVssProjectBase);
			theApp.WriteProfileString(_T("Settings"), _T("VssUser"), m_strVssUser);
			theApp.WriteProfileString(_T("Settings"), _T("VssPassword"), m_strVssPassword);

			IVSSDatabase	vssdb;
			IVSSItems		m_vssis;
			IVSSItem		m_vssi;

			COleException *eOleException = new COleException;
				
			// BSP - Create the COM interface pointer to VSS
			if (FAILED(vssdb.CreateDispatch(_T("SourceSafe"), eOleException)))
			{
				throw eOleException;	// catch block deletes.
			}
			else
			{
				eOleException->Delete();
			}

			//check if m_strVSSDatabase is specified:
			if (m_strVssDatabase.GetLength() > 0)
			{
				CString iniPath = m_strVssDatabase + _T("\\srcsafe.ini");
				// BSP - Open the specific VSS data file  using info from VSS dialog box
				vssdb.Open(iniPath, m_strVssUser, m_strVssPassword);														
				bOpened = TRUE;
			}
			
			if (bOpened == FALSE)
			{
				// BSP - Open the specific VSS data file  using info from VSS dialog box
				//let vss try to find one if not specified
				vssdb.Open(NULL, m_strVssUser, m_strVssPassword);
			}

			SplitFilename(strSavePath, &spath, &sname, 0);

			//check if m_strVssProjectBase has leading $\\, if not put them in:
			if ((m_strVssProjectBase[0] != '$' && m_strVssProjectBase[1] != '\\') ||
				(m_strVssProjectBase[0] != '$' && m_strVssProjectBase[1] != '/'))
			{
				CString temp = _T("$\\") + m_strVssProjectBase;
				m_strVssProjectBase = temp;
			}

			// BSP - Combine the project entered on the dialog box with the file name...
			const UINT nBufferSize = 1024;
			static TCHAR buffer[nBufferSize];
			static TCHAR buffer1[nBufferSize];
			static TCHAR buffer2[nBufferSize];

			_tcscpy(buffer1, strSavePath);
			_tcscpy(buffer2, m_strVssProjectBase);
			_tcslwr(buffer1);
			_tcslwr(buffer2);

			//make sure they both have \\ instead of /
			for (int k = 0; k < nBufferSize; k++)
			{
				if (buffer1[k] == '/')
					buffer1[k] = '\\';
				if (buffer2[k] == '/')
					buffer2[k] = '\\';
			}
			
			//take out last '\\'
			int strlb2 = _tcslen(buffer2);
			if (buffer2[strlb2-1] == '\\')
				buffer2[strlb2-1] = '\0';
			m_strVssProjectBase = buffer2;
			TCHAR * pbuf2 = &buffer2[2];//skip the $/
			TCHAR * pdest =  _tcsstr(buffer1, pbuf2);
			if (pdest)
			{
				int index  = (int)(pdest - buffer1 + 1);
			
				_tcscpy(buffer, buffer1);
				TCHAR * fp = &buffer[int(index + _tcslen(pbuf2))];
				sname = fp;

				if (sname[0] == ':')
				{
					_tcscpy(buffer2, sname);
					_tcscpy(buffer, (TCHAR*)&buffer2[2]);
					sname = buffer;
				}
			}
			CString strItem = m_strVssProjectBase + '\\' + sname;

			//  BSP - ...to get the specific source safe item to be checked out
			m_vssi = vssdb.GetVSSItem( strItem, 0 );

			// BSP - Get the working directory where VSS will put the file...
			CString strLocalSpec = m_vssi.GetLocalSpec();

			// BSP - ...and compare it to the directory WinMerge is using.
			if (strLocalSpec.CompareNoCase(strSavePath))
			{
				// BSP - if the directories are different, let the user confirm the CheckOut
				int iRes = AfxMessageBox(IDS_VSSFOLDER_AND_FILE_NOMATCH, MB_YESNO);

				if (iRes != IDYES)
				{
					m_CheckOutMulti = FALSE;//reset, we don't want 100 of the same errors
					return FALSE;   // BSP - if not Yes, bail.
				}
			}

			// BSP - Finally! Check out the file!
			m_vssi.Checkout(_T(""), strSavePath, 0);
		}
	}
	break;
	case 3:	// ClearCase
	{
		// prompt for user choice
		CCCPrompt dlg;
		userChoice = dlg.DoModal();
		// process versioning system specific action
		if (userChoice == IDOK)
		{
			WaitStatusCursor waitstatus(_T(""));
			CString spath, sname;
			SplitFilename(strSavePath, &spath, &sname, 0);
			if (!spath.IsEmpty())
			{
				_chdrive(_totupper(spath[0])-'A'+1);
				_tchdir(spath);
			}
			DWORD code;
			CString args;
			args.Format(_T("checkout -c \"%s\" %s"), dlg.m_comments, sname);
			HANDLE hVss = RunIt(m_strVssPath, args, TRUE, FALSE);
			if (hVss!=INVALID_HANDLE_VALUE)
			{
				WaitForSingleObject(hVss, INFINITE);
				GetExitCodeProcess(hVss, &code);
				CloseHandle(hVss);
				
				if (code != 0)
				{
					AfxMessageBox(IDS_VSSERROR, MB_ICONSTOP);
					return FALSE;
				}
			}
			else
			{
				AfxMessageBox(IDS_VSS_RUN_ERROR, MB_ICONSTOP);
				return FALSE;
			}
		}
	}
	break;
	}	//switch(m_nVerSys)

	return TRUE;
}

/// Wrapper to set the global option 'm_bAllowMixedEol'
void CMainFrame::SetEOLMixed(BOOL bAllow)
{
	m_options.SaveOption(OPT_ALLOW_MIXED_EOL, bAllow);
	ApplyViewWhitespace();
}

void CMainFrame::OnOptions() 
{
	DIFFOPTIONS diffOptions = {0};
	CString selectedFilter;
	StringPairArray fileFilters;
	theApp.GetFileFilters(&fileFilters, selectedFilter);

	// use CDiffwrapper static functions to exchange the options with registry
	CDiffWrapper::ReadDiffOptions(&diffOptions);

	CPropertySheet sht(IDS_OPTIONS_TITLE);
	CPropVss vss;
	CPropGeneral gen;
	CPropFilter filter(&fileFilters, selectedFilter);
	CPropColors colors(theApp.GetDiffColor(), theApp.GetSelDiffColor(), theApp.GetDiffDeletedColor(), theApp.GetSelDiffDeletedColor(), 
	                   theApp.GetDiffTextColor(), theApp.GetSelDiffTextColor(), theApp.GetTrivialColor(), theApp.GetTrivialDeletedColor());
	CPropRegistry regpage;
    CPropCompare compage;
	CPropEditor editor;
   
	sht.AddPage(&gen);
	sht.AddPage(&compage);
	sht.AddPage(&editor);
	sht.AddPage(&filter);
	sht.AddPage(&vss);
	sht.AddPage(&colors);
	sht.AddPage(&regpage);
	
	vss.m_nVerSys = m_nVerSys;
	vss.m_strPath = m_strVssPath;
	gen.m_bBackup = m_options.GetInt(OPT_CREATE_BACKUPS);
	gen.m_bScroll = m_options.GetInt(OPT_SCROLL_TO_FIRST);
	gen.m_bDisableSplash = theApp.m_bDisableSplash;
	filter.m_bIgnoreRegExp = m_bIgnoreRegExp;
	filter.m_sPattern = m_sPattern;
	regpage.m_strEditorPath = m_sExtEditorPath;
	regpage.GetContextRegValues();
	regpage.m_bUseRecycleBin = m_options.GetInt(OPT_USE_RECYCLE_BIN);
    compage.m_compareMethod = m_nCompMethod;
	compage.m_nIgnoreWhite = diffOptions.nIgnoreWhitespace;
	compage.m_bIgnoreCase = diffOptions.bIgnoreCase;
	compage.m_bIgnoreBlankLines = diffOptions.bIgnoreBlankLines;
	compage.m_bEolSensitive = diffOptions.bEolSensitive;
	editor.m_nTabSize = m_nTabSize;
	editor.m_nTabType = m_nTabType;
	editor.m_bAutomaticRescan = m_options.GetInt(OPT_AUTOMATIC_RESCAN);
	editor.m_bHiliteSyntax = theApp.m_bHiliteSyntax;
	editor.m_bAllowMixedEol = m_options.GetInt(OPT_ALLOW_MIXED_EOL);
	
	if (sht.DoModal()==IDOK)
	{
		m_nVerSys = vss.m_nVerSys;
		m_strVssPath = vss.m_strPath;
		
		m_options.SaveOption(OPT_CREATE_BACKUPS, gen.m_bBackup);
		m_options.SaveOption(OPT_SCROLL_TO_FIRST, gen.m_bScroll);

		theApp.m_bDisableSplash = gen.m_bDisableSplash;
		m_options.SaveOption(OPT_USE_RECYCLE_BIN, regpage.m_bUseRecycleBin);

		diffOptions.nIgnoreWhitespace = compage.m_nIgnoreWhite;
		diffOptions.bIgnoreBlankLines = compage.m_bIgnoreBlankLines;
		diffOptions.bEolSensitive = compage.m_bEolSensitive;
		diffOptions.bIgnoreCase = compage.m_bIgnoreCase;
        m_nCompMethod = compage.m_compareMethod;

		m_nTabSize = editor.m_nTabSize;
		m_nTabType = editor.m_nTabType;
		m_options.SaveOption(OPT_AUTOMATIC_RESCAN, editor.m_bAutomaticRescan);
		m_options.SaveOption(OPT_ALLOW_MIXED_EOL, editor.m_bAllowMixedEol);
		theApp.m_bHiliteSyntax = editor.m_bHiliteSyntax;

		m_bIgnoreRegExp = filter.m_bIgnoreRegExp;
		m_sPattern = filter.m_sPattern;
		theApp.SetFileFilterPath(filter.m_sFileFilterPath);

		regpage.SaveMergePath();
		m_sExtEditorPath = regpage.m_strEditorPath;
		m_sExtEditorPath.TrimLeft();
		m_sExtEditorPath.TrimRight();
		if (m_sExtEditorPath.IsEmpty())
			m_sExtEditorPath = GetDefaultEditor();

		theApp.SetDiffColor(colors.m_clrDiff);
		theApp.SetSelDiffColor(colors.m_clrSelDiff);
		theApp.SetDiffDeletedColor(colors.m_clrDiffDeleted);
		theApp.SetSelDiffDeletedColor(colors.m_clrSelDiffDeleted);
		theApp.SetDiffTextColor(colors.m_clrDiffText);
		theApp.SetSelDiffTextColor(colors.m_clrSelDiffText);
		theApp.SetTrivialColor(colors.m_clrTrivial);
		theApp.SetTrivialDeletedColor(colors.m_clrTrivialDeleted);

		theApp.WriteProfileInt(_T("Settings"), _T("VersionSystem"), m_nVerSys);
		theApp.WriteProfileString(_T("Settings"), _T("VssPath"), m_strVssPath);
		theApp.WriteProfileInt(_T("Settings"), _T("TabSize"), m_nTabSize);
		theApp.WriteProfileInt(_T("Settings"), _T("TabType"), m_nTabType);
		theApp.WriteProfileInt(_T("Settings"), _T("IgnoreRegExp"), m_bIgnoreRegExp);
		theApp.WriteProfileString(_T("Settings"), _T("RegExps"), m_sPattern);
		theApp.WriteProfileString(_T("Settings"), _T("FileFilterPath"), filter.m_sFileFilterPath);
		theApp.WriteProfileInt(_T("Settings"), _T("DisableSplash"), theApp.m_bDisableSplash);
        theApp.WriteProfileInt(_T("Settings"), _T("CompMethod"), m_nCompMethod);

		theApp.WriteProfileInt(_T("Settings"), _T("HiliteSyntax"), theApp.m_bHiliteSyntax);
		theApp.WriteProfileString(_T("Settings"), _T("ExternalEditor"), m_sExtEditorPath);

		// use CDiffwrapper static functions to exchange the options with registry
		CDiffWrapper::WriteDiffOptions(&diffOptions);

		RebuildRegExpList();

		// Call the wrapper to set m_bAllowMixedEol (the wrapper updates the registry)
		SetEOLMixed(editor.m_bAllowMixedEol);

		// make an attempt at rescanning any open diff sessions
		MergeDocList docs;
		GetAllMergeDocs(&docs);
		while (!docs.IsEmpty())
		{
			CMergeDoc * pMergeDoc = docs.RemoveHead();
			CMergeEditView * pLeft = pMergeDoc->GetLeftView();
			CMergeEditView * pRight = pMergeDoc->GetRightView();

			// Re-read MergeDoc settings
			pMergeDoc->ReadSettings();
			
			// Enable/disable automatic rescan (rescan after editing)
			pLeft->EnableRescan(m_options.GetInt(OPT_AUTOMATIC_RESCAN));
			pRight->EnableRescan(m_options.GetInt(OPT_AUTOMATIC_RESCAN));

			// Set tab type (tabs/spaces)
			if (m_nTabType == 0)
			{
				pLeft->SetInsertTabs(TRUE);
				pRight->SetInsertTabs(TRUE);
			}
			else
			{
				pLeft->SetInsertTabs(FALSE);
				pRight->SetInsertTabs(FALSE);
			}

			// Allow user to save files or not, cancel is pointless
			pMergeDoc->SaveHelper(FALSE);
			pMergeDoc->FlushAndRescan(TRUE);
		}

		// Update all dirdoc settings
		DirDocList dirDocs;
		GetAllDirDocs(&dirDocs);

		while (!dirDocs.IsEmpty())
		{
			CDirDoc *pDirDoc = dirDocs.RemoveHead();
			pDirDoc->ReadSettings();
		}
	}
}

/**
 * @brief Begin a diff: open dirdoc if it is directories, else open a mergedoc for editing
 */
BOOL CMainFrame::DoFileOpen(LPCTSTR pszLeft /*=NULL*/, LPCTSTR pszRight /*=NULL*/,
	DWORD dwLeftFlags /*=0*/, DWORD dwRightFlags /*=0*/, BOOL bRecurse /*=FALSE*/)
{
	CString strLeft(pszLeft);
	CString strRight(pszRight);
	CString strExt;
	PackingInfo infoUnpacker;
	int cpleft=0, cpright=0; // 8-bit codepages

	BOOL bRORight = dwLeftFlags & FFILEOPEN_READONLY;
	BOOL bROLeft = dwRightFlags & FFILEOPEN_READONLY;
	BOOL docNull;
	CDirDoc * pDirDoc = GetDirDocToShow(&docNull);

	// If the dirdoc we are supposed to use is busy doing a diff, bail out
	UINT threadState = pDirDoc->m_diffThread.GetThreadState();
	if (threadState == THREAD_COMPARING)
		return FALSE;

	if (!docNull)
	{
		// If reusing an existing doc, give it a chance to save its data
		// and close any merge views, and clear its window
		if (!pDirDoc->ReusingDirDoc())
			return FALSE;
	}

	// pop up dialog unless arguments exist (and are compatible)
	PATH_EXISTENCE pathsType = GetPairComparability(strLeft, strRight);
	if (pathsType == DOES_NOT_EXIST)
	{
		COpenDlg dlg;
		dlg.m_strLeft = strLeft;
		dlg.m_strRight = strRight;
		dlg.m_bRecurse = bRecurse;
		if (dlg.DoModal() != IDOK)
			return FALSE;

		strLeft = dlg.m_strLeft;
		strRight = dlg.m_strRight;
		bRecurse = dlg.m_bRecurse;
		strExt = dlg.m_strParsedExt;
		infoUnpacker = dlg.m_infoHandler;
		pathsType = static_cast<PATH_EXISTENCE>(dlg.m_pathsType);
		// TODO: add codepage options to open dialog ?
		cpleft = 0;
		cpright = 0;
	}
	else
	{
		//save the MRU left and right files.
		if (!(dwLeftFlags & FFILEOPEN_NOMRU))
			addToMru(pszLeft, _T("Files\\Left"));
		if (!(dwRightFlags & FFILEOPEN_NOMRU))
			addToMru(pszRight, _T("Files\\Right"));
	}

	if (1)
	{
		gLog.Write(_T("### Begin Comparison Parameters #############################\r\n")
				  _T("\tLeft: %s\r\n")
				   _T("\tRight: %s\r\n")
				  _T("\tRecurse: %d\r\n")
				  _T("\tShowUniqueLeft: %d\r\n")
				  _T("\tShowUniqueRight: %d\r\n")
				  _T("\tShowIdentical: %d\r\n")
				  _T("\tShowDiff: %d\r\n")
				  _T("\tShowBinaries: %d\r\n")
				  _T("\tHideBak: %d\r\n")
				  _T("\tVerSys: %d\r\n")
				  _T("\tVssPath: %s\r\n")
				  _T("\tBackups: %d\r\n")
				  _T("\tAllowMixedEOL: %d\r\n")
				  _T("\tScrollToFirst: %d\r\n")
				  _T("### End Comparison Parameters #############################\r\n"),
				  strLeft,
				  strRight,
				  bRecurse,
				  m_options.GetInt(OPT_SHOW_UNIQUE_LEFT),
				  m_options.GetInt(OPT_SHOW_UNIQUE_RIGHT),
				  m_options.GetInt(OPT_SHOW_IDENTICAL),
				  m_options.GetInt(OPT_SHOW_DIFFERENT),
				  m_options.GetInt(OPT_SHOW_BINARIES),
				  m_options.GetInt(OPT_HIDE_BACKUP),
				  m_nVerSys,
				  m_strVssPath,
				  m_options.GetInt(OPT_CREATE_BACKUPS),
				  m_options.GetInt(OPT_ALLOW_MIXED_EOL),
				  m_options.GetInt(OPT_SCROLL_TO_FIRST));
	}

	try
	{
		// Handle archives using 7-zip
		if (Merge7z::Format *piHandler = Merge7z->GuessFormat(strLeft))
		{
			pathsType = IS_EXISTING_DIR;
			if (strRight == strLeft)
			{
				strRight.Empty();
			}
			CTempPath path = pDirDoc;
			do
			{
				if FAILED(piHandler->DeCompressArchive(m_hWnd, strLeft, path))
					break;
				if (strLeft.Find(path) == 0)
				{
					if (!::DeleteFile(strLeft))
					{
						LogErrorString(Fmt(_T("DeleteFile(%s) failed: %s"),
							strLeft, GetSysError(GetLastError())));
					}
				}
				strLeft.Delete(0, strLeft.ReverseFind('\\'));
				int dot = strLeft.ReverseFind('.');
				if (piHandler != &Merge7z->TarHandler && StrChr(_T("Tt"), strLeft[dot + 1]))
				{
					strLeft.GetBufferSetLength(dot + 2);
					strLeft += _T("ar");
				}
				else
				{
					strLeft.GetBufferSetLength(dot);
				}
				strLeft.Insert(0, path);
			} while (piHandler = Merge7z->GuessFormat(strLeft));
			strLeft = path;
			if (Merge7z::Format *piHandler = Merge7z->GuessFormat(strRight))
			{
				path.MakeSibling(_T(".1"));
				do
				{
					if FAILED(piHandler->DeCompressArchive(m_hWnd, strRight, path))
						break;;
					if (strRight.Find(path) == 0)
					{
						if (!::DeleteFile(strRight))
						{
							LogErrorString(Fmt(_T("DeleteFile(%s) failed: %s"),
								strRight, GetSysError(GetLastError())));
						}
					}
					strRight.Delete(0, strRight.ReverseFind('\\'));
					int dot = strRight.ReverseFind('.');
					if (piHandler != &Merge7z->TarHandler && StrChr(_T("Tt"), strRight[dot + 1]))
					{
						strRight.GetBufferSetLength(dot + 2);
						strRight += _T("ar");
					}
					else
					{
						strRight.GetBufferSetLength(dot);
					}
					strRight.Insert(0, path);
				} while (piHandler = Merge7z->GuessFormat(strRight));
				strRight = path;
			}
			else if (strRight.IsEmpty())
			{
				// assume Perry style patch
				strRight = path;
				strLeft += _T("\\ORIGINAL");
				strRight += _T("\\ALTERED");
				if (!PathFileExists(strLeft) || !PathFileExists(strRight))
				{
					// not a Perry style patch: diff with itself...
					strLeft = strRight = path;
				}
			}
		}
	}
	catch (CException *e)
	{
		e->ReportError(MB_ICONSTOP);
		e->Delete();
	}

	// open the diff
	if (pathsType == IS_EXISTING_DIR)
	{
		if (pDirDoc)
		{
			CDiffContext *pCtxt = new CDiffContext(strLeft, strRight);
			if (pCtxt != NULL)
			{
				gLog.Write(LOGLEVEL::LNOTICE, _T("Open dirs: Left: %s\n\tRight: %s."),
					strLeft, strRight);

				pDirDoc->SetReadOnly(TRUE, bROLeft);
				pDirDoc->SetReadOnly(FALSE, bRORight);
				pDirDoc->SetRecursive(bRecurse);
				pCtxt->SetRegExp(strExt);
				pDirDoc->SetDiffContext(pCtxt);
				pDirDoc->SetDescriptions(m_strLeftDesc, m_strRightDesc);
				m_strLeftDesc.Empty();
				m_strRightDesc.Empty();

				pDirDoc->Rescan();
				if (m_options.GetInt(OPT_SCROLL_TO_FIRST))
				{
					CDirView * pDirView = pDirDoc->GetMainView();
					pDirView->GotoFirstDiff();
				}
			}
		}
	}
	else
	{
		// If file is read-only on disk, set RO status
		// But if file is not, don't reset RO status given as param
		if (files_isFileReadOnly(strLeft))
			bROLeft = TRUE;
		if (files_isFileReadOnly(strRight))
			bRORight = TRUE;
	
		gLog.Write(LOGLEVEL::LNOTICE, _T("Open files: Left: %s\n\tRight: %s."),
			strLeft, strRight);
		
		ShowMergeDoc(pDirDoc, strLeft, strRight, bROLeft, bRORight,
			cpleft, cpright, &infoUnpacker);
	}
	return TRUE;
}

/// Creates backup before file is saved over
BOOL CMainFrame::CreateBackup(LPCTSTR pszPath)
{
	// first, make a backup copy of the original
	CFileStatus status;

	// create backup copy of file if destination file exists
	if (m_options.GetInt(OPT_CREATE_BACKUPS) && CFile::GetStatus(pszPath, status))
	{
		// build the backup filename
		CString spath, sname, sext;
		SplitFilename(pszPath, &spath, &sname, &sext);
		CString s;
		if (!sext.IsEmpty())
			s.Format(_T("%s\\%s.%s") BACKUP_FILE_EXT, spath, sname, sext);
		else
			s.Format(_T("%s\\%s")  BACKUP_FILE_EXT, spath, sname);

		// Copy the file
		if (!CopyFile(pszPath, s, FALSE))
		{
			if (AfxMessageBox(IDS_BACKUP_FAILED_PROMPT,
					MB_YESNO | MB_ICONQUESTION) != IDYES)
				return FALSE;
		}
		return TRUE;
	}

	// we got here because we're either not backing up of there was nothing to backup
	return TRUE;
}

/**
 * @brief Trim trailing line returns.
 */
static void RemoveLineReturns(CString & str)
{
	str.Remove('\n');
	str.Remove('\r');
}

/**
 * @brief Sync file to Version Control System
 * @param [in] pszSrc File to copy
 * @param [in] pszDest Where to copy (incl. filename)
 * @param [out] psError Error string that can be shown to user in caller func.
 * Does not contain filename.
 */
BOOL CMainFrame::SyncFilesToVCS(LPCTSTR pszSrc, LPCTSTR pszDest, CString * psError)
{
	CString sActionError;
	CString strSavePath(pszDest);

	if (!CheckSavePath(strSavePath))
	{
		psError->LoadString(IDS_ERROR_FILE_WRITEABLE);
		return FALSE;
	}
	
	if (!CreateBackup(strSavePath))
	{
		psError->LoadString(IDS_ERROR_BACKUP);
		return FALSE;
	}
	
	// If VC project opened from VSS sync first
	if (m_bVCProjSync)
		return ReLinkVCProj(strSavePath, psError);
	else
		return TRUE;
}

BOOL CMainFrame::ReLinkVCProj(CString strSavePath,CString * psError)
{
	const UINT nBufferSize = 1024;
	static TCHAR buffer[nBufferSize];
	static TCHAR buffer1[nBufferSize];
	static TCHAR buffer2[nBufferSize];
	TCHAR tempPath[MAX_PATH] = {0};
	TCHAR tempFile[MAX_PATH] = {0};
	CString spath;
	BOOL bVCPROJ = FALSE;

	if (::GetTempPath(MAX_PATH, tempPath))
	{
		if (!::GetTempFileName(tempPath, _T ("_LT"), 0, tempFile))
			return FALSE;
	}
	else
	{
		return FALSE;
	}

	_tsplitpath((LPCTSTR)strSavePath, NULL, NULL, NULL, buffer);
	if (!_tcscmp(buffer, _T(".vcproj")) || !_tcscmp(buffer, _T(".sln")))
	{
		if (!_tcscmp(buffer,_T(".vcproj")))
			bVCPROJ = TRUE;
		SplitFilename(strSavePath, &spath, NULL, NULL);
		
		//check if m_strVssProjectBase has leading $\\, if not put them in:
		if ((m_strVssProjectBase[0] != '$' && m_strVssProjectBase[1] != '\\') ||
			(m_strVssProjectBase[0] != '$' && m_strVssProjectBase[1] != '/'))
		{
			CString temp = _T("$/") + m_strVssProjectBase;
			m_strVssProjectBase = temp;
		}
		_tcscpy(buffer1, strSavePath);
		_tcscpy(buffer2, m_strVssProjectBase);
		_tcslwr(buffer1);
		_tcslwr(buffer2);

		//make sure they both have \\ instead of /
		for (int k = 0; k < nBufferSize; k++)
		{
			if (buffer1[k] == '\\')
				buffer1[k] = '/';
			if (buffer2[k] == '\\')
				buffer2[k] = '/';
		}
		
		//take out last '\\'
		int strlb2 = _tcslen(buffer2);
		if (buffer2[strlb2-1] == '/')
			buffer2[strlb2-1] = '\0';
		m_strVssProjectBase  =buffer2;

		TCHAR * pbuf2 = &buffer2[2];//skip the $/
		TCHAR * pdest = _tcsstr(buffer1, pbuf2);

		if (pdest)
		{
			int index  = (int)(pdest - buffer1 + 1);
		
			_tcscpy(buffer,buffer1);
			TCHAR * fp = &buffer[int(index + _tcslen(pbuf2))];
			m_strVssProjectFull = fp;

			if (m_strVssProjectFull[0] == ':')
			{
				_tcscpy(buffer2,m_strVssProjectFull);
				_tcscpy(buffer, (TCHAR*)&buffer2[2]);
				m_strVssProjectFull = buffer;
			}
		}

		SplitFilename(m_strVssProjectFull, &spath, NULL, NULL);
		if (!(m_strVssProjectBase[m_strVssProjectBase.GetLength()-1] == '\\' ||
			m_strVssProjectBase[m_strVssProjectBase.GetLength()-1] == '/'))
		{
			m_strVssProjectBase += "/";
		}

		m_strVssProjectFull = m_strVssProjectBase + spath;
		
		//if sln file, we need to replace ' '  with _T("\\u0020")
		if (!bVCPROJ)
		{
			_tcscpy(buffer, m_strVssProjectFull);
			ZeroMemory(&buffer1, nBufferSize * sizeof(TCHAR));
			ZeroMemory(&buffer2, nBufferSize * sizeof(TCHAR));
			for (TCHAR * pc = buffer; *pc; pc++)
			{
				if (*pc == ' ')//insert \\u0020
				{
					_stprintf(buffer1, _T("%s\\u0020"), buffer2);
					_tcscpy(buffer2, buffer1);
				}
				else
				{
					int slb2 = _tcslen(buffer2);
					buffer2[slb2] = *pc;
					buffer2[slb2+1] = '\0';
				}
			}
			m_strVssProjectFull = buffer2;
		}

		HANDLE hfile;
		HANDLE tfile;
		SetFileAttributes(strSavePath, FILE_ATTRIBUTE_NORMAL);
		
		hfile = CreateFile(strSavePath,
                GENERIC_ALL,              // open for writing
                FILE_SHARE_READ,           // share for reading 
                NULL,                      // no security 
                OPEN_EXISTING,             // existing file only 
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,     // normal file 
                NULL);
		tfile = CreateFile(tempFile,
                GENERIC_ALL,              // open for writing
                FILE_SHARE_READ,           // share for reading 
                NULL,                      // no security 
                CREATE_ALWAYS,             // existing file only 
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,     // normal file 
                NULL);  
		static TCHAR charset[] = _T(" \t\n\r=");
		DWORD numwritten = 0;
	
		ZeroMemory(&buffer2, nBufferSize * sizeof(TCHAR));
		while (GetWordFile(hfile, buffer, charset))
		{
			WriteFile(tfile, buffer, _tcslen(buffer), &numwritten, NULL);
			if (bVCPROJ)
			{
				if (!_tcscmp(buffer, _T("SccProjectName")))
				{
					//nab the equals sign
					GetWordFile(hfile, buffer, _T("="));
					WriteFile(tfile, buffer, _tcslen(buffer), &numwritten, NULL);
					CString stemp = _T("\"&quot;") + m_strVssProjectFull + 
						_T("&quot;");
					WriteFile(tfile, stemp, stemp.GetLength(),
						&numwritten, NULL);
					GetWordFile(hfile, buffer, _T(",\n"));//for junking
					GetWordFile(hfile, buffer, _T(",\n"));//get the next delimiter
					if (!_tcscmp(buffer, _T("\n")))
					{
						WriteFile(tfile, _T("\""), 1, &numwritten, NULL);						
					}
					WriteFile(tfile, buffer, _tcslen(buffer), &numwritten, NULL);
				}
			}
			else
			{//sln file
				//find sccprojectname inside this string
				if (_tcsstr(buffer, _T("SccProjectUniqueName")) == buffer)
				{
					//nab until next no space, and no =
					GetWordFile(hfile, buffer, _T(" ="));
					WriteFile(tfile, buffer, _tcslen(buffer), &numwritten, NULL);
					//nab word
					GetWordFile(hfile, buffer, _T("\\\n."));
					while (!_tcsstr(buffer, _T(".")))
					{						
						if (buffer[0] != '\\')
						{
							_stprintf(buffer1, _T("%s/%s"), buffer2, buffer);//put append word to file
							_tcscpy(buffer2,buffer1);
						}
						WriteFile(tfile, buffer, _tcslen(buffer), &numwritten, NULL);
						GetWordFile(hfile, buffer, _T("\\\n."));
					}
					WriteFile(tfile, buffer, _tcslen(buffer), &numwritten, NULL);
				}
				else if (_tcsstr(buffer, _T("SccProjectName")) == buffer)
				{
					
					//buffer2 appends
					CString capp;
					if (buffer2[0] != '\\' && !_tcsstr(buffer2, _T(".")))
					{
						//write out \\u0020s for every space in buffer2
						ZeroMemory(&buffer1, nBufferSize * sizeof(TCHAR));
						ZeroMemory(&buffer, nBufferSize * sizeof(TCHAR));
						for (TCHAR * pc = buffer2; *pc; pc++)
						{
							if (*pc == ' ')//insert \\u0020
							{
								_stprintf(buffer, _T("%s\\u0020"), buffer1);
								_tcscpy(buffer1, buffer);
							}
							else
							{
								int slb2 = _tcslen(buffer1);
								buffer1[slb2] = *pc;
								buffer1[slb2+1] = '\0';
							}
						}
						_tcslwr(buffer1);
						capp = buffer1;
						
						//nab until the no space, and no =
						GetWordFile(hfile, buffer, _T(" ="));
						WriteFile(tfile, buffer, _tcslen(buffer), &numwritten, NULL);
						CString stemp =  _T("\\u0022") + m_strVssProjectFull + capp + _T("\\u0022");
						WriteFile(tfile, stemp, stemp.GetLength(),
							&numwritten, NULL);
						
						//nab until the first backslash
						GetWordFile(hfile, buffer, _T(","));
						ZeroMemory(&buffer2, nBufferSize * sizeof(TCHAR));						
					}
				}
			}
		}
		CloseHandle(hfile);
		CloseHandle(tfile);
		if (!CopyFile(tempFile, strSavePath, FALSE))
		{
			*psError = GetSysError(GetLastError());
			return FALSE;
		}
		else
		{
			DeleteFile(tempFile);
		}
	}
	return TRUE;
}

void CMainFrame::OnViewSelectfont() 
{
	CHOOSEFONT cf;
	memset(&cf, 0, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.Flags = CF_INITTOLOGFONTSTRUCT|CF_FORCEFONTEXIST|CF_SCREENFONTS|CF_FIXEDPITCHONLY;
	// CF_FIXEDPITCHONLY = 0x00004000L
	// in case you are a developer and want to disable it to test with, eg, a Chinese capable font
	cf.lpLogFont = &m_lfDiff;
	if (ChooseFont(&cf))
	{
		m_bFontSpecified = TRUE;
		theApp.WriteProfileInt(_T("Font"), _T("Specified"), m_bFontSpecified);
		theApp.WriteProfileInt(_T("Font"), _T("Height"), m_lfDiff.lfHeight);
		theApp.WriteProfileInt(_T("Font"), _T("Width"), m_lfDiff.lfWidth);
		theApp.WriteProfileInt(_T("Font"), _T("Escapement"), m_lfDiff.lfEscapement);
		theApp.WriteProfileInt(_T("Font"), _T("Orientation"), m_lfDiff.lfOrientation);
		theApp.WriteProfileInt(_T("Font"), _T("Weight"), m_lfDiff.lfWeight);
		theApp.WriteProfileInt(_T("Font"), _T("Italic"), m_lfDiff.lfItalic);
		theApp.WriteProfileInt(_T("Font"), _T("Underline"), m_lfDiff.lfUnderline);
		theApp.WriteProfileInt(_T("Font"), _T("StrikeOut"), m_lfDiff.lfStrikeOut);
		theApp.WriteProfileInt(_T("Font"), _T("CharSet"), m_lfDiff.lfCharSet);
		theApp.WriteProfileInt(_T("Font"), _T("OutPrecision"), m_lfDiff.lfOutPrecision);
		theApp.WriteProfileInt(_T("Font"), _T("ClipPrecision"), m_lfDiff.lfClipPrecision);
		theApp.WriteProfileInt(_T("Font"), _T("Quality"), m_lfDiff.lfQuality);
		theApp.WriteProfileInt(_T("Font"), _T("PitchAndFamily"), m_lfDiff.lfPitchAndFamily);
		theApp.WriteProfileString(_T("Font"), _T("FaceName"), m_lfDiff.lfFaceName);

		AfxMessageBox(IDS_FONT_CHANGE, MB_ICONINFORMATION);

		MergeEditViewList editViews;
		GetAllViews(&editViews, NULL, NULL);
		
		// TODO: Update document fonts
		/*
		for (POSITION pos = editViews.GetHeadPosition(); pos; editViews.GetNext(pos))
		{
			CMergeEditView * pEditView = editViews.GetAt(pos);
			// update pEditView for font change
		}
		*/
	}
}

void CMainFrame::GetFontProperties()
{
	m_lfDiff.lfHeight = theApp.GetProfileInt(_T("Font"), _T("Height"), -16);
	m_lfDiff.lfWidth = theApp.GetProfileInt(_T("Font"), _T("Width"), 0);
	m_lfDiff.lfEscapement = theApp.GetProfileInt(_T("Font"), _T("Escapement"), 0);
	m_lfDiff.lfOrientation = theApp.GetProfileInt(_T("Font"), _T("Orientation"), 0);
	m_lfDiff.lfWeight = theApp.GetProfileInt(_T("Font"), _T("Weight"), FW_NORMAL);
	m_lfDiff.lfItalic = (BYTE)theApp.GetProfileInt(_T("Font"), _T("Italic"), FALSE);
	m_lfDiff.lfUnderline = (BYTE)theApp.GetProfileInt(_T("Font"), _T("Underline"), FALSE);
	m_lfDiff.lfStrikeOut = (BYTE)theApp.GetProfileInt(_T("Font"), _T("StrikeOut"), FALSE);
	m_lfDiff.lfCharSet = (BYTE)theApp.GetProfileInt(_T("Font"), _T("CharSet"), ANSI_CHARSET);
	m_lfDiff.lfOutPrecision = (BYTE)theApp.GetProfileInt(_T("Font"), _T("OutPrecision"), OUT_STRING_PRECIS);
	m_lfDiff.lfClipPrecision = (BYTE)theApp.GetProfileInt(_T("Font"), _T("ClipPrecision"), CLIP_STROKE_PRECIS);
	m_lfDiff.lfQuality = (BYTE)theApp.GetProfileInt(_T("Font"), _T("Quality"), DRAFT_QUALITY);
	m_lfDiff.lfPitchAndFamily = (BYTE)theApp.GetProfileInt(_T("Font"), _T("PitchAndFamily"), FF_MODERN | FIXED_PITCH);
	_tcscpy(m_lfDiff.lfFaceName, theApp.GetProfileString(_T("Font"), _T("FaceName"), _T("Courier")));
}

void CMainFrame::OnViewUsedefaultfont() 
{
	m_bFontSpecified=FALSE;
	theApp.WriteProfileInt(_T("Font"), _T("Specified"), m_bFontSpecified);
}

void CMainFrame::OnUpdateViewUsedefaultfont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bFontSpecified);
}

/// Update any language-dependent data
void CMainFrame::UpdateResources()
{
	m_wndStatusBar.SetPaneText(0, LoadResString(AFX_IDS_IDLEMESSAGE));

	DirDocList dirdocs;
	GetAllDirDocs(&dirdocs);
	while (!dirdocs.IsEmpty())
	{
		CDirDoc * pDoc = dirdocs.RemoveHead();
		pDoc->UpdateResources();
	}

	MergeDocList mergedocs;
	GetAllMergeDocs(&mergedocs);
	while (!mergedocs.IsEmpty())
	{
		CMergeDoc * pDoc = mergedocs.RemoveHead();
		pDoc->GetLeftView()->UpdateResources();
		pDoc->GetRightView()->UpdateResources();
	}
}

void CMainFrame::OnHelpContents() 
{
	CString spath = GetModulePath(0) + _T("\\Docs\\index.html");
	CString url = _T("http://winmerge.sourceforge.net/docs20/index.html");

	CFileStatus status;
	if (CFile::GetStatus(spath, status))
		ShellExecute(NULL, _T("open"), spath, NULL, NULL, SW_SHOWNORMAL);
	else
		ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);

}

void CMainFrame::OnUpdateHelpContents(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	if (!m_bFirstTime)
	{
		CMDIFrameWnd::ActivateFrame(nCmdShow);
		return;
	}

	m_bFirstTime = FALSE;

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);
	wp.rcNormalPosition.left=theApp.GetProfileInt(_T("Settings"), _T("MainLeft"),0);
	wp.rcNormalPosition.top=theApp.GetProfileInt(_T("Settings"), _T("MainTop"),0);
	wp.rcNormalPosition.right=theApp.GetProfileInt(_T("Settings"), _T("MainRight"),0);
	wp.rcNormalPosition.bottom=theApp.GetProfileInt(_T("Settings"), _T("MainBottom"),0);
	wp.showCmd = nCmdShow;

	CRect dsk_rc,rc(wp.rcNormalPosition);

	dsk_rc.left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	dsk_rc.top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	dsk_rc.right = dsk_rc.left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	dsk_rc.bottom = dsk_rc.top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
	if (theApp.GetProfileInt(_T("Settings"), _T("MainMax"), FALSE))
	{
		CMDIFrameWnd::ActivateFrame(SW_MAXIMIZE);	
	}
	else if (rc.Width() != 0 && rc.Height() != 0)
	{
		// Ensure top-left corner is on visible area,
		// 20 points margin is added to prevent "lost" window
		CPoint ptTopLeft(rc.TopLeft());
		ptTopLeft += CPoint(20, 20);

		if (dsk_rc.PtInRect(ptTopLeft))
			SetWindowPlacement(&wp);
		else
			CMDIFrameWnd::ActivateFrame(nCmdShow);
	}
	else
		CMDIFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnClose() 
{
	// save any dirty edit views
	MergeDocList mergedocs;
	GetAllMergeDocs(&mergedocs);
	for (POSITION pos = mergedocs.GetHeadPosition(); pos; mergedocs.GetNext(pos))
	{
		CMergeDoc * pMergeDoc = mergedocs.GetAt(pos);
		CMergeEditView * pLeft = pMergeDoc->GetLeftView();
		CMergeEditView * pRight = pMergeDoc->GetRightView();
		if ((pLeft && pLeft->IsModified())
			|| (pRight && pRight->IsModified()))
		{
			// Allow user to cancel closing
			if (!pMergeDoc->SaveHelper(TRUE))
				return;
			else
			{
				// Set modified status to false so that we are not asking
				// about saving again. 
				pMergeDoc->m_ltBuf.SetModified(FALSE);
				pMergeDoc->m_rtBuf.SetModified(FALSE);
			}
		}
	}

	// save main window position
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);
	theApp.WriteProfileInt(_T("Settings"), _T("MainLeft"),wp.rcNormalPosition.left);
	theApp.WriteProfileInt(_T("Settings"), _T("MainTop"),wp.rcNormalPosition.top);
	theApp.WriteProfileInt(_T("Settings"), _T("MainRight"),wp.rcNormalPosition.right);
	theApp.WriteProfileInt(_T("Settings"), _T("MainBottom"),wp.rcNormalPosition.bottom);
	theApp.WriteProfileInt(_T("Settings"), _T("MainMax"), (wp.showCmd == SW_MAXIMIZE));

	// tell all merge docs to save position
	// don't call SavePosition, it is called when the child frame is destroyed
	/*
	while (!mergedocs.IsEmpty())
	{
		CMergeDoc * pMergeDoc = mergedocs.RemoveHead();
		CMergeEditView * pLeft = pMergeDoc->GetLeftView();
		if (pLeft)
			pMergeDoc->GetParentFrame()->SavePosition();
	}
	*/
	
	// Stop handling status messages from CustomStatusCursors
	CustomStatusCursor::SetStatusDisplay(0);
	myStatusDisplay.SetFrame(0);
	
	CMDIFrameWnd::OnClose();
}

void CMainFrame::FreeRegExpList()
{
	struct regexp_list *r;
	r = ignore_regexp_list;
	// iterate through the list, free the reg expression
	// list item
	while (ignore_regexp_list)
	{
		r = r->next;
		free((ignore_regexp_list->buf).fastmap);
		free((ignore_regexp_list->buf).buffer);
		free(ignore_regexp_list);
		ignore_regexp_list = r;
	}
}

void CMainFrame::RebuildRegExpList()
{
	USES_CONVERSION;

	_TCHAR tmp[_MAX_PATH] = {0};
	_TCHAR* token;
	_TCHAR sep[] = _T("\r\n");
	
	// destroy the old list if the it is not NULL
	FreeRegExpList();

	// build the new list if the user choose to
	// ignore lines matching the reg expression patterns
	if (m_bIgnoreRegExp)
	{
		// find each regular expression and add to list
		_tcscpy(tmp, m_sPattern);

		token = _tcstok(tmp, sep);
		while (token)
		{
			add_regexp(&ignore_regexp_list, T2A(token));
			token = _tcstok(NULL, sep);
		}
	}

	if (ignore_regexp_list)
	{
		ignore_some_changes = 1;
	}
}

// Add the compiled form of regexp pattern to reglist
static void
add_regexp(struct regexp_list **reglist,
     char const* pattern)
{
  struct regexp_list *r;
  char const *m;

  r = (struct regexp_list *) xmalloc (sizeof (*r));
  bzero (r, sizeof (*r));
  r->buf.fastmap = (char*) xmalloc (256);
  m = re_compile_pattern (pattern, strlen (pattern), &r->buf);
  if (m != 0)
    error ("%s: %s", pattern, m);

  /* Add to the start of the list, since it's easier than the end.  */
  r->next = *reglist;
  *reglist = r;
}

/**
 * @brief Utility function to update CSuperComboBox format MRU
 */
void CMainFrame::addToMru(LPCTSTR szItem, LPCTSTR szRegSubKey, UINT nMaxItems)
{
	CString s,s2;
	UINT cnt = AfxGetApp()->GetProfileInt(szRegSubKey, _T("Count"), 0);
	++cnt;	// add new string
	if(cnt>nMaxItems)
	{
		cnt=nMaxItems;
	}
	// move items down a step
	for (UINT i=cnt ; i!=0; --i)
	{
		s2.Format(_T("Item_%d"), i-1);
		s = AfxGetApp()->GetProfileString(szRegSubKey, s2);
		s2.Format(_T("Item_%d"), i);
		AfxGetApp()->WriteProfileString(szRegSubKey, s2, s);
	}
	// add most recent item
	AfxGetApp()->WriteProfileString(szRegSubKey, _T("Item_0"), szItem);
	// update count
	AfxGetApp()->WriteProfileInt(szRegSubKey, _T("Count"), cnt);
}

/**
 * @brief Apply tabs and eols settings to all merge documents
 */
void CMainFrame::ApplyViewWhitespace() 
{
	MergeDocList mergedocs;
	GetAllMergeDocs(&mergedocs);
	while (!mergedocs.IsEmpty())
	{
		CMergeDoc * pMergeDoc = mergedocs.RemoveHead();
		CMergeEditView * pLeft = pMergeDoc->GetLeftView();
		CMergeEditView * pRight = pMergeDoc->GetRightView();
		CMergeDiffDetailView * pLeftDetail = pMergeDoc->GetLeftDetailView();
		CMergeDiffDetailView * pRightDetail = pMergeDoc->GetRightDetailView();
		if (pLeft)
		{
			pLeft->SetViewTabs(mf->m_options.GetInt(OPT_VIEW_WHITESPACE));
			pLeft->SetViewEols(mf->m_options.GetInt(OPT_VIEW_WHITESPACE),
				mf->m_options.GetInt(OPT_ALLOW_MIXED_EOL));
		}
		if (pRight)
		{
			pRight->SetViewTabs(mf->m_options.GetInt(OPT_VIEW_WHITESPACE));
			pRight->SetViewEols(mf->m_options.GetInt(OPT_VIEW_WHITESPACE),
				mf->m_options.GetInt(OPT_ALLOW_MIXED_EOL));
		}
		if (pLeftDetail)
		{
			pLeftDetail->SetViewTabs(mf->m_options.GetInt(OPT_VIEW_WHITESPACE));
			pLeftDetail->SetViewEols(mf->m_options.GetInt(OPT_VIEW_WHITESPACE),
				mf->m_options.GetInt(OPT_ALLOW_MIXED_EOL));
		}
		if (pRightDetail)
		{
			pRightDetail->SetViewTabs(mf->m_options.GetInt(OPT_VIEW_WHITESPACE));
			pRightDetail->SetViewEols(mf->m_options.GetInt(OPT_VIEW_WHITESPACE),
				mf->m_options.GetInt(OPT_ALLOW_MIXED_EOL));
		}
	}
}

void CMainFrame::OnViewWhitespace() 
{
	BOOL bViewWhitespace = m_options.GetInt(OPT_VIEW_WHITESPACE);
	bViewWhitespace = !bViewWhitespace;
	m_options.SaveOption(OPT_VIEW_WHITESPACE, bViewWhitespace);
	ApplyViewWhitespace();
}

/// Enables View/View Whitespace menuitem when merge view is active
void CMainFrame::OnUpdateViewWhitespace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_options.GetInt(OPT_VIEW_WHITESPACE));
}

/// Get list of MergeDocs (documents underlying edit sessions)
void CMainFrame::GetAllMergeDocs(MergeDocList * pMergeDocs)
{
	CMultiDocTemplate * pTemplate = theApp.m_pDiffTemplate;
	for (POSITION pos = pTemplate->GetFirstDocPosition(); pos; )
	{
		CDocument * pDoc = pTemplate->GetNextDoc(pos);
		CMergeDoc * pMergeDoc = static_cast<CMergeDoc *>(pDoc);
		pMergeDocs->AddTail(pMergeDoc);
	}
}

/// Get list of DirDocs (documents underlying a scan)
void CMainFrame::GetAllDirDocs(DirDocList * pDirDocs)
{
	CMultiDocTemplate * pTemplate = theApp.m_pDirTemplate;
	for (POSITION pos = pTemplate->GetFirstDocPosition(); pos; )
	{
		CDocument * pDoc = pTemplate->GetNextDoc(pos);
		CDirDoc * pDirDoc = static_cast<CDirDoc *>(pDoc);
		pDirDocs->AddTail(pDirDoc);
	}
}

/// Get pointers to all views into typed lists (both arguments are optional)
void CMainFrame::GetAllViews(MergeEditViewList * pEditViews, MergeDetailViewList * pDetailViews, DirViewList * pDirViews)
{
	for (POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition(); pos; )
	{
		CDocTemplate * pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
		for (POSITION pos2 = pTemplate->GetFirstDocPosition(); pos2; )
		{
			CDocument * pDoc = pTemplate->GetNextDoc(pos2);
			CMergeDoc * pMergeDoc = dynamic_cast<CMergeDoc *>(pDoc);
			CDirDoc * pDirDoc = dynamic_cast<CDirDoc *>(pDoc);
			for (POSITION pos3 = pDoc->GetFirstViewPosition(); pos3; )
			{
				CView * pView = pDoc->GetNextView(pos3);
				if (pMergeDoc)
				{
					if (pEditViews || pDetailViews)
					{
						// a merge doc only has merge edit views or diff detail views
						CMergeEditView * pEditView = dynamic_cast<CMergeEditView *>(pView);
						CMergeDiffDetailView * pDetailView = dynamic_cast<CMergeDiffDetailView *>(pView);
						ASSERT(pEditView || pDetailView);
						if (pEditView)
						{
							if (pEditViews)
								pEditViews->AddTail(pEditView);
						}
						else if (pDetailView)
						{
							if (pDetailViews)
								pDetailViews->AddTail(pDetailView);
						}
					}
				}
				else if (pDirDoc)
				{
					if (pDirViews)
					{
						// a dir doc only has dir views
						CDirView * pDirView = dynamic_cast<CDirView *>(pView);
						ASSERT(pDirView);
						pDirViews->AddTail(pDirView);
					}
				}
				else
				{
					// There are currently only two types of docs 2003-02-20
					ASSERT(0);
				}
			}
		}
	}
}

/// Obtain a merge doc to display a difference in files
CMergeDoc * CMainFrame::GetMergeDocToShow(CDirDoc * pDirDoc, BOOL * pNew)
{
	CMergeDoc * pMergeDoc = pDirDoc->GetMergeDocForDiff(pNew);
	return pMergeDoc;
}

/// Get pointer to a dir doc for displaying a scan
CDirDoc * CMainFrame::GetDirDocToShow(BOOL * pNew)
{
	CDirDoc * pDirDoc = 0;
	if (m_bReuseDirDoc)
	{
		POSITION pos = theApp.m_pDirTemplate->GetFirstDocPosition();
		if (pos)
		{
			pDirDoc = static_cast<CDirDoc *>(theApp.m_pDirTemplate->GetNextDoc(pos));
			*pNew = FALSE;
		}
	}
	if (!pDirDoc)
	{
		pDirDoc = (CDirDoc*)theApp.m_pDirTemplate->OpenDocumentFile(NULL);
		*pNew = TRUE;
	}
	return pDirDoc;
}

// Set status in the main status pane
CString CMainFrame::SetStatus(LPCTSTR status)
{
	CString old = m_wndStatusBar.GetPaneText(0);
	m_wndStatusBar.SetPaneText(0, status);
	return old;
}

/**
 * @brief Generate patch from files selected
 */
void CMainFrame::OnToolsGeneratePatch()
{
	CPatchTool patcher;
	CFrameWnd * pFrame = GetActiveFrame();
	BOOL bOpenDialog = TRUE;

	// Mergedoc active?
	if (pFrame->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		// Add open files to patch-list
		MergeDocList mergedocs;
		GetAllMergeDocs(&mergedocs);
		while (!mergedocs.IsEmpty())
		{
			CMergeDoc * pMergeDoc = mergedocs.RemoveHead();

			// If user cancels, don't open create patch-dialog
			if (!pMergeDoc->SaveHelper(TRUE))
			{
				bOpenDialog = FALSE;
			}
			patcher.AddFiles(pMergeDoc->m_strLeftFile, pMergeDoc->m_strRightFile);
		}
	}
	// Dirview active
	else if (pFrame->IsKindOf(RUNTIME_CLASS(CDirFrame)))
	{
		CDirDoc * pDoc = (CDirDoc*)pFrame->GetActiveDocument();
		CDirView *pView = pDoc->GetMainView();

		// Get first selected item from dirview to patch dialog
		int ind = pView->GetFirstSelectedInd();
		if (ind != -1)
		{
			DIFFITEM item = pView->GetItemAt(ind);
		
			if (!item.isBin() && !item.isDirectory() &&	!item.isResultError())
			{
				CString leftFile = item.getLeftFilepath();
				if (!leftFile.IsEmpty())
					leftFile += _T("\\") + item.sfilename;
				CString rightFile = item.getRightFilepath();
				if (!rightFile.IsEmpty())
					rightFile += _T("\\") + item.sfilename;
				
				patcher.AddFiles(leftFile, rightFile);
			}
		}
	}

	if (bOpenDialog)
	{
		if (patcher.CreatePatch())
		{
			if (patcher.GetOpenToEditor())
			{
				OpenFileToExternalEditor(patcher.GetPatchFile());
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	OnDropFiles code from CDropEdit
//	Copyright 1997 Chris Losinger
//
//	shortcut expansion code modified from :
//	CShortcut, 1996 Rob Warner
//
void CMainFrame::OnDropFiles(HDROP dropInfo)
{
	// Get the number of pathnames that have been dropped
	UINT wNumFilesDropped = DragQueryFile(dropInfo, 0xFFFFFFFF, NULL, 0);
	CString files[2];
	UINT fileCount = 0;

	// get all file names. but we'll only need the first one.
	for (WORD x = 0 ; x < wNumFilesDropped; x++)
	{
		// Get the number of bytes required by the file's full pathname
		UINT wPathnameSize = DragQueryFile(dropInfo, x, NULL, 0);

		// Allocate memory to contain full pathname & zero byte
		wPathnameSize += 1;
		LPTSTR npszFile = (TCHAR *) new TCHAR[wPathnameSize];

		// If not enough memory, skip this one
		if (npszFile == NULL)
			continue;

		// Copy the pathname into the buffer
		DragQueryFile(dropInfo, x, npszFile, wPathnameSize);

		if (x < 2)
		{
			files[x] = npszFile;
			fileCount++;
		}
		delete[] npszFile;
	}

	// Free the memory block containing the dropped-file information
	DragFinish(dropInfo);

	for (UINT i = 0; i < fileCount; i++)
	{
		// if this was a shortcut, we need to expand it to the target path
		CString expandedFile = ExpandShortcut(files[i]);

		// if that worked, we should have a real file name
		if (!expandedFile.IsEmpty()) 
			files[i] = expandedFile;
	}

	// If Ctrl pressed, do recursive compare
	BOOL ctrlKey = ::GetAsyncKeyState(VK_CONTROL);

	// If user has <Shift> pressed with one file selected,
	// assume it is an archive and set filenames to same
	if (::GetAsyncKeyState(VK_SHIFT) < 0 && fileCount == 1)
	{
		files[1] = files[0];
	}

	gLog.Write(LOGLEVEL::LNOTICE, _T("D&D open: Left: %s\n\tRight: %s."),
		files[0], files[1]);

	DoFileOpen(files[0], files[1], FFILEOPEN_NONE, FFILEOPEN_NONE, ctrlKey);
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (CustomStatusCursor::HasWaitCursor())
	{
		CustomStatusCursor::RestoreWaitCursor();
		return TRUE;
	}
	return CMDIFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::OnPluginUnpackMode(UINT nID )
{
	switch (nID)
	{
	case ID_UNPACK_MANUAL:
		m_bUnpackerMode = UNPACK_MANUAL;
		break;
	case ID_UNPACK_AUTO:
		m_bUnpackerMode = UNPACK_AUTO;
		break;
	}
	theApp.WriteProfileInt(_T("Settings"), _T("UnpackerMode"), m_bUnpackerMode);
}

void CMainFrame::OnUpdatePluginUnpackMode(CCmdUI* pCmdUI) 
{
	if (pCmdUI->m_nID == ID_UNPACK_MANUAL)
		pCmdUI->SetRadio(UNPACK_MANUAL == m_bUnpackerMode);
	if (pCmdUI->m_nID == ID_UNPACK_AUTO)
		pCmdUI->SetRadio(UNPACK_AUTO == m_bUnpackerMode);
}

/**
 * @brief Open given file to external editor specified in options
 */
BOOL CMainFrame::OpenFileToExternalEditor(CString file)
{
	CString ext;
	SplitFilename(m_sExtEditorPath, NULL, NULL, &ext);
	ext.MakeLower();

	if (ext == _T("exe") || ext == _T("cmd") || ext == ("bat"))
	{
		// Format command line
		CString strCommandLine = _T("\"") + m_sExtEditorPath + _T("\" \"") +
			file + _T("\"");

		BOOL retVal = FALSE;
		STARTUPINFO stInfo = {0};
		stInfo.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION processInfo;

		retVal = CreateProcess(NULL, (LPTSTR)(LPCTSTR) strCommandLine,
			NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL,
			&stInfo, &processInfo);

		if (!retVal)
		{
			CString msg;
			AfxFormatString1(msg, IDS_CANNOT_EXECUTE_FILE, m_sExtEditorPath);
			AfxMessageBox(msg, MB_ICONSTOP);
		}
	}
	else
	{
		CString msg;
		AfxFormatString1(msg, IDS_CANNOT_EXECUTE_FILE, m_sExtEditorPath);
		AfxMessageBox(msg, MB_ICONSTOP);
	}
	return TRUE;
}

/**
 * @brief Get default editor path
 */
CString CMainFrame::GetDefaultEditor()
{
	CString path;
	GetWindowsDirectory(path.GetBuffer(MAX_PATH), MAX_PATH);
	path.ReleaseBuffer();
	path += _T("\\NOTEPAD.EXE");
	return path;
}

/**
 * @brief Save WinMerge configuration and info to file
 */
void CMainFrame::OnSaveConfigData()
{
	CConfigLog configLog;
	
	configLog.viewSettings.bShowIdent = m_options.GetInt(OPT_SHOW_DIFFERENT);
	configLog.viewSettings.bShowDiff = m_options.GetInt(OPT_SHOW_DIFFERENT);
	configLog.viewSettings.bShowUniqueLeft = m_options.GetInt(OPT_SHOW_UNIQUE_LEFT);
	configLog.viewSettings.bShowUniqueRight = m_options.GetInt(OPT_SHOW_UNIQUE_RIGHT);
	configLog.viewSettings.bShowBinaries = m_options.GetInt(OPT_SHOW_BINARIES);
	configLog.viewSettings.bShowSkipped = m_options.GetInt(OPT_SHOW_SKIPPED);
	configLog.viewSettings.bHideBak = m_options.GetInt(OPT_HIDE_BACKUP);

	configLog.miscSettings.bAutomaticRescan = m_options.GetInt(OPT_AUTOMATIC_RESCAN);
	configLog.miscSettings.bAllowMixedEol = m_options.GetInt(OPT_ALLOW_MIXED_EOL);
	configLog.miscSettings.bScrollToFirst = m_options.GetInt(OPT_SCROLL_TO_FIRST);
	configLog.miscSettings.bBackup = m_options.GetInt(OPT_CREATE_BACKUPS);
	configLog.miscSettings.bViewWhitespace = m_options.GetInt(OPT_VIEW_WHITESPACE);

	if (configLog.WriteLogFile())
	{
		CString sFileName = configLog.GetFileName();
		OpenFileToExternalEditor(sFileName);
	}
}

/**
 * @brief Open two new empty docs, 'Scratchpads'
 * 
 * Allows user to open two empty docs, to paste text to
 * compare from clipboard.
 * @note File filenames are set emptys and filedescriptors
 * are loaded from resource.
 * @sa CMergeDoc::OpenDocs()
 * @sa CMergeDoc::TrySaveAs()
 */
void CMainFrame::OnFileNew() 
{
	BOOL docNull;
	CDirDoc *pDirDoc = GetDirDocToShow(&docNull);

	// If the dirdoc we are supposed to use is busy doing a diff, bail out
	UINT threadState = pDirDoc->m_diffThread.GetThreadState();
	if (threadState == THREAD_COMPARING)
		return;

	if (!docNull)
	{
		// If reusing an existing doc, give it a chance to save its data
		// and close any merge views, and clear its window
		if (!pDirDoc->ReusingDirDoc())
			return;
	}
	
	// Load emptyfile descriptors and open empty docs
	VERIFY(m_strLeftDesc.LoadString(IDS_EMPTY_LEFT_FILE));
	VERIFY(m_strRightDesc.LoadString(IDS_EMPTY_RIGHT_FILE));
	ShowMergeDoc(pDirDoc, _T(""), _T(""), FALSE, FALSE, 0, 0);
}

