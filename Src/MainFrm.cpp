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
// ID line follows -- this is updated by SVN
// $Id: MainFrm.cpp 7063 2009-12-27 15:28:16Z kimmov $

#include "StdAfx.h"
#include <vector>
#include <htmlhelp.h>  // From HTMLHelp Workshop (incl. in Platform SDK)
#include <shlwapi.h>
#include "Constants.h"
#include "Merge.h"
#include "UnicodeString.h"
#include "BCMenu.h"
#include "MainFrm.h"
#include "OpenFrm.h"
#include "DirFrame.h"		// Include type information
#include "ChildFrm.h"
#include "HexMergeFrm.h"
#include "DirView.h"
#include "DirDoc.h"
#include "OpenDoc.h"
#include "OpenView.h"
#include "MergeEditView.h"
#include "HexMergeDoc.h"
#include "LocationView.h"
#include "SyntaxColors.h"
#include "LineFiltersList.h"
#include "ConflictFileParser.h"
#include "Splash.h"
#include "LineFiltersDlg.h"
#include "paths.h"
#include "Environment.h"
#include "WaitStatusCursor.h"
#include "PatchTool.h"
#include "Plugins.h"
#include "SelectUnpackerDlg.h"
#include "ConfigLog.h"
#include "7zCommon.h"
#include "FileFiltersDlg.h"
#include "OptionsMgr.h"
#include "OptionsDef.h"
#include "codepage_detect.h"
#include "unicoder.h"
#include "VSSHelper.h"
#include "codepage.h"
#include "PreferencesDlg.h"
#include "ProjectFilePathsDlg.h"
#include "MergeCmdLineInfo.h"
#include "FileOrFolderSelect.h"
#include "PropBackups.h"
#include "unicoder.h"
#include "PluginsListDlg.h"
#include "stringdiffs.h"
#include "MergeCmdLineInfo.h"
#include "OptionsSyntaxColors.h"
#include "OptionsFont.h"
#include "TFile.h"
#include "JumpList.h"
#include "DragDrop.h"
#include <Poco/Exception.h>

using std::vector;

/*
 One source file must compile the stubs for multimonitor
 by defining the symbol COMPILE_MULTIMON_STUBS & including <multimon.h>
*/
#ifdef COMPILE_MULTIMON_STUBS
#undef COMPILE_MULTIMON_STUBS
#endif
#define COMPILE_MULTIMON_STUBS
#include <multimon.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void LoadToolbarImageList(CMainFrame::TOOLBAR_SIZE size, UINT nIDResource, CImageList& ImgList);
static const CPtrList &GetDocList(const CMultiDocTemplate *pTemplate);

/**
 * @brief A table associating menuitem id, icon and menus to apply.
 */
const CMainFrame::MENUITEM_ICON CMainFrame::m_MenuIcons[] = {
	{ ID_FILE_OPENCONFLICT,			IDB_FILE_OPENCONFLICT,			CMainFrame::MENU_ALL },
	{ ID_EDIT_COPY,					IDB_EDIT_COPY,					CMainFrame::MENU_ALL },
	{ ID_EDIT_CUT,					IDB_EDIT_CUT,					CMainFrame::MENU_ALL },
	{ ID_EDIT_PASTE,				IDB_EDIT_PASTE,					CMainFrame::MENU_ALL },
	{ ID_EDIT_FIND,					IDB_EDIT_SEARCH,				CMainFrame::MENU_ALL },
	{ ID_WINDOW_CASCADE,			IDB_WINDOW_CASCADE,				CMainFrame::MENU_ALL },
	{ ID_WINDOW_TILE_HORZ,			IDB_WINDOW_HORIZONTAL,			CMainFrame::MENU_ALL },
	{ ID_WINDOW_TILE_VERT,			IDB_WINDOW_VERTICAL,			CMainFrame::MENU_ALL },
	{ ID_FILE_CLOSE,				IDB_WINDOW_CLOSE,				CMainFrame::MENU_ALL },
	{ ID_WINDOW_CHANGE_PANE,		IDB_WINDOW_CHANGEPANE,			CMainFrame::MENU_ALL },
	{ ID_EDIT_WMGOTO,				IDB_EDIT_GOTO,					CMainFrame::MENU_ALL },
	{ ID_EDIT_REPLACE,				IDB_EDIT_REPLACE,				CMainFrame::MENU_ALL },
	{ ID_VIEW_LANGUAGE,				IDB_VIEW_LANGUAGE,				CMainFrame::MENU_ALL },
	{ ID_VIEW_SELECTFONT,			IDB_VIEW_SELECTFONT,			CMainFrame::MENU_ALL },
	{ ID_APP_EXIT,					IDB_FILE_EXIT,					CMainFrame::MENU_ALL },
	{ ID_HELP_CONTENTS,				IDB_HELP_CONTENTS,				CMainFrame::MENU_ALL },
	{ ID_EDIT_SELECT_ALL,			IDB_EDIT_SELECTALL,				CMainFrame::MENU_ALL },
	{ ID_TOOLS_FILTERS,				IDB_TOOLS_FILTERS,				CMainFrame::MENU_ALL },
	{ ID_TOOLS_CUSTOMIZECOLUMNS,	IDB_TOOLS_COLUMNS,				CMainFrame::MENU_ALL },
	{ ID_TOOLS_GENERATEPATCH,		IDB_TOOLS_GENERATEPATCH,		CMainFrame::MENU_ALL },
	{ ID_PLUGINS_LIST,				IDB_PLUGINS_LIST,				CMainFrame::MENU_ALL },
	{ ID_FILE_PRINT,				IDB_FILE_PRINT,					CMainFrame::MENU_FILECMP },
	{ ID_TOOLS_GENERATEREPORT,		IDB_TOOLS_GENERATEREPORT,		CMainFrame::MENU_FILECMP },
	{ ID_EDIT_TOGGLE_BOOKMARK,		IDB_EDIT_TOGGLE_BOOKMARK,		CMainFrame::MENU_FILECMP },
	{ ID_EDIT_GOTO_NEXT_BOOKMARK,	IDB_EDIT_GOTO_NEXT_BOOKMARK,	CMainFrame::MENU_FILECMP },
	{ ID_EDIT_GOTO_PREV_BOOKMARK,	IDB_EDIT_GOTO_PREV_BOOKMARK,	CMainFrame::MENU_FILECMP },
	{ ID_EDIT_CLEAR_ALL_BOOKMARKS,	IDB_EDIT_CLEAR_ALL_BOOKMARKS,	CMainFrame::MENU_FILECMP },
	{ ID_VIEW_ZOOMIN,				IDB_VIEW_ZOOMIN,				CMainFrame::MENU_FILECMP },
	{ ID_VIEW_ZOOMOUT,				IDB_VIEW_ZOOMOUT,				CMainFrame::MENU_FILECMP },
	{ ID_MERGE_COMPARE,				IDB_MERGE_COMPARE,				CMainFrame::MENU_FOLDERCMP },
	{ ID_MERGE_COMPARE_LEFT1_LEFT2,		IDB_MERGE_COMPARE_LEFT1_LEFT2,	CMainFrame::MENU_FOLDERCMP },
	{ ID_MERGE_COMPARE_RIGHT1_RIGHT2,	IDB_MERGE_COMPARE_RIGHT1_RIGHT2,CMainFrame::MENU_FOLDERCMP },
	{ ID_MERGE_COMPARE_LEFT1_RIGHT2,	IDB_MERGE_COMPARE_LEFT1_RIGHT2,	CMainFrame::MENU_FOLDERCMP },
	{ ID_MERGE_COMPARE_LEFT2_RIGHT1,	IDB_MERGE_COMPARE_LEFT2_RIGHT1,	CMainFrame::MENU_FOLDERCMP },
	{ ID_MERGE_DELETE,				IDB_MERGE_DELETE,				CMainFrame::MENU_FOLDERCMP },
	{ ID_TOOLS_GENERATEREPORT,		IDB_TOOLS_GENERATEREPORT,		CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_LEFT_TO_RIGHT,	IDB_LEFT_TO_RIGHT,				CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_RIGHT_TO_LEFT,	IDB_RIGHT_TO_LEFT,				CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_LEFT_TO_BROWSE,	IDB_LEFT_TO_BROWSE,				CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_RIGHT_TO_BROWSE,	IDB_RIGHT_TO_BROWSE,			CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_MOVE_LEFT_TO_BROWSE,	IDB_MOVE_LEFT_TO_BROWSE,		CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_MOVE_RIGHT_TO_BROWSE,	IDB_MOVE_RIGHT_TO_BROWSE,		CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_DEL_LEFT,				IDB_LEFT,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_DEL_RIGHT,				IDB_RIGHT,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_DEL_BOTH,				IDB_BOTH,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_PATHNAMES_LEFT,	IDB_LEFT,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_PATHNAMES_RIGHT,	IDB_RIGHT,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_PATHNAMES_BOTH,	IDB_BOTH,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_LEFT_TO_CLIPBOARD, IDB_LEFT,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_RIGHT_TO_CLIPBOARD, IDB_RIGHT,					CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_COPY_BOTH_TO_CLIPBOARD, IDB_BOTH,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_ZIP_LEFT,				IDB_LEFT,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_ZIP_RIGHT,				IDB_RIGHT,						CMainFrame::MENU_FOLDERCMP },
	{ ID_DIR_ZIP_BOTH,				IDB_BOTH,						CMainFrame::MENU_FOLDERCMP }
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_MENUCHAR()
	ON_WM_MEASUREITEM()
	ON_WM_INITMENUPOPUP()
	ON_WM_INITMENU()
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
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTFONT, OnUpdateViewSelectfont)
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
	ON_COMMAND_RANGE(ID_PREDIFFER_MANUAL, ID_PREDIFFER_AUTO, OnPluginPrediffMode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PREDIFFER_MANUAL, ID_PREDIFFER_AUTO, OnUpdatePluginPrediffMode)
	ON_UPDATE_COMMAND_UI(ID_RELOAD_PLUGINS, OnUpdateReloadPlugins)
	ON_COMMAND(ID_RELOAD_PLUGINS, OnReloadPlugins)
	ON_COMMAND(ID_HELP_GETCONFIG, OnSaveConfigData)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_NEW3, OnFileNew3)
	ON_COMMAND(ID_TOOLS_FILTERS, OnToolsFilters)
	ON_COMMAND(ID_HELP_MERGE7ZMISMATCH, OnHelpMerge7zmismatch)
	ON_UPDATE_COMMAND_UI(ID_HELP_MERGE7ZMISMATCH, OnUpdateHelpMerge7zmismatch)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_BAR, OnUpdateViewTabBar)
	ON_COMMAND(ID_VIEW_TAB_BAR, OnViewTabBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZE_PANES, OnUpdateResizePanes)
	ON_COMMAND(ID_VIEW_RESIZE_PANES, OnResizePanes)
	ON_COMMAND(ID_FILE_OPENPROJECT, OnFileOpenproject)
	ON_MESSAGE(WM_COPYDATA, OnCopyData)
	ON_MESSAGE(WM_USER+1, OnUser1)
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseAll)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSEALL, OnUpdateWindowCloseAll)
	ON_COMMAND(ID_FILE_SAVEPROJECT, OnSaveProject)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_COMMAND(ID_TOOLBAR_NONE, OnToolbarNone)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_NONE, OnUpdateToolbarNone)
	ON_COMMAND(ID_TOOLBAR_SMALL, OnToolbarSmall)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_SMALL, OnUpdateToolbarSmall)
	ON_COMMAND(ID_TOOLBAR_BIG, OnToolbarBig)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BIG, OnUpdateToolbarBig)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_COMMAND(ID_HELP_RELEASENOTES, OnHelpReleasenotes)
	ON_COMMAND(ID_HELP_TRANSLATIONS, OnHelpTranslations)
	ON_COMMAND(ID_FILE_OPENCONFLICT, OnFileOpenConflict)
	ON_COMMAND(ID_PLUGINS_LIST, OnPluginsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 * @brief MainFrame statusbar panels/indicators
 */
static UINT StatusbarIndicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // Merge mode
	ID_SEPARATOR,           // Diff number
	ID_INDICATOR_CAPS,      // Caps Lock
	ID_INDICATOR_NUM,       // Num Lock
	ID_INDICATOR_OVR,       // Insert
};

/** @brief Timer ID for window flashing timer. */
static const UINT ID_TIMER_FLASH = 1;

/** @brief Timeout for window flashing timer, in milliseconds. */
static const UINT WINDOW_FLASH_TIMEOUT = 500;

/** @brief Backup file extension. */
static const TCHAR BACKUP_FILE_EXT[] = _T("bak");

/**
  * @brief Return a const reference to a CMultiDocTemplate's list of documents.
  */
static const CPtrList &GetDocList(const CMultiDocTemplate *pTemplate)
{
	struct Template : public CMultiDocTemplate
	{
	public:
		using CMultiDocTemplate::m_docList;
	};
	return static_cast<const struct Template *>(pTemplate)->m_docList;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

/**
 * @brief MainFrame constructor. Loads settings from registry.
 * @todo Preference for logging?
 */
CMainFrame::CMainFrame()
: m_bFlashing(FALSE)
, m_bFirstTime(TRUE)
, m_bEscShutdown(FALSE)
, m_bClearCaseTool(FALSE)
, m_bExitIfNoDiff(MergeCmdLineInfo::Disabled)
, m_bShowErrors(TRUE)
, m_CheckOutMulti(FALSE)
, m_bVCProjSync(FALSE)
, m_bVssSuppressPathCheck(FALSE)
, m_pLineFilters(new LineFiltersList())
, m_pSyntaxColors(new SyntaxColors())
{
	ZeroMemory(&m_pMenus[0], sizeof(m_pMenus));
	UpdateCodepageModule();

	InitializeSourceControlMembers();
	g_bUnpackerMode = theApp.GetProfileInt(_T("Settings"), _T("UnpackerMode"), PLUGIN_MANUAL);
	g_bPredifferMode = theApp.GetProfileInt(_T("Settings"), _T("PredifferMode"), PLUGIN_MANUAL);

	if (m_pSyntaxColors)
		Options::SyntaxColors::Load(m_pSyntaxColors.get());

	if (m_pLineFilters)
		m_pLineFilters->Initialize(GetOptionsMgr());

	// If there are no filters loaded, and there is filter string in previous
	// option string, import old filters to new place.
	if (m_pLineFilters->GetCount() == 0)
	{
		String oldFilter = theApp.GetProfileString(_T("Settings"), _T("RegExps"));
		if (!oldFilter.empty())
			m_pLineFilters->Import(oldFilter);
	}

	// Check if filter folder is set, and create it if not
	String pathMyFolders = GetOptionsMgr()->GetString(OPT_FILTER_USERPATH);
	if (pathMyFolders.empty())
	{
		// No filter path, set it to default and make sure it exists.
		String pathFilters = theApp.GetDefaultFilterUserPath(TRUE);
		GetOptionsMgr()->SaveOption(OPT_FILTER_USERPATH, pathFilters);
		theApp.m_globalFileFilter.SetFileFilterPath(pathFilters.c_str());
	}

	sd_Init(); // String diff init
	sd_SetBreakChars(GetOptionsMgr()->GetString(OPT_BREAK_SEPARATORS).c_str());
}

CMainFrame::~CMainFrame()
{
	sd_Close();
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

#ifdef _UNICODE
const TCHAR CMainFrame::szClassName[] = _T("WinMergeWindowClassW");
#else
const TCHAR CMainFrame::szClassName[] = _T("WinMergeWindowClassA");
#endif
/**
 * @brief Change MainFrame window class name
 *        see http://support.microsoft.com/kb/403825/ja
 */
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	WNDCLASS wndcls;
	BOOL bRes = CMDIFrameWnd::PreCreateWindow(cs);
	HINSTANCE hInst = AfxGetInstanceHandle();
	// see if the class already exists
	if (!::GetClassInfo(hInst, szClassName, &wndcls))
	{
		// get default stuff
		::GetClassInfo(hInst, cs.lpszClass, &wndcls);
		// register a new class
		wndcls.lpszClassName = szClassName;
		wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
		::RegisterClass(&wndcls);
	}
	cs.lpszClass = szClassName;
	return bRes;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lfDiff = Options::Font::Load(OPT_FONT_FILECMP);
	m_lfDir = Options::Font::Load(OPT_FONT_DIRCMP);
	
	if (!CreateToobar())
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndTabBar.Create(this))
	{
		TRACE0("Failed to create tab bar\n");
		return -1;      // fail to create
	}
	if (GetOptionsMgr()->GetBool(OPT_SHOW_TABBAR) == false)
		CMDIFrameWnd::ShowControlBar(&m_wndTabBar, false, 0);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	theApp.SetIndicators(m_wndStatusBar, StatusbarIndicators,
			countof(StatusbarIndicators));

	m_wndStatusBar.SetPaneInfo(1, ID_STATUS_MERGINGMODE, 0, 100); 
	m_wndStatusBar.SetPaneInfo(2, ID_STATUS_DIFFNUM, 0, 150); 
	if (GetOptionsMgr()->GetBool(OPT_SHOW_STATUSBAR) == false)
		CMDIFrameWnd::ShowControlBar(&m_wndStatusBar, false, 0);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	// Start handling status messages from CustomStatusCursors
	myStatusDisplay.SetFrame(this);
	CustomStatusCursor::SetStatusDisplay(&myStatusDisplay);

	return 0;
}

/** 
 * @brief Find the scripts submenu from the main menu
 * As now this is the first submenu in "Edit" menu
 * We find the "Edit" menu by looking for a menu 
 *  starting with ID_EDIT_UNDO.
 */
HMENU CMainFrame::GetScriptsSubmenu(HMENU mainMenu)
{
	// look for "Plugin" menu
	int i;
	for (i = 0 ; i < ::GetMenuItemCount(mainMenu) ; i++)
		if (::GetMenuItemID(::GetSubMenu(mainMenu, i), 0) == ID_PLUGINS_LIST)
			break;
	HMENU pluginMenu = ::GetSubMenu(mainMenu, i);

	// look for "script" submenu (last submenu)
	for (i = ::GetMenuItemCount(pluginMenu) ; i >= 0  ; i--)
		if (::GetSubMenu(pluginMenu, i) != NULL)
			return ::GetSubMenu(pluginMenu, i);

	// error, submenu not found
	return NULL;
}

/**
 * @brief Find the scripts submenu from the main menu
 * As now this is the first submenu in "Plugins" menu
 * We find the "Plugins" menu by looking for a menu 
 *  starting with ID_UNPACK_MANUAL.
 */
HMENU CMainFrame::GetPrediffersSubmenu(HMENU mainMenu)
{
	// look for "Plugins" menu
	int i;
	for (i = 0 ; i < ::GetMenuItemCount(mainMenu) ; i++)
		if (::GetMenuItemID(::GetSubMenu(mainMenu, i), 0) == ID_PLUGINS_LIST)
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
 * @brief Create a new menu for the view..
 * @param [in] view Menu view either MENU_DEFAULT, MENU_MERGEVIEW or MENU_DIRVIEW.
 * @param [in] ID Menu's resource ID.
 * @return Menu for the view.
 */
HMENU CMainFrame::NewMenu(int view, int ID)
{
	int menu_view, index;
	if (m_pMenus[view] == NULL)
	{
		m_pMenus[view].reset(new BCMenu());
		if (m_pMenus[view] == NULL)
			return NULL;
	}

	switch (view)
	{
	case MENU_MERGEVIEW:
		menu_view = MENU_FILECMP;
		break;
	case MENU_DIRVIEW:
		menu_view = MENU_FOLDERCMP;
		break;
	case MENU_DEFAULT:
	default:
		menu_view = MENU_MAINFRM;
		break;
	};

	if (!m_pMenus[view]->LoadMenu(ID))
	{
		ASSERT(FALSE);
		return NULL;
	}

	// Load bitmaps to menuitems
	for (index = 0; index < countof(m_MenuIcons); index ++)
	{
		if (menu_view == (m_MenuIcons[index].menusToApply & menu_view))
		{
			m_pMenus[view]->ModifyODMenu(NULL, m_MenuIcons[index].menuitemID, m_MenuIcons[index].iconResID);
		}
	}

	m_pMenus[view]->LoadToolbar(IDR_MAINFRAME);

	theApp.TranslateMenu(m_pMenus[view]->m_hMenu);

	return (m_pMenus[view]->Detach());

}
/** 
* @brief Create new default (CMainFrame) menu.
*/
HMENU CMainFrame::NewDefaultMenu(int ID /*=0*/)
{
	if (ID == 0)
		ID = IDR_MAINFRAME;
	return NewMenu( MENU_DEFAULT, ID );
}

/**
 * @brief Create new File compare (CMergeEditView) menu.
 */
HMENU CMainFrame::NewMergeViewMenu()
{
	return NewMenu( MENU_MERGEVIEW, IDR_MERGEDOCTYPE);
}

/**
 * @brief Create new Dir compare (CDirView) menu
 */
HMENU CMainFrame::NewDirViewMenu()
{
	return NewMenu(MENU_DIRVIEW, IDR_DIRDOCTYPE );
}

/**
 * @brief Create new File compare (CHexMergeView) menu.
 */
HMENU CMainFrame::NewHexMergeViewMenu()
{
	return NewMenu( MENU_HEXMERGEVIEW, IDR_MERGEDOCTYPE);
}

/**
 * @brief Create new File compare (COpenView) menu.
 */
HMENU CMainFrame::NewOpenViewMenu()
{
	return NewMenu( MENU_OPENVIEW, IDR_MAINFRAME);
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

			if (m_pMenus[MENU_DEFAULT]->IsMenu(cmenu))
			{
				m_pMenus[MENU_DEFAULT]->MeasureItem(lpMeasureItemStruct);
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
	if(m_pMenus[MENU_DEFAULT]->IsMenu(pMenu))
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
		{
			BCMenu::UpdateMenu(pPopupMenu);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnFileOpen() 
{
	DoFileOpen();
}

/**
 * @brief Check for BOM, and also, if bGuessEncoding, try to deduce codepage
 *
 * Unpacks info from FileLocation & delegates all work to codepage_detect module
 */
static void
FileLocationGuessEncodings(FileLocation & fileloc, int iGuessEncoding)
{
	fileloc.encoding = GuessCodepageEncoding(fileloc.filepath, iGuessEncoding);
}

/**
 * @brief Creates new MergeDoc instance and shows documents.
 * @param [in] pDirDoc Dir compare document to create a new Merge document for.
 * @param [in] ifilelocLeft Left side file location info.
 * @param [in] ifilelocRight Right side file location info.
 * @param [in] dwLeftFlags Left side flags.
 * @param [in] dwRightFlags Right side flags.
 * @param [in] infoUnpacker Plugin info.
 * @return OPENRESULTS_TYPE for success/failure code.
 */
int CMainFrame::ShowMergeDoc(CDirDoc * pDirDoc,
	int nFiles, const FileLocation ifileloc[],
	DWORD dwFlags[] /*=0*/, PackingInfo * infoUnpacker /*= NULL*/)
{
	BOOL docNull;
	CMergeDoc * pMergeDoc = GetMergeDocToShow(nFiles, pDirDoc, &docNull);

	// Make local copies, so we can change encoding if we guess it below
	FileLocation fileloc[3];
	bool bRO[3];
	int pane;
	for (pane = 0; pane < nFiles; pane++)
	{
		fileloc[pane] = ifileloc[pane];
		if (dwFlags)
			bRO[pane] = (dwFlags[pane] & FFILEOPEN_READONLY) > 0;
		else
			bRO[pane] = FALSE;
	}

	ASSERT(pMergeDoc);		// must ASSERT to get an answer to the question below ;-)
	if (!pMergeDoc)
		return OPENRESULTS_FAILED_MISC; // when does this happen ?

	// if an unpacker is selected, it must be used during LoadFromFile
	// MergeDoc must memorize it for SaveToFile
	// Warning : this unpacker may differ from the pDirDoc one
	// (through menu : "Plugins"->"Open with unpacker")
	pMergeDoc->SetUnpacker(infoUnpacker);

	// detect codepage
	int iGuessEncodingType = GetOptionsMgr()->GetInt(OPT_CP_DETECT);
	for (pane = 0; pane < nFiles; pane++)
	{
		if (fileloc[pane].encoding.m_unicoding == -1)
			fileloc[pane].encoding.m_unicoding = ucr::NONE;
		if (fileloc[pane].encoding.m_unicoding == ucr::NONE && fileloc[pane].encoding.m_codepage == -1)
		{
			FileLocationGuessEncodings(fileloc[pane], iGuessEncodingType);
		}

		// TODO (Perry, 2005-12-04)
		// Should we do any unification if unicodings are different?


#ifndef _UNICODE
		// In ANSI (8-bit) build, character loss can occur in merging
		// if the two buffers use different encodings
		if (pane > 0 && fileloc[pane - 1].encoding.m_codepage != fileloc[pane].encoding.m_codepage)
		{
			CString msg;
			msg.Format(theApp.LoadString(IDS_SUGGEST_IGNORECODEPAGE).c_str(), fileloc[pane - 1].encoding.m_codepage,fileloc[pane].encoding.m_codepage);
			int msgflags = MB_YESNO | MB_ICONQUESTION | MB_DONT_ASK_AGAIN;
			// Two files with different codepages
			// Warn and propose to use the default codepage for both
			int userChoice = AfxMessageBox(msg, msgflags);
			if (userChoice == IDYES)
			{
				fileloc[pane - 1].encoding.SetCodepage(ucr::getDefaultCodepage());
				fileloc[pane - 1].encoding.m_bom = false;
				fileloc[pane - 1].encoding.m_guessed = false;
				fileloc[pane].encoding.SetCodepage(ucr::getDefaultCodepage());
				fileloc[pane].encoding.m_bom = false;
				fileloc[pane].encoding.m_guessed = false;
			}
		}
#endif
	}

	// Note that OpenDocs() takes care of closing compare window when needed.
	OPENRESULTS_TYPE openResults = pMergeDoc->OpenDocs(fileloc, bRO);

	if (openResults == OPENRESULTS_SUCCESS)
	{
		for (pane = 0; pane < nFiles; pane++)
		{
			BOOL bModified = dwFlags && (dwFlags[pane] & FFILEOPEN_MODIFIED) > 0;
			if (bModified)
			{
				pMergeDoc->m_ptBuf[pane]->SetModified(TRUE);
				pMergeDoc->UpdateHeaderPath(pane);
			}
		}
		if (docNull)
		{
			CWnd* pWnd = pMergeDoc->GetParentFrame();
			MDIActivate(pWnd);
		}
		else
			MDINext();
	}
	return openResults;
}

void CMainFrame::ShowHexMergeDoc(CDirDoc * pDirDoc, 
	const PathContext &paths, bool bRO[])
{
	BOOL docNull;
	if (CHexMergeDoc *pHexMergeDoc = GetHexMergeDocToShow(paths.GetSize(), pDirDoc, &docNull))
		pHexMergeDoc->OpenDocs(paths, bRO);
}

void CMainFrame::RedisplayAllDirDocs()
{
	const DirDocList &dirdocs = GetAllDirDocs();
	POSITION pos = dirdocs.GetHeadPosition();
	while (pos)
	{
		CDirDoc * pDirDoc = dirdocs.GetNext(pos);
		pDirDoc->Redisplay();
	}
}

/**
 * @brief Show/Hide different files/directories
 */
void CMainFrame::OnOptionsShowDifferent() 
{
	bool val = GetOptionsMgr()->GetBool(OPT_SHOW_DIFFERENT);
	GetOptionsMgr()->SaveOption(OPT_SHOW_DIFFERENT, !val); // reverse
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide identical files/directories
 */
void CMainFrame::OnOptionsShowIdentical() 
{
	bool val = GetOptionsMgr()->GetBool(OPT_SHOW_IDENTICAL);
	GetOptionsMgr()->SaveOption(OPT_SHOW_IDENTICAL, !val); // reverse
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide left-only files/directories
 */
void CMainFrame::OnOptionsShowUniqueLeft() 
{
	bool val = GetOptionsMgr()->GetBool(OPT_SHOW_UNIQUE_LEFT);
	GetOptionsMgr()->SaveOption(OPT_SHOW_UNIQUE_LEFT, !val); // reverse
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide right-only files/directories
 */
void CMainFrame::OnOptionsShowUniqueRight() 
{
	bool val = GetOptionsMgr()->GetBool(OPT_SHOW_UNIQUE_RIGHT);
	GetOptionsMgr()->SaveOption(OPT_SHOW_UNIQUE_RIGHT, !val); // reverse
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide binary files
 */
void CMainFrame::OnOptionsShowBinaries()
{
	bool val = GetOptionsMgr()->GetBool(OPT_SHOW_BINARIES);
	GetOptionsMgr()->SaveOption(OPT_SHOW_BINARIES, !val); // reverse
	RedisplayAllDirDocs();
}

/**
 * @brief Show/Hide skipped files/directories
 */
void CMainFrame::OnOptionsShowSkipped()
{
	bool val = GetOptionsMgr()->GetBool(OPT_SHOW_SKIPPED);
	GetOptionsMgr()->SaveOption(OPT_SHOW_SKIPPED, !val); // reverse
	RedisplayAllDirDocs();
}

void CMainFrame::OnUpdateOptionsShowdifferent(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_SHOW_DIFFERENT));
}

void CMainFrame::OnUpdateOptionsShowidentical(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_SHOW_IDENTICAL));
}

void CMainFrame::OnUpdateOptionsShowuniqueleft(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_SHOW_UNIQUE_LEFT));
}

void CMainFrame::OnUpdateOptionsShowuniqueright(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_SHOW_UNIQUE_RIGHT));
}

void CMainFrame::OnUpdateOptionsShowBinaries(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_SHOW_BINARIES));
}

void CMainFrame::OnUpdateOptionsShowSkipped(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_SHOW_SKIPPED));
}

/**
 * @brief Show GNU licence information in notepad (local file) or in Web Browser
 */
void CMainFrame::OnHelpGnulicense() 
{
	const String spath = paths_ConcatPath(env_GetProgPath(), LicenseFile);
	OpenFileOrUrl(spath.c_str(), LicenceUrl);
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
int CMainFrame::HandleReadonlySave(String& strSavePath, BOOL bMultiFile,
		BOOL &bApplyToAll)
{
	CFileStatus status;
	UINT userChoice = 0;
	int nRetVal = IDOK;
	BOOL bFileRO = FALSE;
	BOOL bFileExists = FALSE;
	String s;
	String str;
	CString title;
	int nVerSys = 0;

	try
	{
		TFile file(strSavePath);
		bFileExists = file.exists();
		if (bFileExists)
			bFileRO = !file.canWrite();
	}
	catch (...)
	{
	}
	nVerSys = GetOptionsMgr()->GetInt(OPT_VCS_SYSTEM);
	
	if (bFileExists && bFileRO)
	{
		// Version control system used?
		// Checkout file from VCS and modify, don't ask about overwriting
		// RO files etc.
		if (nVerSys != VCS_NONE)
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
				str = string_format_string1(_("%1\nis marked read-only. Would you like to override the read-only item?"), strSavePath);
				userChoice = AfxMessageBox(str.c_str(), MB_YESNOCANCEL |
						MB_ICONWARNING | MB_DEFBUTTON3 | MB_DONT_ASK_AGAIN |
						MB_YES_TO_ALL, IDS_SAVEREADONLY_MULTI);
			}
			else
			{
				// Single file
				str = string_format_string1(_("%1 is marked read-only. Would you like to override the read-only file ? (No to save as new filename.)"), strSavePath);
				userChoice = AfxMessageBox(str.c_str(), MB_YESNOCANCEL |
						MB_ICONWARNING | MB_DEFBUTTON2 | MB_DONT_ASK_AGAIN,
						IDS_SAVEREADONLY_FMT);
			}
		}
		switch (userChoice)
		{
		// Overwrite read-only file
		case IDYESTOALL:
			bApplyToAll = TRUE;  // Don't ask again (no break here)
		case IDYES:
			CFile::GetStatus(strSavePath.c_str(), status);
			status.m_mtime = 0;		// Avoid unwanted changes
			status.m_attribute &= ~CFile::readOnly;
			CFile::SetStatus(strSavePath.c_str(), status);
			nRetVal = IDYES;
			break;
		
		// Save to new filename (single) /skip this item (multiple)
		case IDNO:
			if (!bMultiFile)
			{
				if (SelectFile(GetSafeHwnd(), s, strSavePath.c_str(), _("Save As"), _T(""), FALSE))
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

/// Wrapper to set the global option 'm_bAllowMixedEol'
void CMainFrame::SetEOLMixed(BOOL bAllow)
{
	GetOptionsMgr()->SaveOption(OPT_ALLOW_MIXED_EOL, bAllow == TRUE);
	ApplyViewWhitespace();
}

/**
 * @brief Opens Options-dialog and saves changed options
 */
void CMainFrame::OnOptions() 
{
	// Using singleton shared syntax colors
	CPreferencesDlg dlg(GetOptionsMgr(), m_pSyntaxColors.get());
	int rv = dlg.DoModal();

	if (rv == IDOK)
	{
		// Set new filterpath
		String filterPath = GetOptionsMgr()->GetString(OPT_FILTER_USERPATH);
		theApp.m_globalFileFilter.SetUserFilterPath(filterPath);

		UpdateCodepageModule();
		// Call the wrapper to set m_bAllowMixedEol (the wrapper updates the registry)
		SetEOLMixed(GetOptionsMgr()->GetBool(OPT_ALLOW_MIXED_EOL));

		sd_SetBreakChars(GetOptionsMgr()->GetString(OPT_BREAK_SEPARATORS).c_str());

		// make an attempt at rescanning any open diff sessions
		const MergeDocList &docs = GetAllMergeDocs();
		POSITION pos = docs.GetHeadPosition();
		while (pos)
		{
			CMergeDoc * pMergeDoc = docs.GetNext(pos);

			// Re-read MergeDoc settings (also updates view settings)
			// and rescan using new options
			pMergeDoc->RefreshOptions();
			pMergeDoc->FlushAndRescan(TRUE);
		}

		// Update all dirdoc settings
		const DirDocList &dirDocs = GetAllDirDocs();
		pos = dirDocs.GetHeadPosition();
		while (pos)
		{
			CDirDoc * pDirDoc = dirDocs.GetNext(pos);
			pDirDoc->RefreshOptions();
		}
	}
}

static bool AddToRecentDocs(const PathContext& paths, const unsigned flags[], bool recurse, const String& filter)
{
	String params, title;
	for (int nIndex = 0; nIndex < paths.GetSize(); ++nIndex)
	{
		if (flags[nIndex] & FFILEOPEN_READONLY)
		{
			switch (nIndex)
			{
			case 0: params += _T("/wl "); break;
			case 1: params += paths.GetSize() == 2 ? _T("/wr ") : _T("/wm "); break;
			case 2:	params += _T("/wr "); break;
			}
		}
		params += _T("\"") + paths[nIndex] + _T("\" ");

		String path = paths[nIndex];
		paths_normalize(path);
		title += paths_FindFileName(path);
		if (nIndex < paths.GetSize() - 1)
			title += _T(" - ");
	}
	if (recurse)
		params += _T("/r ");
	if (!filter.empty())
		params += _T("/f \"") + filter + _T("\" ");
	return JumpList::AddToRecentDocs(_T(""), params, title, params, 0);
}

/**
 * @brief Begin a diff: open dirdoc if it is directories, else open a mergedoc for editing.
 * @param [in] pszLeft Left-side path.
 * @param [in] pszRight Right-side path.
 * @param [in] dwLeftFlags Left-side flags.
 * @param [in] dwRightFlags Right-side flags.
 * @param [in] bRecurse Do we run recursive (folder) compare?
 * @param [in] pDirDoc Dir compare document to use.
 * @param [in] prediffer Prediffer plugin name.
 * @return TRUE if opening files and compare succeeded, FALSE otherwise.
 */
BOOL CMainFrame::DoFileOpen(PathContext * pFiles /*=NULL*/,
	DWORD dwFlags[] /*=0*/, bool bRecurse /*=FALSE*/, CDirDoc *pDirDoc/*=NULL*/,
	String prediffer /*=_T("")*/, PackingInfo *infoUnpacker/*=NULL*/)
{
	if (pDirDoc && !pDirDoc->CloseMergeDocs())
		return FALSE;

	g_bUnpackerMode = theApp.GetProfileInt(_T("Settings"), _T("UnpackerMode"), PLUGIN_MANUAL);
	g_bPredifferMode = theApp.GetProfileInt(_T("Settings"), _T("PredifferMode"), PLUGIN_MANUAL);

	PathContext files;
	if (pFiles)
		files = *pFiles;
	bool bRO[3] = {0};
	if (dwFlags)
	{
		bRO[0] = (dwFlags[0] & FFILEOPEN_READONLY) != 0;
		bRO[1] = (dwFlags[1] & FFILEOPEN_READONLY) != 0;
		bRO[2] = (dwFlags[2] & FFILEOPEN_READONLY) != 0;
	};
	// jtuc: docNull used to be uninitialized so you couldn't tell whether
	// pDirDoc->ReusingDirDoc() would be called for passed-in pDirDoc.
	// However, pDirDoc->ReusingDirDoc() kills temp path contexts, and I
	// need to avoid that. This is why I'm initializing docNull to TRUE here.
	// Note that call to pDirDoc->CloseMergeDocs() above preserves me from
	// keeping orphaned MergeDocs in that case.
	BOOL docNull = TRUE;

	// pop up dialog unless arguments exist (and are compatible)
	PATH_EXISTENCE pathsType = GetPairComparability(files, IsArchiveFile);
	if (pathsType == DOES_NOT_EXIST)
	{
		COpenDoc *pOpenDoc = (COpenDoc *)theApp.m_pOpenTemplate->CreateNewDocument();
		if (dwFlags)
		{
			pOpenDoc->m_dwFlags[0] = dwFlags[0];
			pOpenDoc->m_dwFlags[1] = dwFlags[1];
			pOpenDoc->m_dwFlags[2] = dwFlags[2];
		}
		pOpenDoc->m_files = files;
		if (infoUnpacker)
			pOpenDoc->m_infoHandler = *infoUnpacker;
		CFrameWnd *pFrame = theApp.m_pOpenTemplate->CreateNewFrame(pOpenDoc, NULL);
		theApp.m_pOpenTemplate->InitialUpdateFrame(pFrame, pOpenDoc);
		return TRUE;
	}
	else
	{
		// Add trailing '\' for directories if its missing
		if (pathsType == IS_EXISTING_DIR)
		{
			if (!paths_EndsWithSlash(files[0]) && !IsArchiveFile(files[0]))
				files[0] = paths_AddTrailingSlash(files[0]);
			if (!paths_EndsWithSlash(files[1]) && !IsArchiveFile(files[1]))
				files[1] = paths_AddTrailingSlash(files[1]);
			if (files.GetSize() == 3 && !paths_EndsWithSlash(files[2]) && !IsArchiveFile(files[1]))
				files[2] = paths_AddTrailingSlash(files[2]);
		}

		//save the MRU left and right files.
		if (dwFlags)
		{
			if (!(dwFlags[0] & FFILEOPEN_NOMRU))
				addToMru(files[0].c_str(), _T("Files\\Left"));
			if (!(dwFlags[1] & FFILEOPEN_NOMRU))
				addToMru(files[1].c_str(), _T("Files\\Right"));
			if (files.GetSize() == 3 && !(dwFlags[2] & FFILEOPEN_NOMRU))
				addToMru(files[2].c_str(), _T("Files\\Option"));
		}
	}

	CTempPathContext *pTempPathContext = NULL;
	try
	{
		String path;
		USES_CONVERSION;
		// Handle archives using 7-zip
		if (Merge7z::Format *piHandler = ArchiveGuessFormat(files[0].c_str()))
		{
			pTempPathContext = new CTempPathContext;
			path = env_GetTempChildPath();
			for (int index = 0; index < files.GetSize(); index++)
				pTempPathContext->m_strDisplayRoot[index] = files[index];
			pathsType = IS_EXISTING_DIR;
			if (files[0] == files[1])
			{
				files[1].erase();
				if (files.GetSize() > 2)
				{
					files[2].erase();
				}
			}
			do
			{
				if (FAILED(piHandler->DeCompressArchive(m_hWnd, files[0].c_str(), path.c_str())))
					break;
				if (files[0].find(path) == 0)
				{
					VERIFY(::DeleteFile(files[0].c_str()) || (LogErrorString(string_format(_T("DeleteFile(%s) failed"), files[0].c_str())), false));
				}
				BSTR pTmp = piHandler->GetDefaultName(m_hWnd, files[0].c_str());
				files[0] = paths_ConcatPath(path, OLE2T(pTmp));
				SysFreeString(pTmp);
			} while (piHandler = ArchiveGuessFormat(files[0].c_str()));
			files[0] = path;
		}
		if (Merge7z::Format *piHandler = ArchiveGuessFormat(files[1].c_str()))
		{
			if (!pTempPathContext)
			{
				pTempPathContext = new CTempPathContext;
				for (int index = 0; index < files.GetSize(); index++)
					pTempPathContext->m_strDisplayRoot[index] = files[index];
			}
			path = env_GetTempChildPath();
			do
			{
				if (FAILED(piHandler->DeCompressArchive(m_hWnd, files[1].c_str(), path.c_str())))
					break;;
				if (files[1].find(path) == 0)
				{
					VERIFY(::DeleteFile(files[1].c_str()) || (LogErrorString(string_format(_T("DeleteFile(%s) failed"), files[1].c_str())), false));
				}
				BSTR pTmp = piHandler->GetDefaultName(m_hWnd, files[1].c_str());
				files[1] = paths_ConcatPath(path, OLE2T(pTmp));
				SysFreeString(pTmp);
			} while (piHandler = ArchiveGuessFormat(files[1].c_str()));
			files[1] = path;
		}
		if (files.GetSize() > 2)
		{
			if (Merge7z::Format *piHandler = ArchiveGuessFormat(files[2].c_str()))
			{
				if (!pTempPathContext)
				{
					pTempPathContext = new CTempPathContext;
					for (int index = 0; index < files.GetSize(); index++)
						pTempPathContext->m_strDisplayRoot[index] = files[index];
				}
				path = env_GetTempChildPath();
				do
				{
					if (FAILED(piHandler->DeCompressArchive(m_hWnd, files[2].c_str(), path.c_str())))
						break;;
					if (files[2].find(path) == 0)
					{
						VERIFY(::DeleteFile(files[2].c_str()) || (LogErrorString(string_format(_T("DeleteFile(%s) failed"), files[2].c_str())), false));
					}
					BSTR pTmp = piHandler->GetDefaultName(m_hWnd, files[1].c_str());
					files[2] = paths_ConcatPath(path, OLE2T(pTmp));
					SysFreeString(pTmp);
				} while (piHandler = ArchiveGuessFormat(files[2].c_str()));
				files[2] = path;
			}
		}
		if (files[1].empty())
		{
			// assume Perry style patch
			files[0] = paths_ConcatPath(files[0], _T("ORIGINAL"));
			files[1] = paths_ConcatPath(path,     _T("ALTERED"));
			if (paths_DoesPathExist(files[0]) == DOES_NOT_EXIST || paths_DoesPathExist(files[1]) == DOES_NOT_EXIST)
			{
				// not a Perry style patch: diff with itself...
				files[0] = files[1] = path;
				if (files.GetSize() > 2)
					files[2] = path;
			}
			else
			{
				pTempPathContext->m_strDisplayRoot[0] = paths_ConcatPath(pTempPathContext->m_strDisplayRoot[0], _T("ORIGINAL"));
				pTempPathContext->m_strDisplayRoot[1] = paths_ConcatPath(pTempPathContext->m_strDisplayRoot[1], _T("ALTERED"));
			}
		}
	}
	catch (CException *e)
	{
		e->ReportError(MB_ICONSTOP);
		e->Delete();
	}

	// Determine if we want new a dirview open now that we know if it was
	// and archive. Don't open new dirview if we are comparing files.
	if (!pDirDoc)
	{
		if (pathsType == IS_EXISTING_DIR)
		{
			pDirDoc = GetDirDocToShow(files.GetSize(), &docNull);
		}
		else
		{
			pDirDoc = (CDirDoc*)theApp.m_pDirTemplate->CreateNewDocument();
			docNull = TRUE;
		}
	}

	if (!docNull)
	{
		// If reusing an existing doc, give it a chance to save its data
		// and close any merge views, and clear its window
		if (!pDirDoc->ReusingDirDoc())
			return FALSE;
	}
	
	// open the diff
	if (pathsType == IS_EXISTING_DIR)
	{
		if (pDirDoc)
		{
			if (files.GetSize() == 3)
			{
				AfxMessageBox(_T("3�t�H���_��r�@�\�͎������ł�"), MB_ICONWARNING | MB_DONT_ASK_AGAIN);
			}
			// Anything that can go wrong inside InitCompare() will yield an
			// exception. There is no point in checking return value.
			pDirDoc->InitCompare(files, bRecurse, pTempPathContext);

			pDirDoc->SetDescriptions(m_strDescriptions);
			pDirDoc->SetTitle(NULL);
			for (int nIndex = 0; nIndex < files.GetSize(); nIndex++)
			{
				pDirDoc->SetReadOnly(nIndex, bRO[nIndex]);
				m_strDescriptions[nIndex].erase();
			}

			pDirDoc->Rescan();
		}
	}
	else
	{		
		FileLocation fileloc[3];

		for (int nPane = 0; nPane < files.GetSize(); nPane++)
			fileloc[nPane].setPath(files[nPane]);

		if (!prediffer.empty())
		{
			String strBothFilenames;
			for (int nIndex = 0; nIndex < files.GetSize(); nIndex++)
			{
				strBothFilenames += files[nIndex];
				strBothFilenames += _T("|");
			}
			strBothFilenames.resize(strBothFilenames.length() - 1);

			pDirDoc->SetPluginPrediffer(strBothFilenames, prediffer);
		}

		ShowMergeDoc(pDirDoc, files.GetSize(), fileloc, dwFlags,
			infoUnpacker);
	}

	if (pFiles && !(dwFlags[0] & FFILEOPEN_NOMRU))
	{
		String filter = GetOptionsMgr()->GetString(OPT_FILEFILTER_CURRENT);
		AddToRecentDocs(*pFiles, (unsigned *)dwFlags, bRecurse, filter);
	}

	return TRUE;
}

/**
 * @brief Creates backup before file is saved or copied over.
 * This function handles formatting correct path and filename for
 * backup file. Formatting is done based on several options available
 * for users in Options/Backups dialog. After path is formatted, file
 * is simply just copied. Not much error checking, just if copying
 * succeeded or failed.
 * @param [in] bFolder Are we creating backup in folder compare?
 * @param [in] pszPath Full path to file to backup.
 * @return TRUE if backup succeeds, or isn't just done.
 */
BOOL CMainFrame::CreateBackup(BOOL bFolder, const String& pszPath)
{
	// If user doesn't want to backups in folder compare, return
	// success so operations don't abort.
	if (bFolder && !(GetOptionsMgr()->GetBool(OPT_BACKUP_FOLDERCMP)))
		return TRUE;
	// Likewise if user doesn't want backups in file compare
	else if (!bFolder && !(GetOptionsMgr()->GetBool(OPT_BACKUP_FILECMP)))
		return TRUE;

	// create backup copy of file if destination file exists
	if (paths_DoesPathExist(pszPath) == IS_EXISTING_FILE)
	{
		String bakPath;
		String path;
		String filename;
		String ext;
	
		paths_SplitFilename(pszPath, &path, &filename, &ext);

		// Determine backup folder
		if (GetOptionsMgr()->GetInt(OPT_BACKUP_LOCATION) ==
			PropBackups::FOLDER_ORIGINAL)
		{
			// Put backups to same folder than original file
			bakPath = path;
		}
		else if (GetOptionsMgr()->GetInt(OPT_BACKUP_LOCATION) ==
			PropBackups::FOLDER_GLOBAL)
		{
			// Put backups to global folder defined in options
			bakPath = GetOptionsMgr()->GetString(OPT_BACKUP_GLOBALFOLDER);
			if (bakPath.empty())
				bakPath = path;
			else
				bakPath = paths_GetLongPath(bakPath);
		}
		else
		{
			_RPTF0(_CRT_ERROR, "Unknown backup location!");
		}

		BOOL success = FALSE;
		if (GetOptionsMgr()->GetBool(OPT_BACKUP_ADD_BAK))
		{
			// Don't add dot if there is no existing extension
			if (ext.size() > 0)
				ext += _T(".");
			ext += BACKUP_FILE_EXT;
		}

		// Append time to filename if wanted so
		// NOTE just adds timestamp at the moment as I couldn't figure out
		// nice way to add a real time (invalid chars etc).
		if (GetOptionsMgr()->GetBool(OPT_BACKUP_ADD_TIME))
		{
			struct tm *tm;
			time_t curtime = 0;
			time(&curtime);
			tm = localtime(&curtime);
			CString timestr;
			timestr.Format(_T("%04d%02d%02d%02d%02d%02d"), tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
			filename += _T("-");
			filename += timestr;
		}

		// Append filename and extension (+ optional .bak) to path
		if ((bakPath.length() + filename.length() + ext.length())
			< MAX_PATH)
		{
			success = TRUE;
			bakPath = paths_ConcatPath(bakPath, filename);
			bakPath += _T(".");
			bakPath += ext;
		}

		if (success)
			success = CopyFile(pszPath.c_str(), bakPath.c_str(), FALSE);
		
		if (!success)
		{
			String msg = string_format_string1(
				_("Unable to backup original file:\n%1\n\nContinue anyway?"),
				pszPath);
			if (AfxMessageBox(msg.c_str(), MB_YESNO | MB_ICONWARNING | MB_DONT_ASK_AGAIN) != IDYES)
				return FALSE;
		}
		return TRUE;
	}

	// we got here because we're either not backing up of there was nothing to backup
	return TRUE;
}

/**
 * @brief Sync file to Version Control System
 * @param pszDest [in] Where to copy (incl. filename)
 * @param bApplyToAll [in,out] Apply user selection to all items
 * @param psError [out] Error string that can be shown to user in caller func
 * @return User selection or -1 if error happened
 * @sa CMainFrame::HandleReadonlySave()
 * @sa CDirView::PerformActionList()
 */
int CMainFrame::SyncFileToVCS(const String& pszDest, BOOL &bApplyToAll,
	String& sError)
{
	String sActionError;
	String strSavePath(pszDest);
	int nVerSys = 0;

	nVerSys = GetOptionsMgr()->GetInt(OPT_VCS_SYSTEM);
	
	int nRetVal = HandleReadonlySave(strSavePath, TRUE, bApplyToAll);
	if (nRetVal == IDCANCEL || nRetVal == IDNO)
		return nRetVal;
	
	// If VC project opened from VSS sync and version control used
	if ((nVerSys == VCS_VSS4 || nVerSys == VCS_VSS5) && m_bVCProjSync)
	{
		if (!m_vssHelper.ReLinkVCProj(strSavePath, sError))
			nRetVal = -1;
	}
	return nRetVal;
}

void CMainFrame::UpdateFont(FRAMETYPE frame)
{
	if (frame == FRAME_FOLDER)
	{
		const DirDocList &dirdocs = GetAllDirDocs();
		POSITION pos = dirdocs.GetHeadPosition();
		while (pos)
		{
			CDirDoc * pDoc = dirdocs.GetNext(pos);
			if (pDoc)
				pDoc->GetMainView()->SetFont(m_lfDir);
		}
	}
	else
	{
		const MergeDocList &mergedocs = GetAllMergeDocs();
		POSITION pos = mergedocs.GetHeadPosition();
		while (pos)
		{
			CMergeDoc * pDoc = mergedocs.GetNext(pos);
			for (int pane = 0; pane < pDoc->m_nBuffers; pane++)
			{
				CMergeEditView * pView = pDoc->GetView(pane);
				CMergeEditView * pDetailView = pDoc->GetDetailView(pane);
				if (pView)
					pView->SetFont(m_lfDiff);
				if (pDetailView)
					pDetailView->SetFont(m_lfDiff);
			}
		}
	}
}

/**
 * @brief Select font for Merge/Dir view
 * 
 * Shows font selection dialog to user, sets current font and saves
 * selected font properties to registry. Selects fon type to active
 * view (Merge/Dir compare). If there is no open views, then font
 * is selected for Merge view (for example user may want to change to
 * unicode font before comparing files).
 */
void CMainFrame::OnViewSelectfont() 
{
	FRAMETYPE frame = GetFrameType(GetActiveFrame());
	CHOOSEFONT cf;
	LOGFONT *lf = NULL;
	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.Flags = CF_INITTOLOGFONTSTRUCT|CF_FORCEFONTEXIST|CF_SCREENFONTS;
	if (frame == FRAME_FILE)
		cf.Flags |= CF_FIXEDPITCHONLY; // Only fixed-width fonts for merge view

	// CF_FIXEDPITCHONLY = 0x00004000L
	// in case you are a developer and want to disable it to test with, eg, a Chinese capable font
	if (frame == FRAME_FOLDER)
		lf = &m_lfDir;
	else
		lf = &m_lfDiff;

	cf.lpLogFont = lf;

	if (ChooseFont(&cf))
	{
		Options::Font::Save(frame == FRAME_FOLDER ? OPT_FONT_DIRCMP : OPT_FONT_FILECMP, lf, true);
		UpdateFont(frame);
	}
}

/**
 * @brief Enable 'Select font'.
 */
void CMainFrame::OnUpdateViewSelectfont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/**
 * @brief Use default font for active view type
 *
 * Disable user-selected font for active view type (Merge/Dir compare).
 * If there is no open views, then Merge view font is changed.
 */
void CMainFrame::OnViewUsedefaultfont() 
{
	FRAMETYPE frame = GetFrameType(GetActiveFrame());

	if (frame == FRAME_FOLDER)
	{
		Options::Font::Reset(OPT_FONT_DIRCMP);
		m_lfDir = Options::Font::Load(OPT_FONT_DIRCMP);
		Options::Font::Save(OPT_FONT_DIRCMP, &m_lfDir, false);
	}
	else
	{
		Options::Font::Reset(OPT_FONT_FILECMP);
		m_lfDiff = Options::Font::Load(OPT_FONT_FILECMP);
		Options::Font::Save(OPT_FONT_FILECMP, &m_lfDiff, false);
	}

	UpdateFont(frame);
}

/**
 * @brief Enable 'Use Default font'.
 */
void CMainFrame::OnUpdateViewUsedefaultfont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/**
 * @brief Update any resources necessary after a GUI language change
 */
void CMainFrame::UpdateResources()
{
	m_wndStatusBar.SetPaneText(0, theApp.LoadString(AFX_IDS_IDLEMESSAGE).c_str());

	const DirDocList &dirdocs = GetAllDirDocs();
	POSITION pos = dirdocs.GetHeadPosition();
	while (pos)
	{
		CDirDoc * pDoc = dirdocs.GetNext(pos);
		pDoc->UpdateResources();
	}

	const MergeDocList &mergedocs = GetAllMergeDocs();
	pos = mergedocs.GetHeadPosition();
	while (pos)
	{
		CMergeDoc * pDoc = mergedocs.GetNext(pos);
		pDoc->UpdateResources();
	}
}

/**
 * @brief Open file, if it exists, else open url
 */
void CMainFrame::OpenFileOrUrl(LPCTSTR szFile, LPCTSTR szUrl)
{
	if (paths_DoesPathExist(szFile) == IS_EXISTING_FILE)
		ShellExecute(m_hWnd, _T("open"), _T("notepad.exe"), szFile, NULL, SW_SHOWNORMAL);
	else
		ShellExecute(NULL, _T("open"), szUrl, NULL, NULL, SW_SHOWNORMAL);
}

/**
 * @brief Open WinMerge help.
 *
 * If local HTMLhelp file is found, open it, otherwise open HTML page from web.
 */
void CMainFrame::OnHelpContents()
{
	String sPath = env_GetProgPath();
	LANGID LangId = theApp.GetLangId();
	if (PRIMARYLANGID(LangId) == LANG_JAPANESE)
		sPath = paths_ConcatPath(sPath, DocsPath_ja);
	else
		sPath = paths_ConcatPath(sPath, DocsPath);
	if (paths_DoesPathExist(sPath) == IS_EXISTING_FILE)
		::HtmlHelp(NULL, sPath.c_str(), HH_DISPLAY_TOC, NULL);
	else
		ShellExecute(NULL, _T("open"), DocsURL, NULL, NULL, SW_SHOWNORMAL);
}

/**
 * @brief Enable Open WinMerge help -menuitem.
 */
void CMainFrame::OnUpdateHelpContents(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/**
 * @brief Handle translation of default messages on the status bar
 */
void CMainFrame::GetMessageString(UINT nID, CString& rMessage) const
{
	// load appropriate string
	const String s = theApp.LoadString(nID);

	// avoid dereference of empty strings
	if (s.length() <= 0 || !AfxExtractSubString(rMessage, s.c_str(), 0))
	{
		// not found
		TRACE1("Warning: no message line prompt for ID 0x%04X.\n", nID);
	}
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
	if (nCmdShow != SW_MINIMIZE && theApp.GetProfileInt(_T("Settings"), _T("MainMax"), FALSE))
		wp.showCmd = SW_MAXIMIZE;
	else
		wp.showCmd = nCmdShow;

	CRect dsk_rc,rc(wp.rcNormalPosition);

	dsk_rc.left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	dsk_rc.top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	dsk_rc.right = dsk_rc.left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	dsk_rc.bottom = dsk_rc.top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
	if (rc.Width() != 0 && rc.Height() != 0)
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

/**
 * @brief Called when mainframe is about to be closed.
 * This function is called when mainframe is to be closed (not for
 * file/compare windows.
 */
void CMainFrame::OnClose()
{
	if (theApp.GetActiveOperations())
		return;

	// Check if there are multiple windows open and ask for closing them
	BOOL bAskClosing = GetOptionsMgr()->GetBool(OPT_ASK_MULTIWINDOW_CLOSE);
	if (bAskClosing)
	{
		bool quit = AskCloseConfirmation();
		if (!quit)
			return;
	}

	// Save last selected filter
	String filter = theApp.m_globalFileFilter.GetFilterNameOrMask();
	GetOptionsMgr()->SaveOption(OPT_FILEFILTER_CURRENT, filter);

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

/**
 * @brief Utility function to update CSuperComboBox format MRU
 */
void CMainFrame::addToMru(LPCTSTR szItem, LPCTSTR szRegSubKey, UINT nMaxItems)
{
	std::vector<CString> list;
	CString s,s2;
	UINT cnt = AfxGetApp()->GetProfileInt(szRegSubKey, _T("Count"), 0);
	list.push_back(szItem);
	for (UINT i=0 ; i<cnt; ++i)
	{
		s2.Format(_T("Item_%d"), i);
		s = AfxGetApp()->GetProfileString(szRegSubKey, s2);
		if (s != szItem)
			list.push_back(s);
	}
	cnt = list.size() > nMaxItems ? nMaxItems : list.size();
	for (UINT i=0 ; i<cnt; ++i)
	{
		s2.Format(_T("Item_%d"), i);
		AfxGetApp()->WriteProfileString(szRegSubKey, s2, list[i]);
	}
	// update count
	AfxGetApp()->WriteProfileInt(szRegSubKey, _T("Count"), cnt);
}

/**
 * @brief Apply tabs and eols settings to all merge documents
 */
void CMainFrame::ApplyViewWhitespace() 
{
	const MergeDocList &mergedocs = GetAllMergeDocs();
	POSITION pos = mergedocs.GetHeadPosition();
	while (pos)
	{
		CMergeDoc *pMergeDoc = mergedocs.GetNext(pos);
		for (int pane = 0; pane < pMergeDoc->m_nBuffers; pane++)
		{
			CMergeEditView * pView = pMergeDoc->GetView(pane);
			CMergeEditView * pDetailView = pMergeDoc->GetDetailView(pane);
			if (pView)
			{
				pView->SetViewTabs(GetOptionsMgr()->GetBool(OPT_VIEW_WHITESPACE));
				pView->SetViewEols(GetOptionsMgr()->GetBool(OPT_VIEW_WHITESPACE),
					GetOptionsMgr()->GetBool(OPT_ALLOW_MIXED_EOL) ||
					pView->GetDocument()->IsMixedEOL(pView->m_nThisPane));
			}
			if (pDetailView)
			{
				pDetailView->SetViewTabs(GetOptionsMgr()->GetBool(OPT_VIEW_WHITESPACE));
				pDetailView->SetViewEols(GetOptionsMgr()->GetBool(OPT_VIEW_WHITESPACE),
					GetOptionsMgr()->GetBool(OPT_ALLOW_MIXED_EOL) ||
					pDetailView->GetDocument()->IsMixedEOL(pDetailView->m_nThisPane));
			}
		}
	}
}

void CMainFrame::OnViewWhitespace() 
{
	bool bViewWhitespace = GetOptionsMgr()->GetBool(OPT_VIEW_WHITESPACE);
	GetOptionsMgr()->SaveOption(OPT_VIEW_WHITESPACE, !bViewWhitespace);
	ApplyViewWhitespace();
}

/// Enables View/View Whitespace menuitem when merge view is active
void CMainFrame::OnUpdateViewWhitespace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_VIEW_WHITESPACE));
}

/// Get list of OpenDocs (documents underlying edit sessions)
const OpenDocList &CMainFrame::GetAllOpenDocs()
{
	return static_cast<const OpenDocList &>(GetDocList(theApp.m_pOpenTemplate));
}

/// Get list of MergeDocs (documents underlying edit sessions)
const MergeDocList &CMainFrame::GetAllMergeDocs()
{
	return static_cast<const MergeDocList &>(GetDocList(theApp.m_pDiffTemplate));
}

/// Get list of DirDocs (documents underlying a scan)
const DirDocList &CMainFrame::GetAllDirDocs()
{
	return static_cast<const DirDocList &>(GetDocList(theApp.m_pDirTemplate));
}

/// Get list of HexMergeDocs (documents underlying edit sessions)
const HexMergeDocList &CMainFrame::GetAllHexMergeDocs()
{
	return static_cast<const HexMergeDocList &>(GetDocList(theApp.m_pHexMergeTemplate));
}

/**
 * @brief Obtain a merge doc to display a difference in files.
 * This function (usually) uses DirDoc to determine if new or existing
 * MergeDoc is used. However there is exceptional case when DirDoc does
 * not contain diffs. Then we have only file compare, and if we also have
 * limited file compare windows, we always reuse existing MergeDoc.
 * @param [in] pDirDoc Dir compare document.
 * @param [out] pNew Did we create a new document?
 * @return Pointer to merge doucument.
 */
CMergeDoc * CMainFrame::GetMergeDocToShow(int nFiles, CDirDoc * pDirDoc, BOOL * pNew)
{
	const BOOL bMultiDocs = GetOptionsMgr()->GetBool(OPT_MULTIDOC_MERGEDOCS);
	const MergeDocList &docs = GetAllMergeDocs();

	if (!pDirDoc->HasDiffs() && !bMultiDocs && !docs.IsEmpty())
	{
		POSITION pos = docs.GetHeadPosition();
		CMergeDoc * pMergeDoc = docs.GetAt(pos);
		pMergeDoc->CloseNow();
	}
	CMergeDoc * pMergeDoc = pDirDoc->GetMergeDocForDiff(nFiles, pNew);
	return pMergeDoc;
}

/**
 * @brief Obtain a hex merge doc to display a difference in files.
 * This function (usually) uses DirDoc to determine if new or existing
 * MergeDoc is used. However there is exceptional case when DirDoc does
 * not contain diffs. Then we have only file compare, and if we also have
 * limited file compare windows, we always reuse existing MergeDoc.
 * @param [in] pDirDoc Dir compare document.
 * @param [out] pNew Did we create a new document?
 * @return Pointer to merge doucument.
 */
CHexMergeDoc * CMainFrame::GetHexMergeDocToShow(int nFiles, CDirDoc * pDirDoc, BOOL * pNew)
{
	const BOOL bMultiDocs = GetOptionsMgr()->GetBool(OPT_MULTIDOC_MERGEDOCS);
	const HexMergeDocList &docs = GetAllHexMergeDocs();

	if (!pDirDoc->HasDiffs() && !bMultiDocs && !docs.IsEmpty())
	{
		POSITION pos = docs.GetHeadPosition();
		CHexMergeDoc * pHexMergeDoc = docs.GetAt(pos);
		pHexMergeDoc->CloseNow();
	}
	CHexMergeDoc * pHexMergeDoc = pDirDoc->GetHexMergeDocForDiff(nFiles, pNew);
	return pHexMergeDoc;
}

/// Get pointer to a dir doc for displaying a scan
CDirDoc * CMainFrame::GetDirDocToShow(int nDirs, BOOL * pNew)
{
	CDirDoc * pDirDoc = 0;
	if (!GetOptionsMgr()->GetBool(OPT_MULTIDOC_DIRDOCS))
	{
		POSITION pos = theApp.m_pDirTemplate->GetFirstDocPosition();
		while (pos)
		{			
			CDirDoc *pDirDocTemp = static_cast<CDirDoc *>(theApp.m_pDirTemplate->GetNextDoc(pos));
			if (pDirDocTemp->HasDirView())
			{
				*pNew = FALSE;
				pDirDoc = pDirDocTemp;
				break;
			}
		}
	}
	if (!pDirDoc)
	{
		CDirDoc::m_nDirsTemp = nDirs;
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

// Clear the item count in the main status pane
void CMainFrame::ClearStatusbarItemCount()
{
	m_wndStatusBar.SetPaneText(2, _T(""));
}

/**
 * @brief Generate patch from files selected.
 *
 * Creates a patch from selected files in active directory compare, or
 * active file compare. Files in file compare must be saved before
 * creating a patch.
 */
void CMainFrame::OnToolsGeneratePatch()
{
	CPatchTool patcher;
	CFrameWnd * pFrame = GetActiveFrame();
	FRAMETYPE frame = GetFrameType(pFrame);
	BOOL bOpenDialog = TRUE;

	// Mergedoc active?
	if (frame == FRAME_FILE)
	{
		CMergeDoc * pMergeDoc = (CMergeDoc *) pFrame->GetActiveDocument();
		// If there are changes in files, tell user to save them first
		BOOL bModified = FALSE;
		for (int pane = 0; pane < pMergeDoc->m_nBuffers; pane++)
		{
			if (pMergeDoc->m_ptBuf[pane]->IsModified())
				bModified = TRUE;
		}
		if (bModified)
		{
			bOpenDialog = FALSE;
			LangMessageBox(IDS_SAVEFILES_FORPATCH, MB_ICONSTOP);
		}
		else
		{
			patcher.AddFiles(pMergeDoc->m_filePaths.GetLeft(),
					pMergeDoc->m_filePaths.GetRight());
		}
	}
	// Dirview active
	else if (frame == FRAME_FOLDER)
	{
		CDirDoc * pDoc = (CDirDoc*)pFrame->GetActiveDocument();
		CDirView *pView = pDoc->GetMainView();

		// Get selected items from folder compare
		BOOL bValidFiles = TRUE;
		int ind = pView->GetFirstSelectedInd();
		while (ind != -1 && bValidFiles)
		{
			const DIFFITEM &item = pView->GetItemAt(ind);
			if (item.diffcode.isBin())
			{
				LangMessageBox(IDS_CANNOT_CREATE_BINARYPATCH, MB_ICONWARNING |
					MB_DONT_DISPLAY_AGAIN, IDS_CANNOT_CREATE_BINARYPATCH);
				bValidFiles = FALSE;
			}
			else if (item.diffcode.isDirectory())
			{
				LangMessageBox(IDS_CANNOT_CREATE_DIRPATCH, MB_ICONWARNING |
					MB_DONT_DISPLAY_AGAIN, IDS_CANNOT_CREATE_DIRPATCH);
				bValidFiles = FALSE;
			}

			if (bValidFiles)
			{
				// Format full paths to files (leftFile/rightFile)
				String leftFile = item.getFilepath(0, pDoc->GetBasePath(0));
				if (!leftFile.empty())
					leftFile = paths_ConcatPath(leftFile, item.diffFileInfo[0].filename);
				String rightFile = item.getFilepath(1, pDoc->GetBasePath(1));
				if (!rightFile.empty())
					rightFile = paths_ConcatPath(rightFile, item.diffFileInfo[1].filename);

				// Format relative paths to files in folder compare
				String leftpatch = item.diffFileInfo[0].path;
				if (!leftpatch.empty())
					leftpatch += _T("/");
				leftpatch += item.diffFileInfo[0].filename;
				String rightpatch = item.diffFileInfo[1].path;
				if (!rightpatch.empty())
					rightpatch += _T("/");
				rightpatch += item.diffFileInfo[1].filename;
				patcher.AddFiles(leftFile, leftpatch, rightFile, rightpatch);
				pView->GetNextSelectedInd(ind);
			}
		}
	}

	if (bOpenDialog)
	{
		if (patcher.CreatePatch())
		{
			if (patcher.GetOpenToEditor())
			{
				OpenFileToExternalEditor(patcher.GetPatchFile().c_str());
			}
		}
	}
}

void CMainFrame::OnDropFiles(HDROP dropInfo)
{
	std::vector<String> dropped_files;
	GetDroppedFiles(dropInfo, dropped_files);
	PathContext files(dropped_files);
	const size_t fileCount = files.GetSize();

	// If Ctrl pressed, do recursive compare
	bool ctrlKey = !!::GetAsyncKeyState(VK_CONTROL);

	// If user has <Shift> pressed with one file selected,
	// assume it is an archive and set filenames to same
	if (::GetAsyncKeyState(VK_SHIFT) < 0 && fileCount == 1)
	{
		files.SetRight(files[0]);
	}

	// Check if they dropped a project file
	DWORD dwFlags[3] = {FFILEOPEN_NONE, FFILEOPEN_NONE, FFILEOPEN_NONE};
	if (fileCount == 1)
	{
		if (theApp.IsProjectFile(files[0].c_str()))
		{
			theApp.LoadAndOpenProjectFile(files[0].c_str());
			return;
		}
		if (IsConflictFile(files[0]))
		{
			DoOpenConflict(files[0], true);
			return;
		}
	}

	DoFileOpen(&files, dwFlags, ctrlKey);
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
	if (GetOptionsMgr()->GetBool(OPT_PLUGINS_ENABLED))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);

	if (pCmdUI->m_nID == ID_UNPACK_MANUAL)
		pCmdUI->SetRadio(PLUGIN_MANUAL == g_bUnpackerMode);
	if (pCmdUI->m_nID == ID_UNPACK_AUTO)
		pCmdUI->SetRadio(PLUGIN_AUTO == g_bUnpackerMode);
}
void CMainFrame::OnPluginPrediffMode(UINT nID )
{
	switch (nID)
	{
	case ID_PREDIFFER_MANUAL:
		g_bPredifferMode = PLUGIN_MANUAL;
		break;
	case ID_PREDIFFER_AUTO:
		g_bPredifferMode = PLUGIN_AUTO;
		break;
	}
	theApp.WriteProfileInt(_T("Settings"), _T("PredifferMode"), g_bPredifferMode);
}

void CMainFrame::OnUpdatePluginPrediffMode(CCmdUI* pCmdUI) 
{
	if (GetOptionsMgr()->GetBool(OPT_PLUGINS_ENABLED))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);

	if (pCmdUI->m_nID == ID_PREDIFFER_MANUAL)
		pCmdUI->SetRadio(PLUGIN_MANUAL == g_bPredifferMode);
	if (pCmdUI->m_nID == ID_PREDIFFER_AUTO)
		pCmdUI->SetRadio(PLUGIN_AUTO == g_bPredifferMode);
}
/**
 * @brief Called when "Reload Plugins" item is updated
 */
void CMainFrame::OnUpdateReloadPlugins(CCmdUI* pCmdUI)
{
	if (GetOptionsMgr()->GetBool(OPT_PLUGINS_ENABLED))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
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

/** @brief Return active merge edit view, if can figure it out/is available */
CMergeEditView * CMainFrame::GetActiveMergeEditView()
{
	// NB: GetActiveDocument does not return the Merge Doc 
	//     even when the merge edit view is in front
	// NB: CChildFrame::GetActiveView returns NULL when location view active
	// So we have this rather complicated logic to try to get a merge edit view
	// We look at the front child window, which should be a frame
	// and we can get a MergeEditView from it, if it is a CChildFrame
	// (DirViews use a different frame type)
	CChildFrame * pFrame = dynamic_cast<CChildFrame *>(GetActiveFrame());
	if (!pFrame) return 0;
	// Try to get the active MergeEditView (ie, left or right)
	if (pFrame->GetActiveView() && pFrame->GetActiveView()->IsKindOf(RUNTIME_CLASS(CMergeEditView)))
	{
		return dynamic_cast<CMergeEditView *>(pFrame->GetActiveView());
	}
	return pFrame->GetMergeDoc()->GetActiveMergeView();
}

void CMainFrame::UpdatePrediffersMenu()
{
	CMenu* menu = GetMenu();
	if (menu == NULL)
	{
		return;
	}

	HMENU hMainMenu = menu->m_hMenu;
	HMENU prediffersSubmenu = GetPrediffersSubmenu(hMainMenu);
	if (prediffersSubmenu != NULL)
	{
		CMergeEditView * pEditView = GetActiveMergeEditView();
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
 * @brief Open given file to external editor specified in options.
 * @param [in] file Full path to file to open.
 *
 * Opens file to defined (in Options/system), Notepad by default,
 * external editor. Path is decorated with quotation marks if needed
 * (contains spaces). Also '$file' in editor path is replaced by
 * filename to open.
 * @param [in] file Full path to file to open.
 * @param [in] nLineNumber Line number to go to.
 */
void CMainFrame::OpenFileToExternalEditor(const String& file, int nLineNumber/* = 1*/)
{
	String sCmd = GetOptionsMgr()->GetString(OPT_EXT_EDITOR_CMD);
	String sFile(file);
	string_replace(sCmd, _T("$linenum"), string_format(_T("%d"), nLineNumber));

	int nIndex = sCmd.find(_T("$file"));
	if (nIndex > -1)
	{
		sFile.insert(0, _T("\""));
		string_replace(sCmd, _T("$file"), sFile);
		nIndex = sCmd.find(' ', nIndex + sFile.length());
		if (nIndex > -1)
			sCmd.insert(nIndex, _T("\""));
		else
			sCmd += '"';
	}
	else
	{
		sCmd += _T(" \"");
		sCmd += sFile;
		sCmd += _T("\"");
	}

	BOOL retVal = FALSE;
	STARTUPINFO stInfo = {0};
	stInfo.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION processInfo;

	retVal = CreateProcess(NULL, (LPTSTR)sCmd.c_str(),
		NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL,
		&stInfo, &processInfo);

	if (!retVal)
	{
		// Error invoking external editor
		String msg = string_format_string1(_("Failed to execute external editor: %1"), sCmd);
		AfxMessageBox(msg.c_str(), MB_ICONSTOP);
	}
	else
	{
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}
}

/**
 * @brief Save WinMerge configuration and info to file
 */
void CMainFrame::OnSaveConfigData()
{
	CConfigLog configLog;
	String sError;

	if (configLog.WriteLogFile(sError))
	{
		String sFileName = configLog.GetFileName();
		OpenFileToExternalEditor(sFileName);
	}
	else
	{
		String sFileName = configLog.GetFileName();
		String msg = string_format_string2(_("Cannot open file\n%1\n\n%2"), sFileName, sError);
		AfxMessageBox(msg.c_str(), MB_OK | MB_ICONSTOP);
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
void CMainFrame::FileNew(int nPanes) 
{
	CDirDoc *pDirDoc = (CDirDoc*)theApp.m_pDirTemplate->CreateNewDocument();
	
	// Load emptyfile descriptors and open empty docs
	// Use default codepage
	DWORD dwFlags[3] = {0, 0, 0};
	FileLocation fileloc[3];
	if (nPanes == 2)
	{
		m_strDescriptions[0] = _("Untitled left");
		m_strDescriptions[1] = _("Untitled right");
		fileloc[0].encoding.SetCodepage(ucr::getDefaultCodepage());
		fileloc[1].encoding.SetCodepage(ucr::getDefaultCodepage());
		ShowMergeDoc(pDirDoc, 2, fileloc, dwFlags);
	}
	else
	{
		m_strDescriptions[0] = _("Untitled left");
		m_strDescriptions[1] = _("Untitled middle");
		m_strDescriptions[2] = _("Untitled right");
		fileloc[0].encoding.SetCodepage(ucr::getDefaultCodepage());
		fileloc[1].encoding.SetCodepage(ucr::getDefaultCodepage());
		fileloc[2].encoding.SetCodepage(ucr::getDefaultCodepage());
		ShowMergeDoc(pDirDoc, 3, fileloc, dwFlags);
	}

	// Empty descriptors now that docs are open
	m_strDescriptions[0].erase();
	m_strDescriptions[1].erase();
	m_strDescriptions[2].erase();
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
	FileNew(2);
}

void CMainFrame::OnFileNew3() 
{
	FileNew(3);
}

/**
 * @brief Open Filters dialog
 */
void CMainFrame::OnToolsFilters()
{
	String title = _("Filters");
	CPropertySheet sht(title.c_str());
	LineFiltersDlg lineFiltersDlg;
	FileFiltersDlg fileFiltersDlg;
	vector<FileFilterInfo> fileFilters;
	boost::scoped_ptr<LineFiltersList> lineFilters(new LineFiltersList());
	String selectedFilter;
	const String origFilter = theApp.m_globalFileFilter.GetFilterNameOrMask();
	sht.AddPage(&fileFiltersDlg);
	sht.AddPage(&lineFiltersDlg);
	sht.m_psh.dwFlags |= PSH_NOAPPLYNOW; // Hide 'Apply' button since we don't need it

	// Make sure all filters are up-to-date
	theApp.m_globalFileFilter.ReloadUpdatedFilters();

	theApp.m_globalFileFilter.GetFileFilters(&fileFilters, selectedFilter);
	fileFiltersDlg.SetFilterArray(&fileFilters);
	fileFiltersDlg.SetSelected(selectedFilter);
	const BOOL lineFiltersEnabledOrig = GetOptionsMgr()->GetBool(OPT_LINEFILTER_ENABLED);
	lineFiltersDlg.m_bIgnoreRegExp = lineFiltersEnabledOrig;

	lineFilters->CloneFrom(m_pLineFilters.get());
	lineFiltersDlg.SetList(lineFilters.get());

	if (sht.DoModal() == IDOK)
	{
		String strNone = _("<None>");
		String path = fileFiltersDlg.GetSelected();
		if (path.find(strNone) != String::npos)
		{
			// Don't overwrite mask we already have
			if (!theApp.m_globalFileFilter.IsUsingMask())
			{
				String sFilter(_T("*.*"));
				theApp.m_globalFileFilter.SetFilter(sFilter);
				GetOptionsMgr()->SaveOption(OPT_FILEFILTER_CURRENT, sFilter);
			}
		}
		else
		{
			theApp.m_globalFileFilter.SetFileFilterPath(path);
			theApp.m_globalFileFilter.UseMask(FALSE);
			String sFilter = theApp.m_globalFileFilter.GetFilterNameOrMask();
			GetOptionsMgr()->SaveOption(OPT_FILEFILTER_CURRENT, sFilter);
		}
		BOOL linefiltersEnabled = lineFiltersDlg.m_bIgnoreRegExp;
		GetOptionsMgr()->SaveOption(OPT_LINEFILTER_ENABLED, linefiltersEnabled == TRUE);

		// Check if compare documents need rescanning
		BOOL bFileCompareRescan = FALSE;
		BOOL bFolderCompareRescan = FALSE;
		CFrameWnd * pFrame = GetActiveFrame();
		FRAMETYPE frame = GetFrameType(pFrame);
		if (frame == FRAME_FILE)
		{
			if (lineFiltersEnabledOrig != linefiltersEnabled ||
					!m_pLineFilters->Compare(lineFilters.get()))
			{
				bFileCompareRescan = TRUE;
			}
		}
		else if (frame == FRAME_FOLDER)
		{
			const String newFilter = theApp.m_globalFileFilter.GetFilterNameOrMask();
			if (lineFiltersEnabledOrig != linefiltersEnabled || 
					!m_pLineFilters->Compare(lineFilters.get()) || origFilter != newFilter)
			{
				int res = LangMessageBox(IDS_FILTERCHANGED, MB_ICONWARNING | MB_YESNO);
				if (res == IDYES)
					bFolderCompareRescan = TRUE;
			}
		}

		// Save new filters before (possibly) rescanning
		m_pLineFilters->CloneFrom(lineFilters.get());
		m_pLineFilters->SaveFilters();

		if (bFileCompareRescan)
		{
			const MergeDocList &docs = GetAllMergeDocs();
			POSITION pos = docs.GetHeadPosition();
			while (pos)
			{
				CMergeDoc * pMergeDoc = docs.GetNext(pos);
				pMergeDoc->FlushAndRescan(TRUE);
			}
		}
		else if (bFolderCompareRescan)
		{
			const DirDocList &dirDocs = GetAllDirDocs();
			POSITION pos = dirDocs.GetHeadPosition();
			while (pos)
			{
				CDirDoc * pDirDoc = dirDocs.GetNext(pos);
				pDirDoc->Rescan();
			}
		}
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
 * - 'Close Windows with ESC' option is enabled and
 *    there is no open document windows
 * - '-e' commandline switch is given
 */
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// Check if we got 'ESC pressed' -message
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		const OpenDocList &openDocs = GetAllOpenDocs();
		const MergeDocList &docs = GetAllMergeDocs();
		const HexMergeDocList &hexDocs = GetAllHexMergeDocs();
		const DirDocList &dirDocs = GetAllDirDocs();

		if (m_bEscShutdown)
		{
			if (openDocs.GetCount() + hexDocs.GetCount() + dirDocs.GetCount() <= 1)
			{
				AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT);
				return TRUE;
			}
		}
		else
		{
			if (GetOptionsMgr()->GetBool(OPT_CLOSE_WITH_ESC))
			{
				if (openDocs.IsEmpty() && docs.IsEmpty() && hexDocs.IsEmpty() && dirDocs.IsEmpty())
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
 * @brief Show Help - this is for opening help from outside mainframe.
 * @param [in] helpLocation Location inside help, if NULL main help is opened.
 */
void CMainFrame::ShowHelp(LPCTSTR helpLocation /*= NULL*/)
{
	if (helpLocation == NULL)
	{
		OnHelpContents();
	}
	else
	{
		String sPath = env_GetProgPath();
		LANGID LangId = GetUserDefaultLangID();
		if (PRIMARYLANGID(LangId) == LANG_JAPANESE)
			sPath = paths_ConcatPath(sPath, DocsPath_ja);
		else
			sPath = paths_ConcatPath(sPath, DocsPath);
		if (paths_DoesPathExist(sPath) == IS_EXISTING_FILE)
		{
			sPath += helpLocation;
			::HtmlHelp(NULL, sPath.c_str(), HH_DISPLAY_TOPIC, NULL);
		}
	}
}

/**
 * @brief Tell user why archive support is not available.
 */
void CMainFrame::OnHelpMerge7zmismatch() 
{
	Recall7ZipMismatchError();
}

/**
 * @brief Remove ID_HELP_MERGE7ZMISMATCH if archive support is available.
 */
void CMainFrame::OnUpdateHelpMerge7zmismatch(CCmdUI* pCmdUI) 
{
	if (HasZipSupport())
	{
		pCmdUI->m_pMenu->RemoveMenu(ID_HELP_MERGE7ZMISMATCH, MF_BYCOMMAND);
	}
}

/**
 * @brief Show/hide statusbar.
 */
void CMainFrame::OnViewStatusBar()
{
	bool bShow = !GetOptionsMgr()->GetBool(OPT_SHOW_STATUSBAR);
	GetOptionsMgr()->SaveOption(OPT_SHOW_STATUSBAR, bShow);

	CMDIFrameWnd::ShowControlBar(&m_wndStatusBar, bShow, 0);
}

/**
 * @brief Updates "Show Tabbar" menuitem.
 */
void CMainFrame::OnUpdateViewTabBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_SHOW_TABBAR));
}

/**
 * @brief Show/hide tabbar.
 */
void CMainFrame::OnViewTabBar()
{
	bool bShow = !GetOptionsMgr()->GetBool(OPT_SHOW_TABBAR);
	GetOptionsMgr()->SaveOption(OPT_SHOW_TABBAR, bShow);

	CMDIFrameWnd::ShowControlBar(&m_wndTabBar, bShow, 0);
}

/**
 * @brief Updates "Automatically Resize Panes" menuitem.
 */
void CMainFrame::OnUpdateResizePanes(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetOptionsMgr()->GetBool(OPT_RESIZE_PANES));
}


/**
 * @brief Enable/disable automatic pane resizing.
 */
void CMainFrame::OnResizePanes()
{
	bool bResize = !GetOptionsMgr()->GetBool(OPT_RESIZE_PANES);
	GetOptionsMgr()->SaveOption(OPT_RESIZE_PANES, bResize);
	// TODO: Introduce a common merge frame superclass?
	CFrameWnd *pActiveFrame = GetActiveFrame();
	if (CChildFrame *pFrame = DYNAMIC_DOWNCAST(CChildFrame, pActiveFrame))
	{
		pFrame->UpdateAutoPaneResize();
		if (bResize)
			pFrame->UpdateSplitter();
	}
	else if (CHexMergeFrame *pFrame = DYNAMIC_DOWNCAST(CHexMergeFrame, pActiveFrame))
	{
		pFrame->UpdateAutoPaneResize();
		if (bResize)
			pFrame->UpdateSplitter();
	}
}

/**
 * @brief Open project-file.
 */
void CMainFrame::OnFileOpenproject()
{
	String sFilepath;
	
	// get the default projects path
	String strProjectPath = GetOptionsMgr()->GetString(OPT_PROJECTS_PATH);
	if (!SelectFile(GetSafeHwnd(), sFilepath, strProjectPath.c_str(), _("Open"),
			_("WinMerge Project Files (*.WinMerge)|*.WinMerge||"), TRUE))
		return;
	
	strProjectPath = paths_GetParentPath(sFilepath);
	// store this as the new project path
	GetOptionsMgr()->SaveOption(OPT_PROJECTS_PATH, strProjectPath);

	theApp.LoadAndOpenProjectFile(sFilepath.c_str());
}

/**
 * @brief Receive command line from another instance.
 *
 * This function receives command line when only single-instance
 * is allowed. New instance tried to start sends its command line
 * to here so we can open paths it was meant to.
 */
LRESULT CMainFrame::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
	LPCTSTR pchData = (LPCTSTR)pCopyData->lpData;
	// Bail out if data isn't zero-terminated
	DWORD cchData = pCopyData->cbData / sizeof(TCHAR);
	if (cchData == 0 || pchData[cchData - 1] != _T('\0'))
		return FALSE;
	ReplyMessage(TRUE);
	MergeCmdLineInfo cmdInfo = pchData;
	theApp.ParseArgsAndDoOpen(cmdInfo, this);
	return TRUE;
}

LRESULT CMainFrame::OnUser1(WPARAM wParam, LPARAM lParam)
{
	CFrameWnd * pFrame = GetActiveFrame();
	if (pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		CMergeDoc * pMergeDoc = (CMergeDoc *) pFrame->GetActiveDocument();
		if (pMergeDoc)
			pMergeDoc->CheckFileChanged();
	}
	return 0;
}

/**
 * @brief Send current option settings into codepage module
 */
void CMainFrame::UpdateCodepageModule()
{
	// Get current codepage settings from the options module
	// and push them into the codepage module
	theApp.UpdateDefaultCodepage(GetOptionsMgr()->GetInt(OPT_CP_DEFAULT_MODE), GetOptionsMgr()->GetInt(OPT_CP_DEFAULT_CUSTOM));
}

/**
 * @brief Handle timer events.
 * @param [in] nIDEvent Timer that timed out.
 */
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case WM_NONINTERACTIVE:
		KillTimer(WM_NONINTERACTIVE);
		PostMessage(WM_CLOSE);
		break;
	
	case ID_TIMER_FLASH:
		// This timer keeps running until window is activated
		// See OnActivate()
		FlashWindow(TRUE);
		break;
	}
	CMDIFrameWnd::OnTimer(nIDEvent);
}

/**
 * @brief Close all open windows.
 * 
 * Asks about saving unsaved files and then closes all open windows.
 */
void CMainFrame::OnWindowCloseAll()
{
	CMDIChildWnd *pChild = MDIGetActive();
	CDocument* pDoc;
	while (pChild)
	{
		if ((pDoc = pChild->GetActiveDocument()) != NULL)
		{
			if (!pDoc->SaveModified())
				return;
			pDoc->OnCloseDocument();
		}
		pChild = MDIGetActive();
	}
	return;
}

/**
 * @brief Enables Window/Close All item if there are open windows.
 */ 
void CMainFrame::OnUpdateWindowCloseAll(CCmdUI* pCmdUI)
{
	const MergeDocList &mergedocs = GetAllMergeDocs();
	if (!mergedocs.IsEmpty())
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	const DirDocList &dirdocs = GetAllDirDocs();
	if (!dirdocs.IsEmpty())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/**
 * @brief Access to the singleton main frame (where we have some globals)
 */
CMainFrame * GetMainFrame()
{
	CWnd * mainwnd = AfxGetMainWnd();
	ASSERT(mainwnd);
	CMainFrame *pMainframe = dynamic_cast<CMainFrame*>(mainwnd);
	ASSERT(pMainframe);
	return pMainframe;
}

/**
 * @brief Access to the singleton set of syntax colors
 */
SyntaxColors * GetMainSyntaxColors()
{
	return GetMainFrame()->GetMainSyntaxColors();
}

/** 
 * @brief Move dialog to center of MainFrame
 */
void CMainFrame::CenterToMainFrame(CDialog * dlg)
{
	CRect rectFrame;
	CRect rectBar;
	AfxGetMainWnd()->GetWindowRect(&rectFrame);
	dlg->GetClientRect(&rectBar);
	// Middlepoint of MainFrame
	int x = rectFrame.left + (rectFrame.right - rectFrame.left) / 2;
	int y = rectFrame.top + (rectFrame.bottom - rectFrame.top) / 2;
	// Reduce by half of dialog's size
	x -= rectBar.right / 2;
	y -= rectBar.bottom / 2;

	// This refreshes dialog size after m_constraint rezizing dialog so we get
	// correct dialog positioning
	dlg->CenterWindow();

	// Calculate real desktop coordinates (if we have multiple monitors or
	// virtual desktops
	CRect dsk_rc;
	dsk_rc.left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	dsk_rc.top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	dsk_rc.right = dsk_rc.left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	dsk_rc.bottom = dsk_rc.top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

	// Only move Open-dialog if its fully visible in new position
	CPoint ptLeftTop(x, y);
	CPoint ptRightBottom(x + rectBar.right, y + rectBar.bottom);
	if (dsk_rc.PtInRect(ptLeftTop) && dsk_rc.PtInRect(ptRightBottom))
	{
		dlg->SetWindowPos(&CWnd::wndTop, x, y, rectBar.right,
			rectBar.bottom, SWP_NOOWNERZORDER | SWP_NOSIZE );
	}
}

/**
 * @brief Assign the main WinMerge 16x16 icon to dialog
 */
void CMainFrame::SetMainIcon(CDialog * dlg)
{
	// Note: LoadImage gets shared icon, don't need to destroy
	HICON hMergeIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16,
			LR_DEFAULTSIZE | LR_SHARED);
	dlg->SetIcon(hMergeIcon, TRUE);
}

/** 
 * @brief Opens dialog for user to Load, edit and save project files.
 * This dialog gets current compare paths and filter (+other properties
 * possible in project files) and initializes the dialog with them.
 */
void CMainFrame::OnSaveProject()
{
	String title = _("Project File");
	CPropertySheet sht(title.c_str());
	ProjectFilePathsDlg pathsDlg;
	sht.AddPage(&pathsDlg);
	sht.m_psh.dwFlags |= PSH_NOAPPLYNOW; // Hide 'Apply' button since we don't need it

	String left;
	String right;
	CFrameWnd * pFrame = GetActiveFrame();
	FRAMETYPE frame = GetFrameType(pFrame);

	if (frame == FRAME_FILE)
	{
		CMergeDoc * pMergeDoc = (CMergeDoc *) pFrame->GetActiveDocument();
		left = pMergeDoc->m_filePaths.GetLeft();
		right = pMergeDoc->m_filePaths.GetRight();
		pathsDlg.SetPaths(left.c_str(), right.c_str());
		pathsDlg.m_bLeftPathReadOnly = pMergeDoc->m_ptBuf[0]->GetReadOnly();
		pathsDlg.m_bRightPathReadOnly = pMergeDoc->m_ptBuf[1]->GetReadOnly();
	}
	else if (frame == FRAME_FOLDER)
	{
		// Get paths currently in compare
		CDirDoc * pDoc = (CDirDoc*)pFrame->GetActiveDocument();
		left = paths_AddTrailingSlash(pDoc->GetLeftBasePath());
		right = paths_AddTrailingSlash(pDoc->GetRightBasePath());
		
		// Set-up the dialog
		pathsDlg.SetPaths(left.c_str(), right.c_str());
		pathsDlg.m_bIncludeSubfolders = pDoc->GetRecursive();
		pathsDlg.m_bLeftPathReadOnly = pDoc->GetReadOnly(0);
		pathsDlg.m_bRightPathReadOnly = pDoc->GetReadOnly(pDoc->m_nDirs - 1);
	}

	String filterNameOrMask = theApp.m_globalFileFilter.GetFilterNameOrMask();
	pathsDlg.m_sFilter = filterNameOrMask.c_str();
	sht.DoModal();
}

/** 
 * @brief Start flashing window if window is inactive.
 */
void CMainFrame::StartFlashing()
{
	CWnd * activeWindow = GetActiveWindow();
	if (activeWindow != this)
	{
		FlashWindow(TRUE);
		m_bFlashing = TRUE;
		SetTimer(ID_TIMER_FLASH, WINDOW_FLASH_TIMEOUT, NULL);
	}
}

/** 
 * @brief Stop flashing window when window is activated.
 *
 * If WinMerge is inactive when compare finishes, we start flashing window
 * to alert user. When user activates WinMerge window we stop flashing.
 * @param [in] nState Tells if window is being activated or deactivated.
 * @param [in] pWndOther Pointer to window whose status is changing.
 * @param [in] Is window minimized?
 */
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
	{
		if (m_bFlashing)
		{
			m_bFlashing = FALSE;
			FlashWindow(FALSE);
			KillTimer(ID_TIMER_FLASH);
		}
	}
}

#if _MFC_VER > 0x0600
void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
#else
void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask)
#endif
{
#if _MFC_VER > 0x0600
	CMDIFrameWnd::OnActivateApp(bActive, dwThreadID);
#else
	CMDIFrameWnd::OnActivateApp(bActive, hTask);
#endif

	CFrameWnd * pFrame = GetActiveFrame();
	if (pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		CMergeDoc * pMergeDoc = (CMergeDoc *) pFrame->GetActiveDocument();
		if (pMergeDoc)
			PostMessage(WM_USER+1);
	}
}

BOOL CMainFrame::CreateToobar()
{
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		return FALSE;
	}

	if (!m_wndReBar.Create(this))
	{
		return FALSE;
	}

	VERIFY(m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT|TBSTYLE_TRANSPARENT));

	// Remove this if you don't want tool tips or a resizable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndReBar.AddBar(&m_wndToolBar);

	LoadToolbarImages();

	if (GetOptionsMgr()->GetBool(OPT_SHOW_TOOLBAR) == false)
	{
		CMDIFrameWnd::ShowControlBar(&m_wndToolBar, false, 0);
	}

	return TRUE;
}

/** @brief Load toolbar images from the resource. */
void CMainFrame::LoadToolbarImages()
{
	int toolbarSize = GetOptionsMgr()->GetInt(OPT_TOOLBAR_SIZE);
	CToolBarCtrl& BarCtrl = m_wndToolBar.GetToolBarCtrl();

	m_ToolbarImages[TOOLBAR_IMAGES_ENABLED].DeleteImageList();
	m_ToolbarImages[TOOLBAR_IMAGES_DISABLED].DeleteImageList();
	CSize sizeButton(0, 0);

	if (toolbarSize == 0)
	{
		LoadToolbarImageList(TOOLBAR_SIZE_16x16, IDB_TOOLBAR_ENABLED,
			m_ToolbarImages[TOOLBAR_IMAGES_ENABLED]);
		LoadToolbarImageList(TOOLBAR_SIZE_16x16, IDB_TOOLBAR_DISABLED,
			m_ToolbarImages[TOOLBAR_IMAGES_DISABLED]);
		sizeButton = CSize(24, 24);
	}
	else if (toolbarSize == 1)
	{
		LoadToolbarImageList(TOOLBAR_SIZE_32x32, IDB_TOOLBAR_ENABLED32,
			m_ToolbarImages[TOOLBAR_IMAGES_ENABLED]);
		LoadToolbarImageList(TOOLBAR_SIZE_32x32, IDB_TOOLBAR_DISABLED32,
			m_ToolbarImages[TOOLBAR_IMAGES_DISABLED]);
		sizeButton = CSize(40, 40);
	}

	BarCtrl.SetButtonSize(sizeButton);
	BarCtrl.SetImageList(&m_ToolbarImages[TOOLBAR_IMAGES_ENABLED]);
	BarCtrl.SetDisabledImageList(&m_ToolbarImages[TOOLBAR_IMAGES_DISABLED]);
	m_ToolbarImages[TOOLBAR_IMAGES_ENABLED].Detach();
	m_ToolbarImages[TOOLBAR_IMAGES_DISABLED].Detach();

	// resize the rebar.
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE;
	rbbi.cyMinChild = sizeButton.cy;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);
}


/**
 * @brief Load a transparent 24-bit color image list.
 */
static void LoadHiColImageList(UINT nIDResource, int nWidth, int nHeight, int nCount, CImageList& ImgList, COLORREF crMask = RGB(255,0,255))
{
	CBitmap bm;
	VERIFY(bm.LoadBitmap(nIDResource));
	VERIFY(ImgList.Create(nWidth, nHeight, ILC_COLORDDB|ILC_MASK, nCount, 0));
	int nIndex = ImgList.Add(&bm, crMask);
	ASSERT(-1 != nIndex);
}

/**
 * @brief Load toolbar image list.
 */
static void LoadToolbarImageList(CMainFrame::TOOLBAR_SIZE size, UINT nIDResource,
		CImageList& ImgList)
{
	const int ImageCount = 19;
	int imageWidth = 0;
	int imageHeight = 0;

	switch (size)
	{
		case CMainFrame::TOOLBAR_SIZE_16x16:
			imageWidth = 16;
			imageHeight = 15;
			break;
		case CMainFrame::TOOLBAR_SIZE_32x32:
			imageWidth = 32;
			imageHeight = 31;
			break;
	}

	LoadHiColImageList(nIDResource, imageWidth, imageHeight, ImageCount, ImgList);
}

/**
 * @brief Called when the document title is modified.
 */
void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
	
	m_wndTabBar.UpdateTabs();
}

/** @brief Hide the toolbar. */
void CMainFrame::OnToolbarNone()
{
	GetOptionsMgr()->SaveOption(OPT_SHOW_TOOLBAR, false);
	CMDIFrameWnd::ShowControlBar(&m_wndToolBar, FALSE, 0);
}

/** @brief Update menuitem for hidden toolbar. */
void CMainFrame::OnUpdateToolbarNone(CCmdUI* pCmdUI)
{
	bool enabled = GetOptionsMgr()->GetBool(OPT_SHOW_TOOLBAR);
	pCmdUI->SetRadio(!enabled);
}

/** @brief Show small toolbar. */
void CMainFrame::OnToolbarSmall()
{
	GetOptionsMgr()->SaveOption(OPT_SHOW_TOOLBAR, true);
	GetOptionsMgr()->SaveOption(OPT_TOOLBAR_SIZE, 0);

	LoadToolbarImages();

	CMDIFrameWnd::ShowControlBar(&m_wndToolBar, TRUE, 0);
}

/** @brief Update menuitem for small toolbar. */
void CMainFrame::OnUpdateToolbarSmall(CCmdUI* pCmdUI)
{
	bool enabled = GetOptionsMgr()->GetBool(OPT_SHOW_TOOLBAR);
	int toolbar = GetOptionsMgr()->GetInt(OPT_TOOLBAR_SIZE);
	pCmdUI->SetRadio(enabled && toolbar == 0);
}

/** @brief Show big toolbar. */
void CMainFrame::OnToolbarBig()
{
	GetOptionsMgr()->SaveOption(OPT_SHOW_TOOLBAR, true);
	GetOptionsMgr()->SaveOption(OPT_TOOLBAR_SIZE, 1);

	LoadToolbarImages();

	CMDIFrameWnd::ShowControlBar(&m_wndToolBar, TRUE, 0);
}

/** @brief Update menuitem for big toolbar. */
void CMainFrame::OnUpdateToolbarBig(CCmdUI* pCmdUI)
{
	bool enabled = GetOptionsMgr()->GetBool(OPT_SHOW_TOOLBAR);
	int toolbar = GetOptionsMgr()->GetInt(OPT_TOOLBAR_SIZE);
	pCmdUI->SetRadio(enabled && toolbar == 1);
}

/** @brief Lang aware version of CFrameWnd::OnToolTipText() */
BOOL CMainFrame::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	String strFullText;
	CString strTipText;
	UINT_PTR nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) // will be zero on a separator
	{
		strFullText = theApp.LoadString(nID);
		// don't handle the message if no string resource found
		if (strFullText.empty())
			return FALSE;

		// this is the command id, not the button index
		AfxExtractSubString(strTipText, strFullText.c_str(), 1, '\n');
	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return TRUE;    // message was handled
}

/**
 * @brief Ask user for close confirmation when closing the mainframe.
 * This function asks if user wants to close multiple open windows when user
 * selects (perhaps accidentally) to close WinMerge (application).
 * @return true if user agreeds to close all windows.
 */
bool CMainFrame::AskCloseConfirmation()
{
	const DirDocList &dirdocs = GetAllDirDocs();
	const MergeDocList &mergedocs = GetAllMergeDocs();

	int ret = IDYES;
	const int count = dirdocs.GetCount() + mergedocs.GetCount();
	if (count > 1)
	{
		// Check that we don't have one empty dirdoc + mergedoc situation.
		// That happens since we open "hidden" dirdoc for every file compare.
		if (dirdocs.GetCount() == 1)
		{
			CDirDoc *pDoc = dirdocs.GetHead();
			if (!pDoc->HasDiffs())
				return true;
		}
		ret = LangMessageBox(IDS_CLOSEALL_WINDOWS, MB_YESNO | MB_ICONWARNING);
	}
	return (ret == IDYES);
}

/**
 * @brief Shows the release notes for user.
 * This function opens release notes HTML document into browser.
 */
void CMainFrame::OnHelpReleasenotes()
{
	String sPath = paths_ConcatPath(env_GetProgPath(), RelNotes);
	ShellExecute(NULL, _T("open"), sPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

/**
 * @brief Shows the translations page.
 * This function opens translations page URL into browser.
 */
void CMainFrame::OnHelpTranslations()
{
	ShellExecute(NULL, _T("open"), TranslationsUrl, NULL, NULL, SW_SHOWNORMAL);
}

/**
 * @brief Called when user selects File/Open Conflict...
 */
void CMainFrame::OnFileOpenConflict()
{
	String conflictFile;
	if (SelectFile(GetSafeHwnd(), conflictFile))
	{
		DoOpenConflict(conflictFile);
	}
}

/**
 * @brief Select and open conflict file for resolving.
 * This function lets user to select conflict file to resolve.
 * Then we parse conflict file to two files to "merge" and
 * save resulting file over original file.
 *
 * Set left-side file read-only as it is the repository file which cannot
 * be modified anyway. Right-side file is user's file which is set as
 * modified by default so user can just save it and accept workspace
 * file as resolved file.
 * @param [in] conflictFile Full path to conflict file to open.
 * @param [in] checked If true, do not check if it really is project file.
 * @return TRUE if conflict file was opened for resolving.
 */
BOOL CMainFrame::DoOpenConflict(const String& conflictFile, bool checked)
{
	BOOL conflictCompared = FALSE;

	if (!checked)
	{
		bool confFile = IsConflictFile(conflictFile);
		if (!confFile)
		{
			String message = string_format_string1(_("The file\n%1\nis not a conflict file."), conflictFile);
			AfxMessageBox(message.c_str(), MB_ICONSTOP);
			return FALSE;
		}
	}

	// Create temp files and put them into the list,
	// from where they get deleted when MainFrame is deleted.
	String ext = paths_FindExtension(conflictFile);
	TempFilePtr wTemp(new TempFile());
	String workFile = wTemp->Create(_T("confw_"), ext.c_str());
	m_tempFiles.push_back(wTemp);
	TempFilePtr vTemp(new TempFile());
	String revFile = vTemp->Create(_T("confv_"), ext.c_str());
	m_tempFiles.push_back(vTemp);

	// Parse conflict file into two files.
	bool inners;
	int iGuessEncodingType = GetOptionsMgr()->GetInt(OPT_CP_DETECT);
	bool success = ParseConflictFile(conflictFile, workFile, revFile, iGuessEncodingType, inners);

	if (success)
	{
		// Open two parsed files to WinMerge, telling WinMerge to
		// save over original file (given as third filename).
		m_strSaveAsPath = conflictFile.c_str();
		String theirs = _("Theirs File");
		String my = _("Mine File");
		m_strDescriptions[0] = theirs;
		m_strDescriptions[1] = my;

		DWORD dwFlags[2] = {FFILEOPEN_READONLY | FFILEOPEN_NOMRU, FFILEOPEN_NOMRU | FFILEOPEN_MODIFIED};
		conflictCompared = DoFileOpen(&PathContext(revFile, workFile), 
					dwFlags);
	}
	else
	{
		LangMessageBox(IDS_ERROR_CONF_RESOLVE, MB_ICONSTOP);
	}
	return conflictCompared;
}

/**
 * @brief Get type of frame (File/Folder compare).
 * @param [in] pFrame Pointer to frame to check.
 * @return FRAMETYPE of the given frame.
*/
FRAMETYPE CMainFrame::GetFrameType(const CFrameWnd * pFrame) const
{
	BOOL bMergeFrame = pFrame->IsKindOf(RUNTIME_CLASS(CChildFrame));
	BOOL bDirFrame = pFrame->IsKindOf(RUNTIME_CLASS(CDirFrame));

	if (bMergeFrame)
		return FRAME_FILE;
	else if (bDirFrame)
		return FRAME_FOLDER;
	else
		return FRAME_OTHER;
}

/**
 * @brief Show the plugins list dialog.
 */
void CMainFrame::OnPluginsList()
{
	PluginsListDlg dlg;
	dlg.DoModal();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DRAWITEM:
	case WM_INITMENUPOPUP:
	case WM_MEASUREITEM:
	case WM_MENUCHAR:
		{
			if (GetOptionsMgr()->GetBool(OPT_DIRVIEW_ENABLE_SHELL_CONTEXT_MENU))
			{
				// in case of folder comparison we need to pass these messages to shell context menu
				CFrameWnd * pFrame = GetActiveFrame();
				FRAMETYPE frame = GetFrameType(pFrame);
				if (frame == FRAME_FOLDER)
				{
					CDirDoc * pDoc = (CDirDoc*)pFrame->GetActiveDocument();
					CDirView *pView = pDoc->GetMainView();
					pView->HandleMenuMessage(message, wParam, lParam);
				}
			}
		}
	}

	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}
