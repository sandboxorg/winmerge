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
#include "LocationView.h"

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
#include "Plugins.h"
#include "SelectUnpackerDlg.h"
#include "files.h"
#include "ConfigLog.h"
#include "7zCommon.h"
#include <shlwapi.h>
#include "FileFiltersDlg.h"
#include "OptionsMgr.h"
#include "OptionsDef.h"
#include "UniFile.h"
#include "unicoder.h"

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
	ON_WM_MENUCHAR()
	ON_WM_MEASUREITEM()
	ON_WM_INITMENUPOPUP()
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
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_HELP_GNULICENSE, OnHelpGnulicense)
	ON_COMMAND(ID_OPTIONS, OnOptions)
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
	ON_UPDATE_COMMAND_UI(ID_RELOAD_PLUGINS, OnUpdateReloadPlugins)
	ON_COMMAND(ID_RELOAD_PLUGINS, OnReloadPlugins)
	ON_COMMAND(ID_HELP_GETCONFIG, OnSaveConfigData)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_TOOLS_FILTERS, OnToolsFilters)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 * @brief MainFrame statusbar panels/indicators
 */
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // Merge mode
	ID_SEPARATOR,           // Diff number
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
	m_bFontSpecified=FALSE;
	m_strSaveAsPath = _T("");
	m_bFirstTime = TRUE;
	m_bEscShutdown = FALSE;

	m_options.SetRegRootKey(_T("Thingamahoochie\\WinMerge\\"));

	// Initialise options (name, default value)
	m_options.InitOption(OPT_SHOW_UNIQUE_LEFT, TRUE);
	m_options.InitOption(OPT_SHOW_UNIQUE_RIGHT, TRUE);
	m_options.InitOption(OPT_SHOW_DIFFERENT, TRUE);
	m_options.InitOption(OPT_SHOW_IDENTICAL, TRUE);
	m_options.InitOption(OPT_SHOW_BINARIES, TRUE);
	m_options.InitOption(OPT_SHOW_SKIPPED, TRUE);
	m_options.InitOption(OPT_HIDE_BACKUP, TRUE);

	m_options.InitOption(OPT_SYNTAX_HIGHLIGHT, TRUE);
	m_options.InitOption(OPT_DISABLE_SPLASH, FALSE);
	m_options.InitOption(OPT_CREATE_BACKUPS, TRUE);
	m_options.InitOption(OPT_VIEW_WHITESPACE, FALSE);
	m_options.InitOption(OPT_CONNECT_MOVED_BLOCKS, FALSE);
	m_options.InitOption(OPT_SCROLL_TO_FIRST, FALSE);
	m_options.InitOption(OPT_VERIFY_OPEN_PATHS, TRUE);

	m_options.InitOption(OPT_AUTOMATIC_RESCAN, FALSE);
	m_options.InitOption(OPT_ALLOW_MIXED_EOL, FALSE);
	m_options.InitOption(OPT_TAB_SIZE, 4);
	m_options.InitOption(OPT_TAB_TYPE, 0);	// 0 means tabs inserted

	m_options.InitOption(OPT_EXT_EDITOR_CMD, _T(""));
	m_options.InitOption(OPT_USE_RECYCLE_BIN, TRUE);
	m_options.InitOption(OPT_AUTOCLOSE_CMPPANE, FALSE);
	m_options.InitOption(OPT_MERGE_MODE, FALSE);
	m_options.InitOption(OPT_UNREC_APPLYSYNTAX, FALSE);
	m_options.InitOption(OPT_CLOSE_WITH_ESC, TRUE);
	m_options.InitOption(OPT_DETECT_CODEPAGE, FALSE);
	m_options.InitOption(OPT_LOGGING, 0);

	m_options.InitOption(OPT_CMP_IGNORE_WHITESPACE, 0);
	m_options.InitOption(OPT_CMP_IGNORE_BLANKLINES, FALSE);
	m_options.InitOption(OPT_CMP_IGNORE_CASE, FALSE);
	m_options.InitOption(OPT_CMP_EOL_SENSITIVE, TRUE);
	m_options.InitOption(OPT_CMP_METHOD, CMP_CONTENT);
	m_options.InitOption(OPT_CMP_MOVED_BLOCKS, FALSE);

	m_nCompMethod = m_options.GetInt(OPT_CMP_METHOD);

	m_options.InitOption(OPT_CLR_DIFF, RGB(239,203,5));
	m_options.InitOption(OPT_CLR_SELECTED_DIFF, RGB(239,119,116));
	m_options.InitOption(OPT_CLR_DIFF_DELETED, RGB(192, 192, 192));
	m_options.InitOption(OPT_CLR_SELECTED_DIFF_DELETED, RGB(240, 192, 192));
	m_options.InitOption(OPT_CLR_DIFF_TEXT, RGB(0,0,0));
	m_options.InitOption(OPT_CLR_SELECTED_DIFF_TEXT, RGB(0,0,0));
	m_options.InitOption(OPT_CLR_TRIVIAL_DIFF, RGB(251,242,191));
	m_options.InitOption(OPT_CLR_TRIVIAL_DIFF_DELETED, RGB(233,233,233));
	m_options.InitOption(OPT_CLR_TRIVIAL_DIFF_TEXT, RGB(0,0,0));
	m_options.InitOption(OPT_CLR_MOVEDBLOCK, RGB(228,155,82));
	m_options.InitOption(OPT_CLR_MOVEDBLOCK_DELETED, RGB(192, 192, 192));
	m_options.InitOption(OPT_CLR_MOVEDBLOCK_TEXT, RGB(0,0,0));
	m_options.InitOption(OPT_CLR_SELECTED_MOVEDBLOCK, RGB(248,112,78));
	m_options.InitOption(OPT_CLR_SELECTED_MOVEDBLOCK_DELETED, RGB(252, 181, 163));
	m_options.InitOption(OPT_CLR_SELECTED_MOVEDBLOCK_TEXT, RGB(0,0,0));

	// Overwrite WinMerge 2.0 default colors with new colors
	if (m_options.GetInt(OPT_CLR_DIFF) == RGB(255,255,92))
		m_options.SaveOption(OPT_CLR_DIFF, RGB(239,203,5));
	if (m_options.GetInt(OPT_CLR_SELECTED_DIFF) == RGB(255,0,92))
		m_options.SaveOption(OPT_CLR_SELECTED_DIFF, RGB(239,119,116));

	m_bShowErrors = TRUE;
	m_CheckOutMulti = FALSE;
	m_bVCProjSync = FALSE;
	m_bVssSuppressPathCheck = FALSE;

	m_nVerSys = theApp.GetProfileInt(_T("Settings"), _T("VersionSystem"), 0);
	m_strVssProjectBase = theApp.GetProfileString(_T("Settings"), _T("VssProject"), _T(""));
	m_strVssUser = theApp.GetProfileString(_T("Settings"), _T("VssUser"), _T(""));
//	m_strVssPassword = theApp.GetProfileString(_T("Settings"), _T("VssPassword"), _T(""));
	theApp.WriteProfileString(_T("Settings"), _T("VssPassword"), _T(""));
	m_strVssPath = theApp.GetProfileString(_T("Settings"), _T("VssPath"), _T(""));
	m_strVssDatabase = theApp.GetProfileString(_T("Settings"), _T("VssDatabase"),_T(""));
	m_bIgnoreRegExp = theApp.GetProfileInt(_T("Settings"), _T("IgnoreRegExp"), FALSE);
	m_sPattern = theApp.GetProfileString(_T("Settings"), _T("RegExps"), NULL);
	theApp.m_globalFileFilter.SetFileFilterPath(theApp.GetProfileString(_T("Settings"), _T("FileFilterPath"), _T("")));
	g_bUnpackerMode = theApp.GetProfileInt(_T("Settings"), _T("UnpackerMode"), PLUGIN_MANUAL);
	// uncomment this when the GUI allows to toggle the mode
//	g_bPredifferMode = theApp.GetProfileInt(_T("Settings"), _T("PredifferMode"), PLUGIN_MANUAL);


	m_bReuseDirDoc = TRUE;
	// TODO: read preference for logging

	if (m_options.GetString(OPT_EXT_EDITOR_CMD).IsEmpty())
		m_options.SaveOption(OPT_EXT_EDITOR_CMD, GetDefaultEditor());

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

	int logging = m_options.GetInt(OPT_LOGGING);
	if (logging > 0)
	{
		gLog.EnableLogging(TRUE);
		gLog.SetFile(_T("WinMerge.log"));

		if (logging == 1)
			gLog.SetMaskLevel(LOGLEVEL::LALL);
		else if (logging == 2)
			gLog.SetMaskLevel(LOGLEVEL::LERROR | LOGLEVEL::LWARNING);
	}
}

CMainFrame::~CMainFrame()
{
	gLog.EnableLogging(FALSE);

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
	
	m_wndToolBar.SetBorders(1, 1, 1, 1);
	if (!m_wndToolBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_GRIPPER|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	VERIFY(m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT));
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo(1, ID_STATUS_MERGINGMODE, 0, 100); 
	m_wndStatusBar.SetPaneInfo(2, ID_STATUS_DIFFNUM, 0, 150); 

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

/* Uncomment to enable document menus
HMENU CMainFrame::NewMenu()
{
	m_menu.LoadMenu(IDR_MAINFRAME);
	m_menu.ModifyODMenu(NULL, ID_WINDOW_TILE_VERT, IDB_WIN_VERTICAL);
	m_menu.LoadToolbar(IDR_MAINFRAME);
	return(m_menu.Detach());
}
*/

/**
 * @brief Find the scripts submenu from the main menu
 * As now this is the first submenu in "Edit" menu
 */
HMENU CMainFrame::GetScriptsSubmenu(HMENU mainMenu)
{
	// look for "Edit" menu
	int i;
	for (i = 0 ; i < ::GetMenuItemCount(mainMenu) ; i++)
		if (::GetMenuItemID(::GetSubMenu(mainMenu, i), 0) == ID_EDIT_UNDO)
			break;
	HMENU editMenu = ::GetSubMenu(mainMenu, i);

	// look for "script" submenu (first submenu)
	for (i = 0 ; i < ::GetMenuItemCount(editMenu) ; i++)
		if (::GetSubMenu(editMenu, i) != NULL)
			return ::GetSubMenu(editMenu, i);

	// error, submenu not found
	return NULL;
}

/**
 * @brief Find the scripts submenu from the main menu
 * As now this is the first submenu in "Plugins" menu
 */
HMENU CMainFrame::GetPrediffersSubmenu(HMENU mainMenu)
{
	// look for "Plugins" menu
	int i;
	for (i = 0 ; i < ::GetMenuItemCount(mainMenu) ; i++)
		if (::GetMenuItemID(::GetSubMenu(mainMenu, i), 0) == ID_UNPACK_MANUAL)
			break;
	HMENU editMenu = ::GetSubMenu(mainMenu, i);

	// look for "script" submenu (first submenu)
	for (i = 0 ; i < ::GetMenuItemCount(editMenu) ; i++)
		if (::GetSubMenu(editMenu, i) != NULL)
			return ::GetSubMenu(editMenu, i);

	// error, submenu not found
	return NULL;
}

/**
 * @brief Create new default (CMainFrame) menu
 */
HMENU CMainFrame::NewDefaultMenu(int ID /*=0*/)
{
	if (ID == 0)
		ID = IDR_MAINFRAME;

	if (!m_default.LoadMenu(ID))
	{
		ASSERT(FALSE);
		return NULL;
	}
	
	// Load bitmaps to menuitems
	m_default.ModifyODMenu(NULL, ID_EDIT_COPY, IDB_EDIT_COPY);
	m_default.ModifyODMenu(NULL, ID_EDIT_CUT, IDB_EDIT_CUT);
	m_default.ModifyODMenu(NULL, ID_EDIT_PASTE, IDB_EDIT_PASTE);
	m_default.ModifyODMenu(NULL, ID_EDIT_FIND, IDB_EDIT_SEARCH);
	m_default.ModifyODMenu(NULL, ID_WINDOW_CASCADE, IDB_WINDOW_CASCADE);
	m_default.ModifyODMenu(NULL, ID_WINDOW_TILE_HORZ, IDB_WINDOW_HORIZONTAL);
	m_default.ModifyODMenu(NULL, ID_WINDOW_TILE_VERT, IDB_WINDOW_VERTICAL);
	m_default.ModifyODMenu(NULL, ID_FILE_CLOSE, IDB_WINDOW_CLOSE);
	m_default.ModifyODMenu(NULL, ID_WINDOW_CHANGE_PANE, IDB_WINDOW_CHANGEPANE);
	m_default.ModifyODMenu(NULL, ID_EDIT_WMGOTO, IDB_EDIT_GOTO);
	m_default.ModifyODMenu(NULL, ID_EDIT_REPLACE, IDB_EDIT_REPLACE);
	m_default.ModifyODMenu(NULL, ID_VIEW_LANGUAGE, IDB_VIEW_LANGUAGE);
	m_default.ModifyODMenu(NULL, ID_VIEW_SELECTFONT, IDB_VIEW_SELECTFONT);
	m_default.ModifyODMenu(NULL, ID_APP_EXIT, IDB_FILE_EXIT);
	m_default.ModifyODMenu(NULL, ID_HELP_CONTENTS, IDB_HELP_CONTENTS);
	m_default.ModifyODMenu(NULL, ID_EDIT_SELECT_ALL, IDB_EDIT_SELECTALL);
	m_default.ModifyODMenu(NULL, ID_TOOLS_FILTERS, IDB_TOOLS_FILTERS);
	m_default.ModifyODMenu(NULL, ID_TOOLS_CUSTOMIZECOLUMNS, IDB_TOOLS_COLUMNS);
	m_default.LoadToolbar(IDR_MAINFRAME);

	// append the scripts submenu
	HMENU scriptsSubmenu = GetScriptsSubmenu(m_default.GetSafeHmenu());
	if (scriptsSubmenu != NULL)
		CMergeEditView::createScriptsSubmenu(scriptsSubmenu);

	return(m_default.Detach());
}

/**
 * @brief This handler ensures that the popup menu items are drawn correctly.
 */
void CMainFrame::OnMeasureItem(int nIDCtl,
	LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	BOOL setflag = FALSE;
	if (lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		if (IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			CMenu* cmenu =
				CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);

			//if (m_menu.IsMenu(cmenu) || m_default.IsMenu(cmenu))
			if (m_default.IsMenu(cmenu))
			{
				//m_menu.MeasureItem(lpMeasureItemStruct);
				m_default.MeasureItem(lpMeasureItemStruct);
				setflag = TRUE;
			}
		}
	}

	if (!setflag)
		CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

/**
 * @brief This handler ensures that keyboard shortcuts work.
 */
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

/**
 * @brief This handler updates the menus from time to time.
 */
void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	if (!bSysMenu)
	{
		if (BCMenu::IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
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

/**
 * @brief Creates new MergeDoc instance and shows documents
 *
 * @param cpleft, cpright : left and right codepages
 * = -1 when the file must be parsed
 */
void CMainFrame::ShowMergeDoc(CDirDoc * pDirDoc, LPCTSTR szLeft, LPCTSTR szRight,
	BOOL bROLeft, BOOL bRORight,  int cpleft /*=-1*/, int cpright /*=-1*/,
	PackingInfo * infoUnpacker /*= NULL*/)
{
	BOOL docNull;
	BOOL bOpenSuccess = FALSE;
	CMergeDoc * pMergeDoc = GetMergeDocToShow(pDirDoc, &docNull);

	ASSERT(pMergeDoc);		// must ASSERT to get an answer to the question below ;-)
	if (!pMergeDoc) return; // when does this happen ?

	// if an unpacker is selected, it must be used during LoadFromFile
	// MergeDoc must memorize it for SaveToFile
	// Warning : this unpacker may differ from the pDirDoc one
	// (through menu : "Plugins"->"Open with unpacker")
	pMergeDoc->SetUnpacker(infoUnpacker);

	// detect codepage
	BOOL bGuessEncoding = mf->m_options.GetInt(OPT_DETECT_CODEPAGE);
	if (cpleft == -1)
	{
		CString filepath = szLeft;
		int unicoding;
		GuessCodepageEncoding(filepath, &unicoding, &cpleft, bGuessEncoding);
	}
	if (cpright == -1)
	{
		CString filepath = szRight;
		int unicoding;
		GuessCodepageEncoding(filepath, &unicoding, &cpright, bGuessEncoding);
	}

	if (cpleft != cpright)
	{
		// Two files with different codepages
		// Warn and propose to use the default codepage for both
		int userChoice = AfxMessageBox(IDS_SUGGEST_IGNORECODEPAGE, 
		                               MB_YESNO | MB_ICONQUESTION | MB_DONT_ASK_AGAIN);
		if (userChoice == IDYES)
			cpleft = cpright = ucr::getDefaultCodepage();
	}

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

	ASSERT(pfile != NULL && pfile != INVALID_HANDLE_VALUE);
	ZeroMemory(&cbuffer, sizeof(cbuffer));
	
	while (numread == sizeof(ctemp) && buffercount < sizeof(cbuffer))
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
	if (buffercount >= sizeof(cbuffer) || numread == 0)
		return FALSE;
	return TRUE;
}

/**
 * @brief Checks if path (file/folder) is read-only and asks overwriting it.
 *
 * @param strSavePath [in,out] Path where to save (file or folder)
 * @param bMultiFile [in] Single file or multiple files/folder
 * @param bApplyToAll [in,out] Apply last user selection for all items?
 * @return Users selection:
 * - IDOK: Item was not readonly, no actions
 * - IDYES/IDYESTOALL: Overwrite readonly item
 * - IDNO: User selected new filename (single file) or user wants to skip
 * - IDCANCEL: Cancel operation
 * @sa CMainFrame::SyncFileToVCS()
 * @sa CMergeDoc::DoSave()
 */
int CMainFrame::HandleReadonlySave(CString& strSavePath, BOOL bMultiFile,
		BOOL &bApplyToAll)
{
	CFileStatus status;
	UINT userChoice = 0;
	int nRetVal = IDOK;
	BOOL bFileRO = FALSE;
	BOOL bFileExists = FALSE;
	CString s;
	CString title;

	bFileRO = files_isFileReadOnly(strSavePath, &bFileExists);
	
	if (bFileExists && bFileRO)
	{
		// Version control system used?
		// Checkout file from VCS and modify, don't ask about overwriting
		// RO files etc.
		if (m_nVerSys > 0)
		{
			BOOL bRetVal = SaveToVersionControl(strSavePath);
			if (bRetVal)
				return IDYES;
			else
				return IDCANCEL;
		}
		
		// Don't ask again if its already asked
		if (bApplyToAll)
			userChoice = IDYES;
		else
		{
			// Prompt for user choice
			if (bMultiFile)
			{
				// Multiple files or folder
				AfxFormatString1(s, IDS_SAVEREADONLY_MULTI, strSavePath);
				userChoice = AfxMessageBox(s, MB_YESNOCANCEL |
						MB_ICONQUESTION | MB_DEFBUTTON3 | MB_DONT_ASK_AGAIN |
						MB_YES_TO_ALL, IDS_SAVEREADONLY_MULTI);
			}
			else
			{
				// Single file
				AfxFormatString1(s, IDS_SAVEREADONLY_FMT, strSavePath);
				userChoice = AfxMessageBox(s, MB_YESNOCANCEL |
						MB_ICONQUESTION | MB_DEFBUTTON2 | MB_DONT_ASK_AGAIN,
						IDS_SAVEREADONLY_FMT);
			}
		}
		switch (userChoice)
		{
		// Overwrite read-only file
		case IDYESTOALL:
			bApplyToAll = TRUE;  // Don't ask again (no break here)
		case IDYES:
			CFile::GetStatus(strSavePath, status);
			status.m_mtime = 0;		// Avoid unwanted changes
			status.m_attribute &= ~CFile::Attribute::readOnly;
			CFile::SetStatus(strSavePath, status);
			nRetVal = IDYES;
			break;
		
		// Save to new filename (single) /skip this item (multiple)
		case IDNO:
			if (!bMultiFile)
			{
				VERIFY(title.LoadString(IDS_SAVE_AS_TITLE));
				if (SelectFile(s, strSavePath, title, NULL, FALSE))
				{
					strSavePath = s;
					nRetVal = IDNO;
				}
				else
					nRetVal = IDCANCEL;
			}
			else
				nRetVal = IDNO;
			break;

		// Cancel saving
		case IDCANCEL:
			nRetVal = IDCANCEL;
			break;
		}
	}
	return nRetVal;
}

/**
* @brief Saves file to selected version control system
* @param strSavePath Path where to save including filename
* @return Tells if caller can continue (no errors happened)
* @sa CheckSavePath()
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
		// Prompt for user choice
		CVssPrompt dlg;
		dlg.m_strMessage.FormatMessage(IDS_SAVE_FMT, strSavePath);
		dlg.m_strProject = m_strVssProjectBase;
		dlg.m_strUser = m_strVssUser;          // BSP - Add VSS user name to dialog box
		dlg.m_strPassword = m_strVssPassword;

		// Dialog not suppressed - show it and allow user to select "checkout all"
		if (!m_CheckOutMulti)
		{
			dlg.m_bMultiCheckouts = FALSE;
			userChoice = dlg.DoModal();
			m_CheckOutMulti = dlg.m_bMultiCheckouts;
		}
		else // Dialog already shown and user selected to "checkout all"
			userChoice = IDOK;

		// process versioning system specific action
		if (userChoice == IDOK)
		{
			VERIFY(s.LoadString(IDS_VSS_CHECKOUT_STATUS));
			WaitStatusCursor waitstatus(s);
			m_strVssProjectBase = dlg.m_strProject;
			theApp.WriteProfileString(_T("Settings"), _T("VssProject"), mf->m_strVssProjectBase);
			CString spath, sname;
			SplitFilename(strSavePath, &spath, &sname, NULL);
			if (!spath.IsEmpty())
			{
				_chdrive(_totupper(spath[0]) - 'A' + 1);
				_tchdir(spath);
			}
			CString args;
			args.Format(_T("checkout \"%s/%s\""), m_strVssProjectBase, sname);
			HANDLE hVss = RunIt(m_strVssPath, args, TRUE, FALSE);
			if (hVss != INVALID_HANDLE_VALUE)
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

		// Dialog not suppressed - show it and allow user to select "checkout all"
		if (!m_CheckOutMulti)
		{
			dlg.m_bMultiCheckouts = FALSE;
			userChoice = dlg.DoModal();
			m_CheckOutMulti = dlg.m_bMultiCheckouts;
		}
		else // Dialog already shown and user selected to "checkout all"
			userChoice = IDOK;

		// process versioning system specific action
		if (userChoice == IDOK)
		{
			VERIFY(s.LoadString(IDS_VSS_CHECKOUT_STATUS));
			WaitStatusCursor waitstatus(s);
			BOOL bOpened = FALSE;
			m_strVssProjectBase = dlg.m_strProject;
			m_strVssUser = dlg.m_strUser;
			m_strVssPassword = dlg.m_strPassword;
			m_strVssDatabase = dlg.m_strSelectedDatabase;
			m_bVCProjSync = dlg.m_bVCProjSync;					

			theApp.WriteProfileString(_T("Settings"), _T("VssDatabase"), m_strVssDatabase);
			theApp.WriteProfileString(_T("Settings"), _T("VssProject"), m_strVssProjectBase);
			theApp.WriteProfileString(_T("Settings"), _T("VssUser"), m_strVssUser);
//			theApp.WriteProfileString(_T("Settings"), _T("VssPassword"), m_strVssPassword);

			IVSSDatabase vssdb;
			IVSSItems vssis;
			IVSSItem vssi;

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
			if (!m_strVssDatabase.IsEmpty())
			{
				CString iniPath = m_strVssDatabase + _T("\\srcsafe.ini");
				TRY
				{
					// BSP - Open the specific VSS data file  using info from VSS dialog box
					vssdb.Open(iniPath, m_strVssUser, m_strVssPassword);
				}
				CATCH_ALL(e)
				{
					ShowVSSError(e, _T(""));
				}
				END_CATCH_ALL

				bOpened = TRUE;
			}
			
			if (bOpened == FALSE)
			{
				CString iniPath = m_strVssDatabase + _T("\\srcsafe.ini");
				TRY
				{
					// BSP - Open the specific VSS data file  using info from VSS dialog box
					//let vss try to find one if not specified
					vssdb.Open(NULL, m_strVssUser, m_strVssPassword);
				}
				CATCH_ALL(e)
				{
					ShowVSSError(e, _T(""));
					return FALSE;
				}
				END_CATCH_ALL
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

			TRY
			{
				//  BSP - ...to get the specific source safe item to be checked out
				vssi = vssdb.GetVSSItem( strItem, 0 );
			}
			CATCH_ALL(e)
			{
				ShowVSSError(e, strItem);
				return FALSE;
			}
			END_CATCH_ALL

			if (!m_bVssSuppressPathCheck)
			{
				// BSP - Get the working directory where VSS will put the file...
				CString strLocalSpec = vssi.GetLocalSpec();

				// BSP - ...and compare it to the directory WinMerge is using.
				if (strLocalSpec.CompareNoCase(strSavePath))
				{
					// BSP - if the directories are different, let the user confirm the CheckOut
					int iRes = AfxMessageBox(IDS_VSSFOLDER_AND_FILE_NOMATCH, 
							MB_YESNO | MB_YES_TO_ALL | MB_ICONQUESTION);

					if (iRes == IDNO)
					{
						m_bVssSuppressPathCheck = FALSE;
						m_CheckOutMulti = FALSE; // Reset, we don't want 100 of the same errors
						return FALSE;   // No means user has to start from begin
					}
					else if (iRes = IDYESTOALL)
						m_bVssSuppressPathCheck = TRUE; // Don't ask again with selected files
				}
			}

			TRY
			{
				// BSP - Finally! Check out the file!
				vssi.Checkout(_T(""), strSavePath, 0);
			}
			CATCH_ALL(e)
			{
				ShowVSSError(e, strSavePath);
				return FALSE;
			}
			END_CATCH_ALL
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

/**
 * @brief Opens Options-dialog and saves changed options
 */
void CMainFrame::OnOptions() 
{
	CString sExtEditor;
	CPropertySheet sht(IDS_OPTIONS_TITLE);
	CPropVss vss;
	CPropGeneral gen;
	CPropColors colors(&m_options);
	CPropRegistry regpage;
    CPropCompare compage(&m_options);
	CPropEditor editor;
	sht.m_psh.dwFlags |= PSH_NOAPPLYNOW; // Hide 'Apply' button since we don't need it
   
	sht.AddPage(&gen);
	sht.AddPage(&compage);
	sht.AddPage(&editor);
	sht.AddPage(&vss);
	sht.AddPage(&colors);
	sht.AddPage(&regpage);
	
	vss.m_nVerSys = m_nVerSys;
	vss.m_strPath = m_strVssPath;
	gen.m_bBackup = m_options.GetInt(OPT_CREATE_BACKUPS);
	gen.m_bScroll = m_options.GetInt(OPT_SCROLL_TO_FIRST);
	gen.m_bDisableSplash = m_options.GetInt(OPT_DISABLE_SPLASH);
	gen.m_bAutoCloseCmpPane = m_options.GetInt(OPT_AUTOCLOSE_CMPPANE);
	gen.m_bVerifyPaths = m_options.GetInt(OPT_VERIFY_OPEN_PATHS);
	gen.m_bCloseWindowWithEsc = m_options.GetInt(OPT_CLOSE_WITH_ESC);
	gen.m_bDetectCodepage = m_options.GetInt(OPT_DETECT_CODEPAGE);
	regpage.m_strEditorPath = m_options.GetString(OPT_EXT_EDITOR_CMD);
	regpage.GetContextRegValues();
	regpage.m_bUseRecycleBin = m_options.GetInt(OPT_USE_RECYCLE_BIN);
    compage.m_compareMethod = m_options.GetInt(OPT_CMP_METHOD);
	compage.m_nIgnoreWhite = m_options.GetInt(OPT_CMP_IGNORE_WHITESPACE);
	compage.m_bIgnoreBlankLines = m_options.GetInt(OPT_CMP_IGNORE_BLANKLINES);
	compage.m_bIgnoreCase = m_options.GetInt(OPT_CMP_IGNORE_CASE);
	compage.m_bEolSensitive = m_options.GetInt(OPT_CMP_EOL_SENSITIVE) ? FALSE : TRUE; // Reverse
	compage.m_bMovedBlocks = m_options.GetInt(OPT_CMP_MOVED_BLOCKS);
	editor.m_nTabSize = m_options.GetInt(OPT_TAB_SIZE);
	editor.m_nTabType = m_options.GetInt(OPT_TAB_TYPE);
	editor.m_bAutomaticRescan = m_options.GetInt(OPT_AUTOMATIC_RESCAN);
	editor.m_bHiliteSyntax = m_options.GetInt(OPT_SYNTAX_HIGHLIGHT);
	editor.m_bAllowMixedEol = m_options.GetInt(OPT_ALLOW_MIXED_EOL);
	editor.m_bApplySyntax = m_options.GetInt(OPT_UNREC_APPLYSYNTAX);

	if (sht.DoModal()==IDOK)
	{
		m_nVerSys = vss.m_nVerSys;
		m_strVssPath = vss.m_strPath;
		
		m_options.SaveOption(OPT_CREATE_BACKUPS, gen.m_bBackup);
		m_options.SaveOption(OPT_SCROLL_TO_FIRST, gen.m_bScroll);
		m_options.SaveOption(OPT_DISABLE_SPLASH, gen.m_bDisableSplash);
		m_options.SaveOption(OPT_AUTOCLOSE_CMPPANE, gen.m_bAutoCloseCmpPane);
		m_options.SaveOption(OPT_VERIFY_OPEN_PATHS, gen.m_bVerifyPaths);
		m_options.SaveOption(OPT_CLOSE_WITH_ESC, gen.m_bCloseWindowWithEsc);
		m_options.SaveOption(OPT_DETECT_CODEPAGE, gen.m_bDetectCodepage);
		m_options.SaveOption(OPT_USE_RECYCLE_BIN, regpage.m_bUseRecycleBin);
		regpage.SaveMergePath();
		sExtEditor = regpage.m_strEditorPath;
		sExtEditor.TrimLeft();
		sExtEditor.TrimRight();
		if (sExtEditor.IsEmpty())
			sExtEditor = GetDefaultEditor();
		m_options.SaveOption(OPT_EXT_EDITOR_CMD, sExtEditor);

		m_options.SaveOption(OPT_CMP_IGNORE_WHITESPACE, compage.m_nIgnoreWhite);
		m_options.SaveOption(OPT_CMP_IGNORE_BLANKLINES, compage.m_bIgnoreBlankLines);
		m_options.SaveOption(OPT_CMP_EOL_SENSITIVE, compage.m_bEolSensitive ? FALSE : TRUE); // Reverse
		m_options.SaveOption(OPT_CMP_IGNORE_CASE, compage.m_bIgnoreCase);
        m_options.SaveOption(OPT_CMP_METHOD, compage.m_compareMethod);
		m_options.SaveOption(OPT_CMP_MOVED_BLOCKS, compage.m_bMovedBlocks);
		
		m_nCompMethod = compage.m_compareMethod;

		m_options.SaveOption(OPT_TAB_SIZE, editor.m_nTabSize);
		m_options.SaveOption(OPT_TAB_TYPE, editor.m_nTabType);
		m_options.SaveOption(OPT_AUTOMATIC_RESCAN, editor.m_bAutomaticRescan);
		m_options.SaveOption(OPT_ALLOW_MIXED_EOL, editor.m_bAllowMixedEol);
		m_options.SaveOption(OPT_SYNTAX_HIGHLIGHT, editor.m_bHiliteSyntax);
		m_options.SaveOption(OPT_UNREC_APPLYSYNTAX, editor.m_bApplySyntax);

		theApp.WriteProfileInt(_T("Settings"), _T("VersionSystem"), m_nVerSys);
		theApp.WriteProfileString(_T("Settings"), _T("VssPath"), m_strVssPath);

		m_options.SaveOption(OPT_CLR_DIFF, colors.m_clrDiff);
		m_options.SaveOption(OPT_CLR_SELECTED_DIFF, colors.m_clrSelDiff);
		m_options.SaveOption(OPT_CLR_DIFF_DELETED, colors.m_clrDiffDeleted);
		m_options.SaveOption(OPT_CLR_SELECTED_DIFF_DELETED, colors.m_clrSelDiffDeleted);
		m_options.SaveOption(OPT_CLR_DIFF_TEXT, colors.m_clrDiffText);
		m_options.SaveOption(OPT_CLR_SELECTED_DIFF_TEXT, colors.m_clrSelDiffText);
		m_options.SaveOption(OPT_CLR_TRIVIAL_DIFF, colors.m_clrTrivial);
		m_options.SaveOption(OPT_CLR_TRIVIAL_DIFF_DELETED, colors.m_clrTrivialDeleted);
		m_options.SaveOption(OPT_CLR_TRIVIAL_DIFF_TEXT, colors.m_clrTrivialText);
		m_options.SaveOption(OPT_CLR_MOVEDBLOCK, colors.m_clrMoved);
		m_options.SaveOption(OPT_CLR_MOVEDBLOCK_DELETED, colors.m_clrMovedDeleted);
		m_options.SaveOption(OPT_CLR_MOVEDBLOCK_TEXT, colors.m_clrMovedText);
		m_options.SaveOption(OPT_CLR_SELECTED_MOVEDBLOCK, colors.m_clrSelMoved);
		m_options.SaveOption(OPT_CLR_SELECTED_MOVEDBLOCK_DELETED, colors.m_clrSelMovedDeleted);
		m_options.SaveOption(OPT_CLR_SELECTED_MOVEDBLOCK_TEXT, colors.m_clrSelMovedText);
		
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
			pMergeDoc->RefreshOptions();
			
			// Enable/disable automatic rescan (rescan after editing)
			pLeft->EnableRescan(m_options.GetInt(OPT_AUTOMATIC_RESCAN));
			pRight->EnableRescan(m_options.GetInt(OPT_AUTOMATIC_RESCAN));

			// Set tab type (tabs (==0)/spaces (==1))
			if (m_options.GetInt(OPT_TAB_TYPE) == 0)
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
			pDirDoc->RefreshOptions();
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
	PackingInfo infoUnpacker;
	// TODO: Need to allow user to specify these some day
	int cpleft= -1; // to be initialized/guessed in ShowMergeDoc
	int cpright= -1; // to be initialized/guessed in ShowMergeDoc

	BOOL bRORight = dwLeftFlags & FFILEOPEN_READONLY;
	BOOL bROLeft = dwRightFlags & FFILEOPEN_READONLY;
	BOOL docNull;
	CDirDoc * pDirDoc = GetDirDocToShow(&docNull);

	// If the dirdoc we are supposed to use is busy doing a diff, bail out
	UINT threadState = pDirDoc->m_diffThread.GetThreadState();
	if (threadState == THREAD_COMPARING)
		return FALSE;

	// pop up dialog unless arguments exist (and are compatible)
	PATH_EXISTENCE pathsType = GetPairComparability(strLeft, strRight);
	if (pathsType == DOES_NOT_EXIST)
	{
		COpenDlg dlg;
		dlg.m_strLeft = strLeft;
		dlg.m_strRight = strRight;
		dlg.m_bRecurse = bRecurse;

		if (dwLeftFlags & FFILEOPEN_CMDLINE || dwLeftFlags & FFILEOPEN_CMDLINE)
			dlg.m_bOverwriteRecursive = TRUE;

		if (dlg.DoModal() != IDOK)
			return FALSE;

		strLeft = dlg.m_strLeft;
		strRight = dlg.m_strRight;
		bRecurse = dlg.m_bRecurse;
		infoUnpacker = dlg.m_infoHandler;
		pathsType = static_cast<PATH_EXISTENCE>(dlg.m_pathsType);
		// TODO: add codepage options to open dialog ?
		cpleft= -1; // to be initialized/guessed in ShowMergeDoc
		cpright= -1; // to be initialized/guessed in ShowMergeDoc
	}
	else
	{
		//save the MRU left and right files.
		if (!(dwLeftFlags & FFILEOPEN_NOMRU))
			addToMru(pszLeft, _T("Files\\Left"));
		if (!(dwRightFlags & FFILEOPEN_NOMRU))
			addToMru(pszRight, _T("Files\\Right"));
	}

	if (!docNull)
	{
		// If reusing an existing doc, give it a chance to save its data
		// and close any merge views, and clear its window
		if (!pDirDoc->ReusingDirDoc())
			return FALSE;
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
				pDirDoc->SetDiffContext(pCtxt);
				pDirDoc->SetDescriptions(m_strLeftDesc, m_strRightDesc);
				m_strLeftDesc.Empty();
				m_strRightDesc.Empty();

				pDirDoc->Rescan();
			}
		}
	}
	else
	{
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
					MB_YESNO | MB_ICONQUESTION | MB_DONT_ASK_AGAIN, 
					IDS_BACKUP_FAILED_PROMPT) != IDYES)
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
 * @param pszSrc [in] File to copy
 * @param pszDest [in] Where to copy (incl. filename)
 * @param bApplyToAll [in,out] Apply user selection to all items
 * @param psError [out] Error string that can be shown to user in caller func
 * @return User selection or -1 if error happened
 * @sa CMainFrame::HandleReadonlySave()
 * @sa CDirView::PerformActionList()
 */
int CMainFrame::SyncFileToVCS(LPCTSTR pszSrc, LPCTSTR pszDest,
	BOOL &bApplyToAll, CString *psError)
{
	CString sActionError;
	CString strSavePath(pszDest);
	
	int nRetVal = HandleReadonlySave(strSavePath, TRUE, bApplyToAll);
	if (nRetVal == IDCANCEL || nRetVal == IDNO)
		return nRetVal;
	
	if (!CreateBackup(strSavePath))
	{
		psError->LoadString(IDS_ERROR_BACKUP);
		return -1;
	}
	
	// If VC project opened from VSS sync and version control used
	if (m_nVerSys > 0 && m_bVCProjSync)
	{
		if (!ReLinkVCProj(strSavePath, psError))
			nRetVal = -1;
	}
	return nRetVal;
}

void CMainFrame::GetFullVSSPath(CString strSavePath, BOOL & bVCProj)
{
	CString strExt;
	CString spath;

	SplitFilename(strSavePath, NULL, NULL, &strExt);
	if (strExt.CompareNoCase(_T("vcproj")))
		bVCProj = TRUE;
		SplitFilename(strSavePath, &spath, NULL, NULL);
		
	strSavePath.Replace('/', '\\');
	m_strVssProjectBase.Replace('/', '\\');

		//check if m_strVssProjectBase has leading $\\, if not put them in:
	if (m_strVssProjectBase[0] != '$' && m_strVssProjectBase[1] != '\\')
		m_strVssProjectBase.Insert(0, _T("$\\"));

	strSavePath.MakeLower();
	m_strVssProjectBase.MakeLower();
		
		//take out last '\\'
	int nLen = m_strVssProjectBase.GetLength();
	if (m_strVssProjectBase[nLen - 1] == '\\')
		m_strVssProjectBase.Delete(nLen - 1, 1);

	CString strSearch = m_strVssProjectBase.Mid(2); // Don't compare first 2
	int index = strSavePath.Find(strSearch); //Search for project base path
	if (index > -1)
		{
		index++;
		m_strVssProjectFull = strSavePath.Mid(index + strSearch.GetLength());
			if (m_strVssProjectFull[0] == ':')
			{
			m_strVssProjectFull.Delete(0, 2);
			}
		}

		SplitFilename(m_strVssProjectFull, &spath, NULL, NULL);
	if (m_strVssProjectBase[m_strVssProjectBase.GetLength() - 1] != '\\')
		{
		m_strVssProjectBase += "\\";
		}

		m_strVssProjectFull = m_strVssProjectBase + spath;
		
		//if sln file, we need to replace ' '  with _T("\\u0020")
	if (!bVCProj)
		m_strVssProjectFull.Replace( _T(" "), _T("\\u0020"));
}

BOOL CMainFrame::ReLinkVCProj(CString strSavePath, CString * psError)
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
				{
			LogErrorString(_T("CMainFrame::ReLinkVCProj() - couldn't get tempfile!"));
			return FALSE;
		}
				}
				else
				{
		LogErrorString(_T("CMainFrame::ReLinkVCProj() - couldn't get temppath!"));
		return FALSE;
		}

	CString strExt;
	SplitFilename(strSavePath, NULL, NULL, &strExt);
	if (strExt.CompareNoCase(_T("vcproj")) == 0 || strExt.CompareNoCase(_T("sln")))
	{
		GetFullVSSPath(strSavePath, bVCPROJ);

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
		
		if (hfile == INVALID_HANDLE_VALUE || tfile == INVALID_HANDLE_VALUE)
		{
			CString msg;
			if (hfile == INVALID_HANDLE_VALUE)
			{
				msg.Format(_T("CMainFrame::ReLinkVCProj() ")
					_T("- failed to open file: %s"), strSavePath);
				LogErrorString(msg);
				AfxFormatString2(msg, IDS_ERROR_FILEOPEN,
						GetSysError(GetLastError()), strSavePath);
				AfxMessageBox(msg, MB_ICONSTOP);
			}
			if (tfile == INVALID_HANDLE_VALUE)
			{
				msg.Format(_T("CMainFrame::ReLinkVCProj() ")
					_T("- failed to open temporary file: %s"), tempFile);
				LogErrorString(msg);
				AfxFormatString2(msg, IDS_ERROR_FILEOPEN,
						GetSysError(GetLastError()), tempFile);
				AfxMessageBox(msg, MB_ICONSTOP);
			}
			return FALSE;
		}

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

		AfxMessageBox(IDS_FONT_CHANGE, MB_ICONINFORMATION | MB_DONT_DISPLAY_AGAIN, IDS_FONT_CHANGE);

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
	CString url = _T("http://winmerge.org/docs20/index.html");

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
				// Don't get Location View (font don't change for this view)
				if (pView->IsKindOf(RUNTIME_CLASS(CLocationView)))
					continue;
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
			const DIFFITEM item = pView->GetItemAt(ind);
			if (item.isBin())
				AfxMessageBox(IDS_CANNOT_CREATE_BINARYPATCH, MB_ICONWARNING |
					MB_DONT_DISPLAY_AGAIN, IDS_CANNOT_CREATE_BINARYPATCH);
			else if (item.isDirectory())
				AfxMessageBox(IDS_CANNOT_CREATE_DIRPATCH, MB_ICONWARNING |
					MB_DONT_DISPLAY_AGAIN, IDS_CANNOT_CREATE_DIRPATCH);

			CString leftFile = item.getLeftFilepath(pDoc->m_pCtxt);
			if (!leftFile.IsEmpty())
				leftFile += _T("\\") + item.sfilename;
			CString rightFile = item.getRightFilepath(pDoc->m_pCtxt);
			if (!rightFile.IsEmpty())
				rightFile += _T("\\") + item.sfilename;

			patcher.AddFiles(leftFile, rightFile);
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
		g_bUnpackerMode = PLUGIN_MANUAL;
		break;
	case ID_UNPACK_AUTO:
		g_bUnpackerMode = PLUGIN_AUTO;
		break;
	}
	theApp.WriteProfileInt(_T("Settings"), _T("UnpackerMode"), g_bUnpackerMode);
}

void CMainFrame::OnUpdatePluginUnpackMode(CCmdUI* pCmdUI) 
{
	if (pCmdUI->m_nID == ID_UNPACK_MANUAL)
		pCmdUI->SetRadio(PLUGIN_MANUAL == g_bUnpackerMode);
	if (pCmdUI->m_nID == ID_UNPACK_AUTO)
		pCmdUI->SetRadio(PLUGIN_AUTO == g_bUnpackerMode);
}

/**
 * @brief Called when "Reload Plugins" item is updated
 */
void CMainFrame::OnUpdateReloadPlugins(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnReloadPlugins()
{
	// delete all script interfaces
	// (interfaces will be created again automatically when WinMerge needs them)
	CAllThreadsScripts::GetActiveSet()->FreeAllScripts();

	// update the editor scripts submenu
	HMENU scriptsSubmenu = GetScriptsSubmenu(m_hMenuDefault);
	if (scriptsSubmenu != NULL)
		CMergeEditView::createScriptsSubmenu(scriptsSubmenu);
	UpdatePrediffersMenu();
}

void CMainFrame::UpdatePrediffersMenu()
{
		HMENU prediffersSubmenu = GetPrediffersSubmenu(m_hMenuDefault);
		if (prediffersSubmenu != NULL)
	{
		CMergeEditView * pEditView = dynamic_cast<CMergeEditView*> (GetActiveFrame()->GetActiveView());
		if (pEditView)
			pEditView->createPrediffersSubmenu(prediffersSubmenu);
		else
		{
			// no view or dir view : display an empty submenu
			int i = GetMenuItemCount(prediffersSubmenu);
			while (i --)
				::DeleteMenu(prediffersSubmenu, 0, MF_BYPOSITION);
			::AppendMenu(prediffersSubmenu, MF_SEPARATOR, 0, NULL);
		}
	}
}

/**
 * @brief Open given file to external editor specified in options
 */
BOOL CMainFrame::OpenFileToExternalEditor(CString file)
{
	CString sExtEditor;
	CString ext;
	CString sExecutable;
	CString sCmd;
	
	sExtEditor = m_options.GetString(OPT_EXT_EDITOR_CMD);
	GetDecoratedCmdLine(sExtEditor, sCmd, sExecutable);

	SplitFilename(sExecutable, NULL, NULL, &ext);
	ext.MakeLower();

	if (ext == _T("exe") || ext == _T("cmd") || ext == ("bat"))
	{
		sCmd += _T(" \"") + file + _T("\"");

		BOOL retVal = FALSE;
		STARTUPINFO stInfo = {0};
		stInfo.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION processInfo;

		retVal = CreateProcess(NULL, (LPTSTR)(LPCTSTR) sCmd,
			NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL,
			&stInfo, &processInfo);

		if (!retVal)
		{
			// Error invoking external editor
			CString msg;
			AfxFormatString1(msg, IDS_CANNOT_EXECUTE_FILE, sExtEditor);
			AfxMessageBox(msg, MB_ICONSTOP);
		}
	}
	else
	{
		// Don't know how to invoke external editor (it doesn't end with
		// an obvious executable extension)
		CString msg;
		AfxFormatString1(msg, IDS_CANNOT_EXECUTE_FILE, sExtEditor);
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

	configLog.m_diffOptions.nIgnoreWhitespace = mf->m_options.GetInt(OPT_CMP_IGNORE_WHITESPACE);
	configLog.m_diffOptions.bIgnoreBlankLines = mf->m_options.GetInt(OPT_CMP_IGNORE_BLANKLINES);
	configLog.m_diffOptions.bIgnoreCase = mf->m_options.GetInt(OPT_CMP_IGNORE_CASE);
	configLog.m_diffOptions.bEolSensitive = mf->m_options.GetInt(OPT_CMP_EOL_SENSITIVE);
	
	configLog.m_viewSettings.bShowIdent = m_options.GetInt(OPT_SHOW_DIFFERENT);
	configLog.m_viewSettings.bShowDiff = m_options.GetInt(OPT_SHOW_DIFFERENT);
	configLog.m_viewSettings.bShowUniqueLeft = m_options.GetInt(OPT_SHOW_UNIQUE_LEFT);
	configLog.m_viewSettings.bShowUniqueRight = m_options.GetInt(OPT_SHOW_UNIQUE_RIGHT);
	configLog.m_viewSettings.bShowBinaries = m_options.GetInt(OPT_SHOW_BINARIES);
	configLog.m_viewSettings.bShowSkipped = m_options.GetInt(OPT_SHOW_SKIPPED);
	configLog.m_viewSettings.bHideBak = m_options.GetInt(OPT_HIDE_BACKUP);

	configLog.m_miscSettings.bAutomaticRescan = m_options.GetInt(OPT_AUTOMATIC_RESCAN);
	configLog.m_miscSettings.bAllowMixedEol = m_options.GetInt(OPT_ALLOW_MIXED_EOL);
	configLog.m_miscSettings.bScrollToFirst = m_options.GetInt(OPT_SCROLL_TO_FIRST);
	configLog.m_miscSettings.bBackup = m_options.GetInt(OPT_CREATE_BACKUPS);
	configLog.m_miscSettings.bViewWhitespace = m_options.GetInt(OPT_VIEW_WHITESPACE);
	configLog.m_miscSettings.bMovedBlocks = m_options.GetInt(OPT_CMP_MOVED_BLOCKS);
	configLog.m_miscSettings.bDetectCodepage = m_options.GetInt(OPT_DETECT_CODEPAGE);

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
		// If dircompare contains results, warn user that they are lost
		if (pDirDoc->m_pCtxt)
		{
			int res = AfxMessageBox(IDS_DIR_RESULTS_EMPTIED, MB_OKCANCEL |
				MB_ICONWARNING | MB_DONT_DISPLAY_AGAIN, IDS_DIR_RESULTS_EMPTIED);
			if (res == IDCANCEL)
				return;
		}

		// If reusing an existing doc, give it a chance to save its data
		// and close any merge views, and clear its window
		if (!pDirDoc->ReusingDirDoc())
			return;
	}
	
	// Load emptyfile descriptors and open empty docs
	// Use default codepage
	VERIFY(m_strLeftDesc.LoadString(IDS_EMPTY_LEFT_FILE));
	VERIFY(m_strRightDesc.LoadString(IDS_EMPTY_RIGHT_FILE));
	ShowMergeDoc(pDirDoc, _T(""), _T(""), FALSE, FALSE, 
	             ucr::getDefaultCodepage(), ucr::getDefaultCodepage());
	
	// Empty descriptors now that docs are open
	m_strLeftDesc.Empty();
	m_strRightDesc.Empty();
}

/**
 * @brief Open Filters dialog
 */
void CMainFrame::OnToolsFilters()
{
	CPropertySheet sht(IDS_FILTER_TITLE);
	CPropFilter filter;
	FileFiltersDlg fileFiltersDlg;
	StringPairArray fileFilters;
	CString selectedFilter;
	sht.AddPage(&fileFiltersDlg);
	sht.AddPage(&filter);
	sht.m_psh.dwFlags |= PSH_NOAPPLYNOW; // Hide 'Apply' button since we don't need it

	theApp.m_globalFileFilter.GetFileFilters(&fileFilters, selectedFilter);
	fileFiltersDlg.SetFilterArray(&fileFilters);
	fileFiltersDlg.SetSelected(selectedFilter);
	filter.m_bIgnoreRegExp = m_bIgnoreRegExp;
	filter.m_sPattern = m_sPattern;

	if (sht.DoModal() == IDOK)
	{
		CString path = fileFiltersDlg.GetSelected();
		theApp.m_globalFileFilter.SetFileFilterPath(path);
		theApp.WriteProfileString(_T("Settings"), _T("FileFilterPath"), path);

		m_bIgnoreRegExp = filter.m_bIgnoreRegExp;
		m_sPattern = filter.m_sPattern;

		theApp.WriteProfileInt(_T("Settings"), _T("IgnoreRegExp"), m_bIgnoreRegExp);
		theApp.WriteProfileString(_T("Settings"), _T("RegExps"), m_sPattern);

		RebuildRegExpList();

	}
}

/**
 * @brief Open Filters dialog.
 */
void CMainFrame::SelectFilter()
{
	OnToolsFilters();
}

/**
 * @brief Closes application with ESC.
 *
 * Application is closed if:
 * - 'Close Windows with ESC' option is enabled
 * - there is no open document windows
 */
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// Check if we got 'ESC pressed' -message
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		if (m_bEscShutdown)
		{
			AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_APP_EXIT);
			return TRUE;
		}
		else
		{
			if (m_options.GetInt(OPT_CLOSE_WITH_ESC))
			{
				MergeDocList docs;
				GetAllMergeDocs(&docs);
				DirDocList dirDocs;
				GetAllDirDocs(&dirDocs);

				if (docs.IsEmpty() && dirDocs.IsEmpty())
				{
					AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT);
					return FALSE;
				}
			}
		}
	}
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

/**
 * @brief Shows VSS error from exception and writes log.
 */
void CMainFrame::ShowVSSError(CException *e, CString strItem)
{
	CString errMsg;
	CString logMsg;
	TCHAR errStr[1024] = {0};

	VERIFY(errMsg.LoadString(IDS_VSS_ERRORFROM));
	if (e->GetErrorMessage(errStr, 1024))
	{
		logMsg = errMsg;
		errMsg += _T("\n");
		errMsg += errStr;
		logMsg += _T(" ");
		logMsg += errStr;
		if (!strItem.IsEmpty())
		{
			errMsg += _T("\n\n");
			errMsg += strItem;
			logMsg += _T(": ");
			logMsg += strItem;
		}
		LogErrorString(logMsg);
		AfxMessageBox(errMsg, MB_ICONSTOP);
	}
	else
		e->ReportError(MB_ICONSTOP, IDS_VSS_RUN_ERROR);
}

/**
 * @brief Show Help - this is for opening help from outside mainframe.
 */
void CMainFrame::ShowHelp()
{
	OnHelpContents();
}
