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
 * @file  MainFrm.h
 *
 * @brief Declaration file for CMainFrame
 *
 */
// ID line follows -- this is updated by SVN
// $Id: MainFrm.h 6940 2009-08-01 17:29:01Z kimmov $

#if !defined(AFX_MAINFRM_H__BBCD4F8C_34E4_11D1_BAA6_00A024706EDC__INCLUDED_)
#define AFX_MAINFRM_H__BBCD4F8C_34E4_11D1_BAA6_00A024706EDC__INCLUDED_

#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "ToolBarXPThemes.h"
#include "MDITabBar.h"
#include "PathContext.h"

class BCMenu;
class CDiffView;
class CDirView;
class COpenDoc;
class CDirDoc;
class CMergeDoc;
class CHexMergeDoc;
class CMergeEditView;
class SyntaxColors;
class LineFiltersList;
class TempFile;
struct FileLocation;
class SourceControl;

typedef boost::shared_ptr<TempFile> TempFilePtr;

// typed lists (homogenous pointer lists)
typedef CTypedPtrList<CPtrList, COpenDoc *> OpenDocList;
typedef CTypedPtrList<CPtrList, CMergeDoc *> MergeDocList;
typedef CTypedPtrList<CPtrList, CDirDoc *> DirDocList;
typedef CTypedPtrList<CPtrList, CHexMergeDoc *> HexMergeDocList;

class PackingInfo;
class CLanguageSelect;

/**
 * @brief Frame class containing save-routines etc
 */
class CMainFrame : public CMDIFrameWnd
{
	friend CLanguageSelect;
	DECLARE_DYNAMIC(CMainFrame)
public:
	/**
	 * @brief Frame/View/Document types.
	 */
	enum FRAMETYPE
	{
		FRAME_FOLDER, /**< Folder compare frame. */
		FRAME_FILE, /**< File compare frame. */
		FRAME_HEXFILE, /**< Hex file compare frame. */
		FRAME_IMGFILE, /**< Image file compare frame. */
		FRAME_OTHER, /**< No frame? */
	};

	enum { WM_NONINTERACTIVE = 888 }; // timer value

	CMainFrame();

// Attributes
public:	
	BOOL m_bShowErrors; /**< Show folder compare error items? */
	LOGFONT m_lfDiff; /**< MergeView user-selected font */
	LOGFONT m_lfDir; /**< DirView user-selected font */
	static const TCHAR szClassName[];
// Operations
public:
	HMENU NewDirViewMenu();
	HMENU NewMergeViewMenu();
	HMENU NewHexMergeViewMenu();
	HMENU NewImgMergeViewMenu();
	HMENU NewOpenViewMenu();
	HMENU NewDefaultMenu(int ID = 0);
	HMENU GetScriptsSubmenu(HMENU mainMenu);
	HMENU GetPrediffersSubmenu(HMENU mainMenu);
	void UpdatePrediffersMenu();

	BOOL DoFileOpen(const PathContext *pFiles = NULL,
		const DWORD dwFlags[] = NULL, bool bRecurse = false, CDirDoc *pDirDoc = NULL, String prediffer = _T(""), const PackingInfo * infoUnpacker = NULL);
	int ShowAutoMergeDoc(CDirDoc * pDirDoc, int nFiles, const FileLocation fileloc[],
		const DWORD dwFlags[], const PackingInfo * infoUnpacker = NULL);
	int ShowMergeDoc(CDirDoc * pDirDoc, int nFiles, const FileLocation fileloc[],
		const DWORD dwFlags[], const PackingInfo * infoUnpacker = NULL);
	void ShowHexMergeDoc(CDirDoc * pDirDoc,
		const PathContext &paths, const bool bRO[]);
	int ShowImgMergeDoc(CDirDoc * pDirDoc, int nFiles, const FileLocation fileloc[],
		const DWORD dwFlags[], const PackingInfo * infoUnpacker = NULL);

	void UpdateResources();
	CString SetStatus(LPCTSTR status);
	void ClearStatusbarItemCount();
	void ApplyDiffOptions();
	void ApplyViewWhitespace();
	void SetEOLMixed(BOOL bAllow);
	void SelectFilter();
	void StartFlashing();
	bool AskCloseConfirmation();
	BOOL DoOpenConflict(const String& conflictFile, bool checked = false);
	FRAMETYPE GetFrameType(const CFrameWnd * pFrame) const;

// Overrides
	virtual void GetMessageString(UINT nID, CString& rMessage) const;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation methods
protected:
	virtual ~CMainFrame();


// Public implementation data
public:
	BOOL m_bFirstTime; /**< If first time frame activated, get  pos from reg */
	BOOL m_bFlashing; /**< Window is flashing. */

	/** @brief Possible toolbar image sizes. */
	enum TOOLBAR_SIZE
	{
		TOOLBAR_SIZE_16x16,
		TOOLBAR_SIZE_32x32,
	};

// Implementation data
protected:


	// control bar embedded members
	CStatusBar  m_wndStatusBar;
	CReBar m_wndReBar;
	ToolBarXPThemes m_wndToolBar;
	CMDITabBar m_wndTabBar;

	/** @brief Toolbar image table indexes. */
	enum TOOLBAR_IMAGES
	{
		TOOLBAR_IMAGES_ENABLED,
		TOOLBAR_IMAGES_DISABLED,
		TOOLBAR_IMAGES_COUNT
	};

	CImageList m_ToolbarImages[TOOLBAR_IMAGES_COUNT]; /**< Images for toolbar */

	enum
	{
		MENU_DEFAULT,
		MENU_MERGEVIEW,
		MENU_DIRVIEW,
		MENU_HEXMERGEVIEW,
		MENU_IMGMERGEVIEW,
		MENU_OPENVIEW,
		MENU_COUNT, // Add new items before this item
	};
	/**
	 * Menu frames - for which frame(s) the menu is.
	 */
	enum
	{
		MENU_MAINFRM = 0x000001,
		MENU_FILECMP = 0x000002,
		MENU_FOLDERCMP = 0x000004,
		MENU_ALL = MENU_MAINFRM | MENU_FILECMP | MENU_FOLDERCMP
	};

	/**
	 * A structure attaching a menu item, icon and menu types to apply to.
	 */
	struct MENUITEM_ICON
	{
		int menuitemID;   /**< Menu item's ID. */
		int iconResID;    /**< Icon's resource ID. */
		int menusToApply; /**< For which menus to apply. */
	};

	static const MENUITEM_ICON m_MenuIcons[];

	boost::scoped_ptr<BCMenu> m_pMenus[MENU_COUNT]; /**< Menus for different views */
	std::vector<TempFilePtr> m_tempFiles; /**< List of possibly needed temp files. */

// Generated message map functions
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) ;
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnOptionsShowDifferent();
	afx_msg void OnOptionsShowIdentical();
	afx_msg void OnOptionsShowUniqueLeft();
	afx_msg void OnOptionsShowUniqueRight();
	afx_msg void OnOptionsShowBinaries();
	afx_msg void OnOptionsShowSkipped();
	afx_msg void OnUpdateOptionsShowdifferent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsShowidentical(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsShowuniqueleft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsShowuniqueright(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsShowBinaries(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsShowSkipped(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnHelpGnulicense();
	afx_msg void OnOptions();
	afx_msg void OnViewSelectfont();
	afx_msg void OnUpdateViewSelectfont(CCmdUI* pCmdUI);
	afx_msg void OnViewUsedefaultfont();
	afx_msg void OnUpdateViewUsedefaultfont(CCmdUI* pCmdUI);
	afx_msg void OnHelpContents();
	afx_msg void OnUpdateHelpContents(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnViewWhitespace();
	afx_msg void OnUpdateViewWhitespace(CCmdUI* pCmdUI);
	afx_msg void OnToolsGeneratePatch();
	afx_msg void OnDropFiles(HDROP dropInfo);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdatePluginUnpackMode(CCmdUI* pCmdUI);
	afx_msg void OnPluginUnpackMode(UINT nID);
	afx_msg void OnUpdatePluginPrediffMode(CCmdUI* pCmdUI);
	afx_msg void OnPluginPrediffMode(UINT nID);
	afx_msg void OnUpdateReloadPlugins(CCmdUI* pCmdUI);
	afx_msg void OnReloadPlugins();
	afx_msg void OnSaveConfigData();
	afx_msg void OnFileNew();
	afx_msg void OnFileNew3();
	afx_msg void OnToolsFilters();
	afx_msg void OnHelpMerge7zmismatch();
	afx_msg void OnUpdateHelpMerge7zmismatch(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewTabBar(CCmdUI* pCmdUI);
	afx_msg void OnViewTabBar();
	afx_msg void OnUpdateResizePanes(CCmdUI* pCmdUI);
	afx_msg void OnResizePanes();
	afx_msg void OnFileOpenproject();
	afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUser1(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnWindowCloseAll();
	afx_msg void OnUpdateWindowCloseAll(CCmdUI* pCmdUI);
	afx_msg void OnSaveProject();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
#if _MFC_VER > 0x0600
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
#else
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#endif
	afx_msg void OnToolbarNone();
	afx_msg void OnUpdateToolbarNone(CCmdUI* pCmdUI);
	afx_msg void OnToolbarSmall();
	afx_msg void OnUpdateToolbarSmall(CCmdUI* pCmdUI);
	afx_msg void OnToolbarBig();
	afx_msg void OnUpdateToolbarBig(CCmdUI* pCmdUI);
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHelpReleasenotes();
	afx_msg void OnHelpTranslations();
	afx_msg void OnFileOpenConflict();
	afx_msg void OnPluginsList();
	afx_msg void OnUpdatePluginName(CCmdUI* pCmdUI);
	afx_msg void OnDiffOptionsDropDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateDiffOptions(CCmdUI* pCmdUI);
	afx_msg void OnDiffWhitespace(UINT nID);
	afx_msg void OnUpdateDiffWhitespace(CCmdUI* pCmdUI);
	afx_msg void OnDiffCaseSensitive();
	afx_msg void OnUpdateDiffCaseSensitive(CCmdUI* pCmdUI);
	afx_msg void OnDiffIgnoreEOL();
	afx_msg void OnUpdateDiffIgnoreEOL(CCmdUI* pCmdUI);
	afx_msg void OnCompareMethod(UINT nID);
	afx_msg void OnUpdateCompareMethod(CCmdUI* pCmdUI);
	afx_msg void OnMRUs(UINT nID);
	afx_msg void OnUpdateNoMRUs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void addToMru(LPCTSTR szItem, LPCTSTR szRegSubKey, UINT nMaxItems = 20);
	void FileNew(int nPanes);
	const OpenDocList &GetAllOpenDocs();
	const MergeDocList &GetAllMergeDocs();
	const DirDocList &GetAllDirDocs();
	const HexMergeDocList &GetAllHexMergeDocs();
	void RedisplayAllDirDocs();
	CMergeDoc * GetMergeDocToShow(int nFiles, CDirDoc * pDirDoc, BOOL * pNew);
	CHexMergeDoc * GetHexMergeDocToShow(int nDirs, CDirDoc * pDirDoc, BOOL * pNew);
	CDirDoc * GetDirDocToShow(int nDirs, BOOL * pNew);
	void UpdateFont(FRAMETYPE frame);
	BOOL CreateToolbar();
	BOOL CreateComboBoxOnToolbar();
	CMergeEditView * GetActiveMergeEditView();
	void LoadToolbarImages();
	HMENU NewMenu( int view, int ID );
};

CMainFrame * GetMainFrame(); // access to the singleton main frame object

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BBCD4F8C_34E4_11D1_BAA6_00A024706EDC__INCLUDED_)
