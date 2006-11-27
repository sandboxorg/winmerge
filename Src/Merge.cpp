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
 * @file  Merge.cpp
 *
 * @brief Defines the class behaviors for the application.
 *
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#include "stdafx.h"
#include "Merge.h"

#include "AboutDlg.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DirFrame.h"
#include "MergeDoc.h"
#include "DirDoc.h"
#include "DirView.h"
#include "Splash.h"
#include "logfile.h"
#include "coretools.h"
#include "paths.h"
#include "sinstance.h"
#include "FileFilterHelper.h"
#include "Plugins.h"
#include "DirScan.h" // for DirScan_InitializeDefaultCodepage
#include "ProjectFile.h"
#include "MergeEditView.h"
#include "LanguageSelect.h"
#include "OptionsDef.h"
#include "MergeCmdLineInfo.h"

// For shutdown cleanup
#include "charsets.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// VC 6 headers don't define these constants for folder browse dialog
// so define them here. Copied from shlobj.h
#ifndef BIF_EDITBOX
#define BIF_EDITBOX            0x0010   // Add an editbox to the dialog
#endif
#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE     0x0040   // Use the new dialog layout with the ability to resize
                                        // Caller needs to call OleInitialize() before using this API
#endif
#ifndef BIF_USENEWUI
#define BIF_USENEWUI           (BIF_NEWDIALOGSTYLE | BIF_EDITBOX)
#endif


/////////////////////////////////////////////////////////////////////////////
// CMergeApp

BEGIN_MESSAGE_MAP(CMergeApp, CWinApp)
	//{{AFX_MSG_MAP(CMergeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VIEW_LANGUAGE, OnViewLanguage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LANGUAGE, OnUpdateViewLanguage)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

extern CLogFile gLog;

static void AddEnglishResourceHook();

/**
* @brief Mapping from command line argument name (eg, ignorews) to WinMerge
* option name (eg, Settings/IgnoreSpace).
*
* These arguments take an optional colon and number, like so:
*
*  "/ignoreblanklines"  (makes WinMerge ignore blank lines)
*  "/ignoreblanklines:1"  (makes WinMerge ignore blank lines)
*  "/ignoreblanklines:0"  (makes WinMerge not ignore blank lines)
*/
struct ArgSetting
{
	LPCTSTR CmdArgName;
	LPCTSTR WinMergeOptionName;
};

/////////////////////////////////////////////////////////////////////////////
// CMergeApp construction

CMergeApp::CMergeApp() :
  m_bNeedIdleTimer(FALSE)
, m_pDiffTemplate(0)
, m_pDirTemplate(0)
, m_mainThreadScripts(NULL)
, m_nLastCompareResult(0)
, m_bNonInteractive(false)
{
	// add construction code here,
	// Place all significant initialization in InitInstance
	m_pLangDlg = new CLanguageSelect(IDR_MAINFRAME, IDR_MAINFRAME);
}

CMergeApp::~CMergeApp()
{
	delete m_pLangDlg; m_pLangDlg = NULL;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CMergeApp object

CMergeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMergeApp initialization

BOOL CMergeApp::InitInstance()
{
	InitCommonControls();    // initialize common control library
	CWinApp::InitInstance(); // call parent class method

	// Runtime switch so programmer may set this in interactive debugger
	int dbgmem = 0;
	if (dbgmem)
	{
		// get current setting
		int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
		// Keep freed memory blocks in the heap's linked list and mark them as freed
		tmpFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
		// Call _CrtCheckMemory at every allocation and deallocation request.
		// WARNING: This slows down WinMerge *A LOT*
		tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
		// Set the new state for the flag
		_CrtSetDbgFlag( tmpFlag );
	}

	// CCrystalEdit Drag and Drop functionality needs AfxOleInit.
	if(!AfxOleInit())
	{
		TRACE(_T("AfxOleInitFailed. OLE functionality disabled"));
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Only needed by VC6
#if _MSC_VER < 1300
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Parse command-line arguments.
	MergeCmdLineInfo cmdInfo(*__targv);
	ParseCommandLine(cmdInfo);

	// If paths were given to commandline we consider this being an invoke from
	// commandline (from other application, shellextension etc).
	BOOL bCommandLineInvoke = FALSE;
	if (cmdInfo.m_nFiles > 0)
		bCommandLineInvoke = TRUE;

	// Set default codepage
	DirScan_InitializeDefaultCodepage();

	// WinMerge registry settings are stored under HKEY_CURRENT_USER/Software/Thingamahoochie
	// This is the name of the company of the original author (Dean Grimm)
	SetRegistryKey(_T("Thingamahoochie"));

	BOOL bSingleInstance = GetProfileInt(_T("Settings"), _T("SingleInstance"), FALSE) ||
		(true == cmdInfo.m_bSingleInstance);
	
	HANDLE hMutex = NULL;
	if (bSingleInstance)
	{
		hMutex = CreateMutex(NULL, FALSE, _T("WinMerge{05963771-8B2E-11d8-B3B9-000000000000}"));
		WaitForSingleObject(hMutex, INFINITE);
	}

	CInstanceChecker instanceChecker(_T("{05963771-8B2E-11d8-B3B9-000000000000}"));
	if (bSingleInstance)
	{
		if (instanceChecker.PreviousInstanceRunning())
		{
			USES_CONVERSION;

			// Activate previous instance and send commandline to it
			HWND hWnd = instanceChecker.ActivatePreviousInstance();
			
			WCHAR *pszArgs = new WCHAR[_tcslen(__targv[0]) + _tcslen(m_lpCmdLine) + 3];
			WCHAR *p = pszArgs;
			for (int i = 0; i < __argc; i++)
			{
				wcscpy(p, T2W(__targv[i]));
				p += lstrlenW(p) + 1;
			}
			*p++ = _T('\0');
			COPYDATASTRUCT data = {0};
			data.cbData = (DWORD)(p - pszArgs) * sizeof(WCHAR);
			data.lpData = pszArgs;
			data.dwData = __argc;
			SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)&data);
			delete[] pszArgs;

			ReleaseMutex(hMutex);
			CloseHandle(hMutex);

			return FALSE;
		}
	}

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	BOOL bDisableSplash	= GetProfileInt(_T("Settings"), _T("DisableSplash"), FALSE);

	InitializeFileFilters();
	m_globalFileFilter.SetFilter(_T("*.*"));

	CSplashWnd::EnableSplashScreen(bDisableSplash==FALSE && cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew);

	// Initialize i18n (multiple language) support

	m_pLangDlg->SetLogFile(&gLog);
	m_pLangDlg->InitializeLanguage();

	AddEnglishResourceHook(); // Use English string when l10n (foreign) string missing

	m_mainThreadScripts = new CAssureScriptsForThread;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	// Merge Edit view
	m_pDiffTemplate = new CMultiDocTemplate(
		IDR_MERGEDOCTYPE,
		RUNTIME_CLASS(CMergeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMergeEditView));
	AddDocTemplate(m_pDiffTemplate);

	// Directory view
	m_pDirTemplate = new CMultiDocTemplate(
		IDR_DIRDOCTYPE,
		RUNTIME_CLASS(CDirDoc),
		RUNTIME_CLASS(CDirFrame), // custom MDI child frame
		RUNTIME_CLASS(CDirView));
	AddDocTemplate(m_pDirTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		if (hMutex)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
		}
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// Enable drag&drop files
	pMainFrame->ModifyStyleEx(NULL, WS_EX_ACCEPTFILES);

	// Init menus -- hMenuDefault is for MainFrame, other
	// two are for dirdoc and mergedoc (commented out for now)
	m_pDiffTemplate->m_hMenuShared = pMainFrame->NewMergeViewMenu();
	m_pDirTemplate->m_hMenuShared = pMainFrame->NewDirViewMenu();
	pMainFrame->m_hMenuDefault = pMainFrame->NewDefaultMenu();

	// Set the menu
	// Note : for Windows98 compatibility, use FromHandle and not Attach/Detach
	CMenu * pNewMenu = CMenu::FromHandle(pMainFrame->m_hMenuDefault);
	pMainFrame->MDISetMenu(pNewMenu, NULL);

	//Track it so any other instances can find it.
	instanceChecker.TrackFirstInstanceRunning();

	// The main window has been initialized, so activate and update it.
	pMainFrame->ActivateFrame(cmdInfo.m_nCmdShow);
	pMainFrame->UpdateWindow();

	// Since this function actually opens paths for compare it must be
	// called after initializing CMainFrame!
	BOOL bContinue = TRUE;
	if (ParseArgsAndDoOpen(cmdInfo, pMainFrame) == FALSE && bCommandLineInvoke)
		bContinue = FALSE;

	if (hMutex)
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}

	if (m_bNonInteractive)
	{
		DirViewList DirViews;
		pMainFrame->GetDirViews(&DirViews);
		if (DirViews.GetCount() == 1)
		{
			CDirView *pDirView = DirViews.RemoveHead();
			CDirFrame *pf = pDirView->GetParentFrame();
		}
		bContinue = FALSE;
	}

	// If user wants to cancel the compare, close WinMerge
	if (bContinue == FALSE)
	{
		pMainFrame->PostMessage(WM_CLOSE, 0, 0);
	}

	return bContinue;
}

// App command to run the dialog
void CMergeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMergeApp commands


BOOL CMergeApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

void CMergeApp::OnViewLanguage() 
{
	if (m_pLangDlg->DoModal()==IDOK)
	{
		//m_lang.ReloadMenu();
		//m_LangDlg.UpdateDocTitle();
		GetMainFrame()->UpdateResources();
	}
}

/**
 * @brief Updates Language select menu item.
 * If there are no languages installed we disable menuitem to
 * open language selection dialog.
 */
void CMergeApp::OnUpdateViewLanguage(CCmdUI* pCmdUI)
{
	BOOL bLangsInstalled = m_pLangDlg->AreLangsInstalled();
	pCmdUI->Enable(bLangsInstalled);
}

/**
 * @brief Called when application is about to exit.
 * This functions is called when application is exiting, so this is
 * good place to do cleanups.
 * @return Application's exit value (returned from WinMain()).
 */
int CMergeApp::ExitInstance() 
{
	charsets_cleanup();
	delete m_mainThreadScripts;
	CWinApp::ExitInstance();
	return m_nLastCompareResult;
}

static void AddEnglishResourceHook()
{
	// After calling AfxSetResourceHandle to point to a language
	// resource DLL, then the application is no longer on the
	// resource lookup (defined by AfxFindResourceHandle).
	
	// Add a dummy extension DLL record whose resource handle
	// points to the application resources, just to provide
	// fallback to English for any resources missing from 
	// the language resource DLL.
	
	// (Why didn't Microsoft think of this? Bruno Haible who
	// made gettext certainly thought of this.)

	// NB: This does not fix the problem that if a control is
	// missing from a dialog (because it was added only to the
	// English version, for example) then the DDX_ function is
	// going to fail. I see no easy way to intercept all DDX
	// functions except by macro overriding the call--Perry, 2002-12-07.

	static AFX_EXTENSION_MODULE FakeEnglishDLL = { NULL, NULL };
	memset(&FakeEnglishDLL, 0, sizeof(FakeEnglishDLL));
	FakeEnglishDLL.hModule = AfxGetApp()->m_hInstance;
	FakeEnglishDLL.hResource = FakeEnglishDLL.hModule;
	FakeEnglishDLL.bInitialized = TRUE;
	new CDynLinkLibrary(FakeEnglishDLL); // hook into MFC extension DLL chain
}


int CMergeApp::DoMessageBox( LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt )
{
	// This is a convenient point for breakpointing !!!

	// Create a handle to store the parent window of the message box.
	CWnd* pParentWnd = CWnd::GetActiveWindow();
	
	// Check whether an active window was retrieved successfully.
	if ( pParentWnd == NULL )
	{
		// Try to retrieve a handle to the last active popup.
		CWnd * mainwnd = GetMainWnd();
		if (mainwnd)
			pParentWnd = mainwnd->GetLastActivePopup();
	}

	// Use our own message box implementation, which adds the
	// do not show again checkbox, and implements it on subsequent calls
	// (if caller set the style)

	if (m_bNonInteractive)
		return IDCANCEL;

	// Create the message box dialog.
	CMessageBoxDialog dlgMessage(pParentWnd, lpszPrompt, _T(""), nType,
		nIDPrompt);
	
	// Display the message box dialog and return the result.
	return dlgMessage.DoModal();
}

/** 
 * @brief Set flag so that application will broadcast notification at next
 * idle time (via WM_TIMER id=IDLE_TIMER)
 */
void CMergeApp::SetNeedIdleTimer()
{
	m_bNeedIdleTimer = TRUE; 
}

BOOL CMergeApp::OnIdle(LONG lCount) 
{
	if (CWinApp::OnIdle(lCount))
		return TRUE;

	// If anyone has requested notification when next idle occurs, send it
	if (m_bNeedIdleTimer)
	{
		m_bNeedIdleTimer = FALSE;
		m_pMainWnd->SendMessageToDescendants(WM_TIMER, IDLE_TIMER, lCount, TRUE, FALSE);
	}
	return FALSE;
}

/**
 * @brief Load any known file filters.
 *
 * This function loads filter files from paths we know contain them.
 * @note User's filter location may not be set yet.
 */
void CMergeApp::InitializeFileFilters()
{
	CString filterPath = GetProfileString(_T("Settings"), _T("UserFilterPath"), _T(""));

	if (!filterPath.IsEmpty())
	{
		m_globalFileFilter.SetUserFilterPath(filterPath);
	}
	m_globalFileFilter.LoadAllFileFilters();
}

/** @brief Read command line arguments and open files for comparison.
 *
 * The name of the function is a legacy code from the time that this function
 * actually parsed the command line. Today the parsing is done using the
 * MergeCmdLineInfo class.
 * @param [in] cmdInfo Commandline parameters info.
 * @param [in] pMainFrame Pointer to application main frame.
 * @return TRUE if we opened the compare, FALSE if the compare was canceled.
 */
BOOL CMergeApp::ParseArgsAndDoOpen(MergeCmdLineInfo& cmdInfo, CMainFrame* pMainFrame)
{
	BOOL bCompared = FALSE;
	m_bNonInteractive = cmdInfo.m_bNonInteractive;

	SetOptionsFromCmdLine(cmdInfo);

	// Do not load or remember options (preferences).
	if (cmdInfo.m_bNoPrefs)
	{
		// Turn off serializing to registry.
		GetOptionsMgr()->SetSerializing(false);
		// Load all default settings.
		pMainFrame->ResetOptions();
	}

	// Set the global file filter.
	if (!cmdInfo.m_sFileFilter.IsEmpty())
	{
		m_globalFileFilter.SetFilter(cmdInfo.m_sFileFilter);
	}

	// Unless the user has requested to see WinMerge's usage open files for
	// comparison.
	if (cmdInfo.m_bShowUsage)
	{
		CString s = GetUsageDescription();
		AfxMessageBox(s, MB_ICONINFORMATION);
	}
	else
	{
		// Set the required information we need from the command line:

		pMainFrame->m_bClearCaseTool = cmdInfo.m_bClearCaseTool;
		pMainFrame->m_bExitIfNoDiff = cmdInfo.m_bExitIfNoDiff;
		pMainFrame->m_bEscShutdown = cmdInfo.m_bEscShutdown;

		pMainFrame->m_strSaveAsPath = _T("");

		pMainFrame->m_strLeftDesc = cmdInfo.m_sLeftDesc;
		pMainFrame->m_strRightDesc = cmdInfo.m_sRightDesc;

		if (cmdInfo.m_nFiles > 2)
		{
			pMainFrame->m_strSaveAsPath = cmdInfo.m_Files[2];
			bCompared = pMainFrame->DoFileOpen(cmdInfo.m_Files[0],
				cmdInfo.m_Files[1],	cmdInfo.m_dwLeftFlags,
				cmdInfo.m_dwRightFlags, cmdInfo.m_bRecurse, NULL,
				cmdInfo.m_sPreDiffer);
		}
		else if (cmdInfo.m_nFiles > 1)
		{
			cmdInfo.m_dwLeftFlags |= FFILEOPEN_CMDLINE;
			cmdInfo.m_dwRightFlags |= FFILEOPEN_CMDLINE;
			bCompared = pMainFrame->DoFileOpen(cmdInfo.m_Files[0],
				cmdInfo.m_Files[1],	cmdInfo.m_dwLeftFlags,
				cmdInfo.m_dwRightFlags, cmdInfo.m_bRecurse, NULL,
				cmdInfo.m_sPreDiffer);
		}
		else if (cmdInfo.m_nFiles == 1)
		{
			CString sFilepath = cmdInfo.m_Files[0];
			if (IsProjectFile(sFilepath))
			{
				bCompared = LoadAndOpenProjectFile(sFilepath);
			}
			else
			{
				cmdInfo.m_dwRightFlags = FFILEOPEN_NONE;
				bCompared = pMainFrame->DoFileOpen(sFilepath, _T(""),
					cmdInfo.m_dwLeftFlags, cmdInfo.m_dwRightFlags,
					cmdInfo.m_bRecurse, NULL, cmdInfo.m_sPreDiffer);
			}
		}
	}
	return bCompared;
}

/** @brief Handle all command line arguments which are mapped to WinMerge options. */
void CMergeApp::SetOptionsFromCmdLine(const MergeCmdLineInfo& cmdInfo)
{
	static const ArgSetting f_ArgSettings[] = 
	{
		{ _T("ignorews"), OPT_CMP_IGNORE_WHITESPACE },
		{ _T("ignoreblanklines"), OPT_CMP_IGNORE_BLANKLINES },
		{ _T("ignorecase"), OPT_CMP_IGNORE_CASE },
		{ _T("ignoreeol"), OPT_CMP_IGNORE_EOL }
	};

	for (int i = 0; i < countof(f_ArgSettings); ++i)
	{
		const ArgSetting& argSetting = f_ArgSettings[i];
		LPCTSTR szCmdArgName = argSetting.CmdArgName;
		LPCTSTR szOptionName = argSetting.WinMergeOptionName;
		CString	sValue;

		if (cmdInfo.m_Settings.Lookup(szCmdArgName, sValue))
		{
			GetOptionsMgr()->CoerceAndSaveOption(szOptionName, sValue);
		}
	}
}

/** @brief Open help from mainframe when user presses F1*/
void CMergeApp::OnHelp()
{
	GetMainFrame()->ShowHelp();
}


/**
 * @brief Is specified file a project file?
 * @param [in] filepath Full path to file to check.
 * @return true if file is a projectfile.
 */
bool CMergeApp::IsProjectFile(const CString & filepath) const
{
	CString sExt;
	SplitFilename(filepath, NULL, NULL, &sExt);
	if (sExt.CompareNoCase(PROJECTFILE_EXT) == 0)
		return true;
	else
		return false;
}

/** 
 * @brief Read project and perform comparison specified
 * @param [in] sProject Full path to project file.
 * @return TRUE if loading project file and starting compare succeeded.
 */
bool CMergeApp::LoadAndOpenProjectFile(const CString & sProject)
{
	if (sProject.IsEmpty())
		return false;

	ProjectFile project;
	CString sErr;
	if (!project.Read(sProject, &sErr))
	{
		if (sErr.IsEmpty())
			sErr = LoadResString(IDS_UNK_ERROR_READING_PROJECT);
		CString msg;
		AfxFormatString2(msg, IDS_ERROR_FILEOPEN, sProject, sErr);
		AfxMessageBox(msg, MB_ICONSTOP);
		return false;
	}
	CString sLeft, sRight;
	BOOL bLeftReadOnly = FALSE;
	BOOL bRightReadOnly = FALSE;
	BOOL bRecursive = FALSE;
	sLeft = project.GetLeft(&bLeftReadOnly);
	sRight = project.GetRight(&bRightReadOnly);
	if (project.HasFilter())
	{
		CString filter = project.GetFilter();
		filter.TrimLeft();
		filter.TrimRight();
		m_globalFileFilter.SetFilter(filter);
	}
	if (project.HasSubfolders())
		bRecursive = project.GetSubfolders() > 0;

	DWORD dwLeftFlags = FFILEOPEN_NONE;
	DWORD dwRightFlags = FFILEOPEN_NONE;
	if (!sLeft.IsEmpty())
	{	
		dwLeftFlags = FFILEOPEN_PROJECT;
		if (bLeftReadOnly)
			dwLeftFlags |= FFILEOPEN_READONLY;
	}
	if (!sRight.IsEmpty())
	{	
		dwRightFlags = FFILEOPEN_PROJECT;
		if (bRightReadOnly)
			dwRightFlags |= FFILEOPEN_READONLY;
	}

	WriteProfileInt(_T("Settings"), _T("Recurse"), bRecursive);

	BOOL rtn = GetMainFrame()->DoFileOpen(sLeft, sRight, dwLeftFlags, dwRightFlags, bRecursive);
	return !!rtn;
}

/**
 * @brief Return windows language ID of current WinMerge GUI language
 */
WORD CMergeApp::GetLangId() const
{
	return m_pLangDlg->GetLangId();
}

/**
 * @brief Reload main menu(s) (for language change)
 */
void CMergeApp::ReloadMenu()
{
	m_pLangDlg->ReloadMenu();
}

/** @brief Wrap one line of cmdline help in appropriate whitespace */
static CString CmdlineOption(int idres)
{
	CString str = LoadResString(idres) + _T(" \n");
	return str;
}

/** @brief Put together string of all cmdline arguments */
CString CMergeApp::GetUsageDescription()
{
	CString str;
	str += LoadResString(IDS_CMDLINE_SYNTAX);
	str += _T(" [/f ") + LoadResString(IDS_CMDLINE_SYNTAX_ARG_FILTER) + _T("]");
	str += _T(" ");
	str += LoadResString(IDS_CMDLINE_SYNTAX_ARGS);
	str += _T("\n\n") + LoadResString(IDS_CMDLINE_WHERE) + _T(" \n");
	str += CmdlineOption(IDS_CMDLINE_HELP);
	str += CmdlineOption(IDS_CMDLINE_RECURSIVE);
	str += CmdlineOption(IDS_CMDLINE_ESCKEY);
	str += CmdlineOption(IDS_CMDLINE_FILEMASK);
	str += CmdlineOption(IDS_CMDLINE_FASTCLOSE);
	str += CmdlineOption(IDS_CMDLINE_SINGLE_INST);
	str += CmdlineOption(IDS_CMDLINE_LEFTPATH);
	str += CmdlineOption(IDS_CMDLINE_RIGHTPATH);
	str += CmdlineOption(IDS_CMDLINE_OUTPUTPATH);
	str += _T("\n\n") + LoadResString(IDS_CMDLINE_SEEMANUAL);
	return str;
}
