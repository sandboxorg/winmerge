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
// DirView.cpp : implementation file
//

#include "stdafx.h"
#include "Merge.h"
#include "DirView.h"
#include "DirDoc.h"
#include "MainFrm.h"
#include "resource.h"
#include "coretools.h"
#include "WaitStatusCursor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirView



IMPLEMENT_DYNCREATE(CDirView, CListViewEx)

CDirView::CDirView()
{
	m_pList=NULL;
}

CDirView::~CDirView()
{
	 m_imageList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CDirView, CListViewEx)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CDirView)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_R2L, OnDirCopyFileToLeft)
	ON_UPDATE_COMMAND_UI(ID_R2L, OnUpdateDirCopyFileToLeft)
	ON_COMMAND(ID_DIR_COPY_FILE_TO_LEFT, OnCtxtDirCopyFileToLeft)
	ON_UPDATE_COMMAND_UI(ID_DIR_COPY_FILE_TO_LEFT, OnUpdateCtxtDirCopyFileToLeft)
	ON_COMMAND(ID_L2R, OnDirCopyFileToRight)
	ON_UPDATE_COMMAND_UI(ID_L2R, OnUpdateDirCopyFileToRight)
	ON_COMMAND(ID_DIR_COPY_FILE_TO_RIGHT, OnCtxtDirCopyFileToRight)
	ON_UPDATE_COMMAND_UI(ID_DIR_COPY_FILE_TO_RIGHT, OnUpdateCtxtDirCopyFileToRight)
	ON_COMMAND(ID_DIR_DEL_LEFT, OnCtxtDirDelLeft)
	ON_UPDATE_COMMAND_UI(ID_DIR_DEL_LEFT, OnUpdateCtxtDirDelLeft)
	ON_COMMAND(ID_DIR_DEL_RIGHT, OnCtxtDirDelRight)
	ON_UPDATE_COMMAND_UI(ID_DIR_DEL_RIGHT, OnUpdateCtxtDirDelRight)
	ON_COMMAND(ID_DIR_DEL_BOTH, OnCtxtDirDelBoth)
	ON_UPDATE_COMMAND_UI(ID_DIR_DEL_BOTH, OnUpdateCtxtDirDelBoth)
	ON_COMMAND(ID_DIR_OPEN_LEFT, OnCtxtDirOpenLeft)
	ON_UPDATE_COMMAND_UI(ID_DIR_OPEN_LEFT, OnUpdateCtxtDirOpenLeft)
	ON_COMMAND(ID_DIR_OPEN_LEFT_WITH, OnCtxtDirOpenLeftWith)
	ON_UPDATE_COMMAND_UI(ID_DIR_OPEN_LEFT_WITH, OnUpdateCtxtDirOpenLeftWith)
	ON_COMMAND(ID_DIR_OPEN_RIGHT, OnCtxtDirOpenRight)
	ON_UPDATE_COMMAND_UI(ID_DIR_OPEN_RIGHT, OnUpdateCtxtDirOpenRight)
	ON_COMMAND(ID_DIR_OPEN_RIGHT_WITH, OnCtxtDirOpenRightWith)
	ON_UPDATE_COMMAND_UI(ID_DIR_OPEN_RIGHT_WITH, OnUpdateCtxtDirOpenRightWith)
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_COMMAND(ID_FIRSTDIFF, OnFirstdiff)
	ON_UPDATE_COMMAND_UI(ID_FIRSTDIFF, OnUpdateFirstdiff)
	ON_COMMAND(ID_LASTDIFF, OnLastdiff)
	ON_UPDATE_COMMAND_UI(ID_LASTDIFF, OnUpdateLastdiff)
	ON_COMMAND(ID_NEXTDIFF, OnNextdiff)
	ON_UPDATE_COMMAND_UI(ID_NEXTDIFF, OnUpdateNextdiff)
	ON_COMMAND(ID_PREVDIFF, OnPrevdiff)
	ON_UPDATE_COMMAND_UI(ID_PREVDIFF, OnUpdatePrevdiff)
	ON_COMMAND(ID_CURDIFF, OnCurdiff)
	ON_UPDATE_COMMAND_UI(ID_CURDIFF, OnUpdateCurdiff)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateSave)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirView drawing

void CDirView::OnDraw(CDC* /*pDC*/)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDirView diagnostics

#ifdef _DEBUG
void CDirView::AssertValid() const
{
	CListViewEx::AssertValid();
}

void CDirView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}

CDirDoc* CDirView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDirDoc)));
	return (CDirDoc*)m_pDocument;
}
#endif //_DEBUG

CDiffContext * CDirView::GetDiffContext()
{
	return GetDocument()->m_pCtxt;
}

/////////////////////////////////////////////////////////////////////////////
// CDirView message handlers

void CDirView::OnInitialUpdate() 
{
	CListViewEx::OnInitialUpdate();
	m_sortColumn = -1;	// start up in no sorted order.
	m_pList = &GetListCtrl();
	GetDocument()->SetDirView(this);

    // Replace standard header with sort header
    if (HWND hWnd = ListView_GetHeader(m_pList->m_hWnd))
            m_ctlSortHeader.SubclassWindow(hWnd);
        
	int w;
	CString sKey;
	CString sFmt(_T("WDirHdr%d")), sSect(_T("DirView"));

	sKey.Format(sFmt, DV_NAME);
	w = max(10, theApp.GetProfileInt(sSect, sKey, 150));
	m_pList->InsertColumn(DV_NAME, _T("Filename"), LVCFMT_LEFT, w);

	sKey.Format(sFmt, DV_PATH);
	w = max(10, theApp.GetProfileInt(sSect, sKey, 200));
	m_pList->InsertColumn(DV_PATH, _T("Directory"), LVCFMT_LEFT, w);

	sKey.Format(sFmt, DV_STATUS);
	w = max(10, theApp.GetProfileInt(sSect, sKey, 250));
	m_pList->InsertColumn(DV_STATUS, _T("Comparison result"), LVCFMT_LEFT, w);

	sKey.Format(sFmt, DV_LTIME);
	w = max(10, theApp.GetProfileInt(sSect, sKey, 150));
	m_pList->InsertColumn(DV_LTIME, _T("Left Time"), LVCFMT_LEFT, w);

	sKey.Format(sFmt, DV_RTIME);
	w = max(10, theApp.GetProfileInt(sSect, sKey, 150));
	m_pList->InsertColumn(DV_RTIME, _T("Right Time"), LVCFMT_LEFT, w);

    // BSP - Create a column for the extension values
	sKey.Format(sFmt, DV_EXT);
	w = max(10, theApp.GetProfileInt(sSect, sKey, 150));
	m_pList->InsertColumn(DV_EXT, _T("Extension"), LVCFMT_LEFT, w);

	CBitmap bm;
	VERIFY (m_imageList.Create (16, 16, ILC_MASK, 0, 1));
	VERIFY (bm.LoadBitmap (IDB_LFILE));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_RFILE));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_NOTEQUAL));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_EQUAL));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_UNKNOWN));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_BINARYSAME));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_BINARYDIFF));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_LFOLDER));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	VERIFY (bm.LoadBitmap (IDB_RFOLDER));
	VERIFY (-1 != m_imageList.Add (&bm, RGB (255, 255, 255)));
	bm.Detach();
	m_pList->SetImageList (&m_imageList, LVSIL_SMALL);
	UpdateResources();

	//m_ctlSortHeader.SetSortImage(m_sortColumn, m_bSortAscending);
}

void CDirView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	WaitStatusCursor waitstatus(_T("Opening selection"));
	OpenSelection();
	CListViewEx::OnLButtonDblClk(nFlags, point);
}


void CDirView::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_DIRVIEW));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		// set the menu items with the proper directory names
		CString s, sl, sr;
		GetSelectedDirNames(sl, sr);

		// find non-child ancestor to use as menu parent
		CWnd* pWndPopupOwner = this;
		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		// TODO: It would be more efficient to set
		// all the popup items now with one traverse over selected items
		// instead of using updates, in which we make a traverse for every item
		// Perry, 2002-12-04
		// 


		// invoke context menu
		// this will invoke all the OnUpdate methods to enable/disable the individual items
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

// Make a string out of a number
// TODO: Ought to introduce commas every three digits, except this is locale-specific
// How to do this with locale sensitivity ?
CString
NumToStr(int n)
{
	CString s;
	s.Format(_T("%d"), n);
	return s;
}

// Change menu item by using string resource
// (Question: Why don't we just remove it from the menu resource entirely & do an Add here ?)
void CDirView::ModifyPopup(CMenu * pPopup, int nStringResource, int nMenuId, LPCTSTR szPath)
{
	CString s;
	AfxFormatString1(s, nStringResource, szPath);
	pPopup->ModifyMenu(nMenuId, MF_BYCOMMAND|MF_STRING, nMenuId, s);
}


// User chose (main menu) Copy from right to left
void CDirView::OnDirCopyFileToLeft() 
{
	DoCopyFileToLeft();
}
// User chose (main menu) Copy from left to right
void CDirView::OnDirCopyFileToRight() 
{
	DoCopyFileToRight();
}

// User chose (context men) Copy from right to left
void CDirView::OnCtxtDirCopyFileToLeft()
{
	DoCopyFileToLeft();
}
// User chose (context menu) Copy from left to right
void CDirView::OnCtxtDirCopyFileToRight()
{
	DoCopyFileToRight();
}

// Update context menu Copy Right to Left item
void CDirView::OnUpdateCtxtDirCopyFileToLeft(CCmdUI* pCmdUI) 
{
	DoUpdateDirCopyFileToLeft(pCmdUI, eContext);
}
// Update context menu Copy Left to Right item
void CDirView::OnUpdateCtxtDirCopyFileToRight(CCmdUI* pCmdUI) 
{
	DoUpdateDirCopyFileToRight(pCmdUI, eContext);
}

// Update main menu Copy Right to Left item
void CDirView::OnUpdateDirCopyFileToLeft(CCmdUI* pCmdUI) 
{
	DoUpdateDirCopyFileToLeft(pCmdUI, eMain);
}
// Update main menu Copy Left to Right item
void CDirView::OnUpdateDirCopyFileToRight(CCmdUI* pCmdUI) 
{
	DoUpdateDirCopyFileToRight(pCmdUI, eMain);
}

// Should Copy to Left be enabled or disabled ? (both main menu & context menu use this)
void CDirView::DoUpdateDirCopyFileToLeft(CCmdUI* pCmdUI, eMenuType menuType)
{
	int sel=-1;
	int legalcount=0, selcount=0;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (IsItemCopyableToLeft(di.code))
			++legalcount;
		++selcount;
	}
	pCmdUI->Enable(legalcount>0);
	if (menuType==eContext)
	{
		CString s;
		AfxFormatString2(s, IDS_COPY_TO_LEFT, NumToStr(legalcount), NumToStr(selcount));
		pCmdUI->SetText(s);
	}
}
// Should Copy to Right be enabled or disabled ? (both main menu & context menu use this)
void CDirView::DoUpdateDirCopyFileToRight(CCmdUI* pCmdUI, eMenuType menuType)
{
	int sel=-1;
	int legalcount=0, selcount=0;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (IsItemCopyableToRight(di.code))
			++legalcount;
		++selcount;
	}
	pCmdUI->Enable(legalcount>0);
	if (menuType==eContext)
	{
		CString s;
		AfxFormatString2(s, IDS_COPY_TO_RIGHT, NumToStr(legalcount), NumToStr(selcount));
		pCmdUI->SetText(s);
	}
}



void CDirView::UpdateResources()
{
	CString s;
	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT;

	VERIFY(s.LoadString(IDS_FILENAME_HEADER));
	lvc.pszText = (LPTSTR)((LPCTSTR)s);
	m_pList->SetColumn(DV_NAME, &lvc);
	VERIFY(s.LoadString(IDS_EXTENSION_HEADER));
	lvc.pszText = (LPTSTR)((LPCTSTR)s);
	m_pList->SetColumn(DV_EXT, &lvc);
	VERIFY(s.LoadString(IDS_DIR_HEADER));
	lvc.pszText = (LPTSTR)((LPCTSTR)s);
	m_pList->SetColumn(DV_PATH, &lvc);
	VERIFY(s.LoadString(IDS_RESULT_HEADER));
	lvc.pszText = (LPTSTR)((LPCTSTR)s);
	m_pList->SetColumn(DV_STATUS, &lvc);
	VERIFY(s.LoadString(IDS_LTIME_HEADER));
	lvc.pszText = (LPTSTR)((LPCTSTR)s);
	m_pList->SetColumn(DV_LTIME, &lvc);
	VERIFY(s.LoadString(IDS_RTIME_HEADER));
	lvc.pszText = (LPTSTR)((LPCTSTR)s);
	m_pList->SetColumn(DV_RTIME, &lvc);
}

int CALLBACK CDirView::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// initialize structures to obtain required information
	CDirView* pView = reinterpret_cast<CDirView*>(lParamSort);
	POSITION diffposl = pView->GetItemKeyFromData(lParam1);
	POSITION diffposr = pView->GetItemKeyFromData(lParam2);
	DIFFITEM lDi = pView->GetDiffContext()->GetDiffAt(diffposl);
	DIFFITEM rDi = pView->GetDiffContext()->GetDiffAt(diffposr);

	// compare 'left' and 'right' parameters as appropriate
	int retVal = 0;		// initialize for default case
	switch (pView->m_sortColumn)
	{
	case DV_NAME: // File name.
		retVal = lDi.sfilename.CompareNoCase(rDi.sfilename);
		break;
	case DV_PATH: // File Path.
		retVal =  lDi.slpath.CompareNoCase(rDi.slpath);
		break;
	case DV_STATUS: // Diff Status.
		retVal = rDi.code-lDi.code;
		break;
	case DV_LTIME: // Diff Status.
		retVal = rDi.ltime-lDi.ltime;
		break;
	case DV_RTIME: // Diff Status.
		retVal = lDi.rtime-rDi.rtime;
		break;
	case DV_EXT: // File extension.
		retVal = lDi.sext.CompareNoCase(rDi.sext);
		break;
	}
	// return compare result, considering sort direction
	return (pView->m_bSortAscending)?retVal:-retVal;
}

void CDirView::OnColumnClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// set sort parameters and handle ascending/descending
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
	if(pNMListView->iSubItem==m_sortColumn)
	{
		m_bSortAscending = !m_bSortAscending;
	}
	else
	{
		m_sortColumn = pNMListView->iSubItem;
		// date columns get default descending sort.
		if(m_sortColumn==3 || m_sortColumn==4)
		{
			m_bSortAscending = false;
		}
		else
		{
			m_bSortAscending = true;
		}
	}
	m_ctlSortHeader.SetSortImage(m_sortColumn, m_bSortAscending);

	//sort using static CompareFunc comparison function
	GetListCtrl ().SortItems (CompareFunc, reinterpret_cast<DWORD>(this));//pNMListView->iSubItem);
	*pResult = 0;
}

void CDirView::OnDestroy() 
{
	DeleteAllDisplayItems();

	// save the column widths
	CListCtrl& ctl = GetListCtrl();

	CHeaderCtrl *phdr = ctl.GetHeaderCtrl();
	for (int i=0; i < phdr->GetItemCount(); i++)
	{
		CString s;
		s.Format(_T("WDirHdr%d"), i);
		theApp.WriteProfileInt(_T("DirView"), s, ctl.GetColumnWidth(i));
	}

	CListViewEx::OnDestroy();
	
}



void CDirView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_RETURN)
	{
		WaitStatusCursor waitstatus(_T("Opening selection"));
		OpenSelection();
	}
	CListViewEx::OnChar(nChar, nRepCnt, nFlags);
}

// TODO: This just opens first selected item
// should it do something different when multiple items are selected ?
// (Perry, 2002-12-04)
void CDirView::OpenSelection()
{
	int sel = m_pList->GetNextItem(-1, LVNI_SELECTED);
	if (sel != -1)
	{
		POSITION diffpos = GetItemKey(sel);
		DIFFITEM di = GetDiffContext()->GetDiffAt(diffpos);
		switch(di.code)
		{
		case FILE_DIFF:
			{
				CString left, right;
				GetItemFileNames(sel, left, right);
				mf->ShowMergeDoc(GetDocument(), left, right);
			}
			break;
		case FILE_SAME:
			{
				CString s;
				VERIFY(s.LoadString(IDS_FILESSAME));
				AfxMessageBox(s, MB_ICONINFORMATION);
			}
			break;
		case FILE_LDIRUNIQUE:
		case FILE_RDIRUNIQUE:
			{
				CString s;
				VERIFY(s.LoadString(IDS_FILEISDIR));
				AfxMessageBox(s, MB_ICONINFORMATION);
			}
			break;
		case FILE_LUNIQUE:
		case FILE_RUNIQUE:
			{
				CString s;
				VERIFY(s.LoadString(IDS_FILEUNIQUE));
				AfxMessageBox(s, MB_ICONINFORMATION);
			}
			break;
		case FILE_BINDIFF:
		case FILE_BINSAME:
			{
				CString s;
				VERIFY(s.LoadString(IDS_FILEBINARY));
				AfxMessageBox(s, MB_ICONSTOP);
			}
			break;
		default:
			{
				CString s;
				VERIFY(s.LoadString(IDS_FILEERROR));
				AfxMessageBox(s, MB_ICONSTOP);
			}
			break;
		}
	}

}

// User chose (context menu) delete left
void CDirView::OnCtxtDirDelLeft()
{
	DoDelLeft();
}
// User chose (context menu) delete right
void CDirView::OnCtxtDirDelRight()
{
	DoDelRight();
}
// User chose (context menu) delete both
void CDirView::OnCtxtDirDelBoth()
{
	DoDelBoth();
}


// Enable/disable Delete Left menu choice on context menu
void CDirView::OnUpdateCtxtDirDelLeft(CCmdUI* pCmdUI)
{
	DoUpdateCtxtDirDelLeft(pCmdUI);
}

// Enable/disable Delete Right menu choice on context menu
void CDirView::OnUpdateCtxtDirDelRight(CCmdUI* pCmdUI) 
{
	DoUpdateCtxtDirDelRight(pCmdUI);
}
// Enable/disable Delete Both menu choice on context menu
void CDirView::OnUpdateCtxtDirDelBoth(CCmdUI* pCmdUI) 
{
	DoUpdateCtxtDirDelBoth(pCmdUI);
}

// Should Delete left be enabled or disabled ?
void CDirView::DoUpdateCtxtDirDelLeft(CCmdUI* pCmdUI)
{
	int sel=-1;
	int count=0, total=0;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (IsItemDeletableOnLeft(di.code))
			++count;
		++total;
	}
	pCmdUI->Enable(count>0);
	CString s;
	AfxFormatString2(s, IDS_DEL_LEFT_FMT, NumToStr(count), NumToStr(total));
	pCmdUI->SetText(s);
}
// Should Delete right be enabled or disabled ?
void CDirView::DoUpdateCtxtDirDelRight(CCmdUI* pCmdUI) 
{
	int sel=-1;
	int count=0, total=0;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (IsItemDeletableOnRight(di.code))
			++count;
		++total;
	}
	pCmdUI->Enable(count>0);
	CString s;
	AfxFormatString2(s, IDS_DEL_RIGHT_FMT, NumToStr(count), NumToStr(total));
	pCmdUI->SetText(s);
}
// Should Delete both be enabled or disabled ?
void CDirView::DoUpdateCtxtDirDelBoth(CCmdUI* pCmdUI) 
{
	int sel=-1;
	int count=0, total=0;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (IsItemDeletableOnBoth(di.code))
			++count;
		++total;
	}
	pCmdUI->Enable(count>0);
	CString s;
	AfxFormatString2(s, IDS_DEL_BOTH_FMT, NumToStr(count), NumToStr(total));
	pCmdUI->SetText(s);
}


POSITION CDirView::GetItemKey(int idx)
{
	return GetItemKeyFromData(m_pList->GetItemData(idx));
}

// SetItemKey & GetItemKey encapsulate how the display list items
// are mapped to DiffItems, which in turn are DiffContext keys to the actual DIFFITEM data
POSITION CDirView::GetItemKeyFromData(DWORD dw)
{
	return (POSITION)dw;
}
void CDirView::SetItemKey(int idx, POSITION diffpos)
{
	m_pList->SetItemData(idx, (DWORD)diffpos);
}

// given index in list control, get its associated DIFFITEM data
DIFFITEM CDirView::GetDiffItem(int sel)
{
	POSITION diffpos = GetItemKey(sel);
	return GetDiffContext()->GetDiffAt(diffpos);
}

void CDirView::DeleteAllDisplayItems()
{
	// item data are just positions (diffposes)
	// that is, they contain no memory needing to be freed
	m_pList->DeleteAllItems();
}

// given key, get index of item which has it stored
int CDirView::GetItemIndex(DWORD key)
{
	LVFINDINFO findInfo;

	findInfo.flags = LVFI_PARAM;  // Search for itemdata
	findInfo.lParam = key;
	return m_pList->FindItem( &findInfo );
}

// User chose (context menu) open left
void CDirView::OnCtxtDirOpenLeft()
{
	DoOpen(SIDE_LEFT);
}
// User chose (context menu) open right
void CDirView::OnCtxtDirOpenRight()
{
	DoOpen(SIDE_RIGHT);
}

// User chose (context menu) open left with
void CDirView::OnCtxtDirOpenLeftWith()
{
	DoOpenWith(SIDE_LEFT);
}
// User chose (context menu) open right with
void CDirView::OnCtxtDirOpenRightWith()
{
	DoOpenWith(SIDE_RIGHT);
}
// return selected item index, or -1 if none or multiple
int CDirView::GetSingleSelectedItem() const
{
	int sel=-1, sel2=-1;
	sel = m_pList->GetNextItem(sel, LVNI_SELECTED);
	if (sel == -1) return -1;
	sel2 = m_pList->GetNextItem(sel, LVNI_SELECTED);
	if (sel2 != -1) return -1;
	return sel;
}
// Enable/disable Open Left menu choice on context menu
void CDirView::OnUpdateCtxtDirOpenLeft(CCmdUI* pCmdUI)
{
	DoUpdateOpenLeft(pCmdUI);
}
// Enable/disable Open Right menu choice on context menu
void CDirView::OnUpdateCtxtDirOpenRight(CCmdUI* pCmdUI)
{
	DoUpdateOpenRight(pCmdUI);
}

// Enable/disable Open Left With menu choice on context menu
void CDirView::OnUpdateCtxtDirOpenLeftWith(CCmdUI* pCmdUI)
{
	DoUpdateOpenLeft(pCmdUI);
}
// Enable/disable Open Right With menu choice on context menu
void CDirView::OnUpdateCtxtDirOpenRightWith(CCmdUI* pCmdUI)
{
	DoUpdateOpenRight(pCmdUI);
}

// used for both OpenLeft and OpenLeftWith
void CDirView::DoUpdateOpenLeft(CCmdUI* pCmdUI)
{
	int sel = GetSingleSelectedItem();
	if (sel != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (!IsItemOpenableOnLeft(di.code))
			sel = -1;
	}

	pCmdUI->Enable(sel>=0);
}
// used for both OpenRight and OpenRightWith
void CDirView::DoUpdateOpenRight(CCmdUI* pCmdUI)
{
	int sel = GetSingleSelectedItem();
	if (sel != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (!IsItemOpenableOnRight(di.code))
			sel = -1;
	}

	pCmdUI->Enable(sel>=0);
}

UINT CDirView::GetSelectedCount() const
{
	return m_pList->GetSelectedCount();
}

int CDirView::GetFirstSelectedInd()
{
	int sel =- 1;
	sel = m_pList->GetNextItem(sel, LVNI_SELECTED);
	
	return sel;
}

DIFFITEM CDirView::GetNextSelectedInd(int &ind)
{
	DIFFITEM di;
	int sel =- 1;

	sel = m_pList->GetNextItem(ind, LVNI_SELECTED);
	di = GetDiffItem(ind);
	ind = sel;
	
	return di;
}

DIFFITEM CDirView::GetItemAt(int ind)
{
	DIFFITEM di;
	if (ind != -1)
	{	
		di = GetDiffItem(ind);
	}
	return di;
}

// Go to first diff
// If none or one item selected select found item
void CDirView::OnFirstdiff()
{
	DIFFITEM di;
	const int count = m_pList->GetItemCount();
	BOOL found = FALSE;
	int i = 0;
	int currentInd = GetFirstSelectedInd();
	int selCount = GetSelectedCount();

	while (i < count && found == FALSE)
	{
		di = GetItemAt(i);
		if (IsItemNavigableDiff(di))
		{
			MoveSelection(currentInd, i, selCount);
			found = TRUE;
		}
		i++;
	}
}

void CDirView::OnUpdateFirstdiff(CCmdUI* pCmdUI)
{
	int firstDiff = GetFirstDifferentItem();
	if (firstDiff > -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

// Go to last diff
// If none or one item selected select found item
void CDirView::OnLastdiff()
{
	DIFFITEM di;
	BOOL found = FALSE;
	const int count = m_pList->GetItemCount();
	int i = count - 1;
	int currentInd = GetFirstSelectedInd();
	int selCount = GetSelectedCount();

	while (i > -1 && found == FALSE)
	{
		di = GetItemAt(i);
		if (IsItemNavigableDiff(di))
		{
			MoveSelection(currentInd, i, selCount);
			found = TRUE;
		}
		i--;
	}
}



void CDirView::OnUpdateLastdiff(CCmdUI* pCmdUI)
{
	int firstDiff = GetFirstDifferentItem();
	if (firstDiff > -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

// Go to next diff
// If none or one item selected select found item
void CDirView::OnNextdiff()
{
	DIFFITEM di;
	const int count = m_pList->GetItemCount();
	BOOL found = FALSE;
	int i = GetFocusedItem();
	int currentInd = 0;
	int selCount = GetSelectedCount();

	currentInd = i;
	i++;

	while (i < count && found == FALSE)
	{
		di = GetItemAt(i);
		if (IsItemNavigableDiff(di))
		{
			MoveSelection(currentInd, i, selCount);
			found = TRUE;
		}
		i++;
	}
}


void CDirView::OnUpdateNextdiff(CCmdUI* pCmdUI)
{
	int focused = GetFocusedItem();
	int lastDiff = GetLastDifferentItem();

	// Check if different files were found and
	// there is different item after focused item
	if ((lastDiff > -1) && (focused < lastDiff))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

// Go to prev diff
// If none or one item selected select found item
void CDirView::OnPrevdiff()
{
	DIFFITEM di;
	BOOL found = FALSE;
	int i = GetFocusedItem();
	int currentInd = 0;
	int selCount = GetSelectedCount();

	currentInd = i;
	if (i > 0)
		i--;

	while (i > -1 && found == FALSE)
	{
		di = GetItemAt(i);
		if (IsItemNavigableDiff(di))
		{
			MoveSelection(currentInd, i, selCount);
			found = TRUE;
		}
		i--;
	}
}


void CDirView::OnUpdatePrevdiff(CCmdUI* pCmdUI)
{
	int focused = GetFocusedItem();
	int firstDiff = GetFirstDifferentItem();

	// Check if different files were found and
	// there is different item before focused item
	if ((firstDiff > -1) && (firstDiff < focused))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CDirView::OnCurdiff()
{
	const int count = m_pList->GetItemCount();
	BOOL found = FALSE;
	int i = GetFirstSelectedInd();
	BOOL selected = FALSE;
	BOOL focused = FALSE;

	// No selection - no diff to go
	if (i == -1)
		i = count;

	while (i < count && found == FALSE)
	{
		selected = m_pList->GetItemState(i, LVIS_SELECTED);
		focused = m_pList->GetItemState(i, LVIS_FOCUSED);
			
		if (selected == LVIS_SELECTED && focused == LVIS_FOCUSED)
		{
			m_pList->EnsureVisible(i, FALSE);
			found = TRUE;
		}
		i++;
	}
}

void CDirView::OnUpdateCurdiff(CCmdUI* pCmdUI)
{
	int selection = GetFirstSelectedInd();	
	if (selection > -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

int CDirView::GetFocusedItem()
{
	int retInd = -1;
	int i = 0;
	const int count = m_pList->GetItemCount();
	BOOL found = FALSE;
	BOOL focused = FALSE;

	while (i < count && found == FALSE)
	{
		focused = m_pList->GetItemState(i, LVIS_FOCUSED);			
		if (focused == LVIS_FOCUSED)
		{
			retInd = i;
			found = TRUE;
		}
		i++;
	}
	return retInd;
}

int CDirView::GetFirstDifferentItem()
{
	DIFFITEM di;
	const int count = m_pList->GetItemCount();
	BOOL found = FALSE;
	int i = 0;
	int foundInd = -1;

	while (i < count && found == FALSE)
	{
		di = GetItemAt(i);
		if (IsItemNavigableDiff(di))
		{
			foundInd = i;		
			found = TRUE;
		}
		i++;
	}
	return foundInd;
}

int CDirView::GetLastDifferentItem()
{
	DIFFITEM di;
	const int count = m_pList->GetItemCount();
	BOOL found = FALSE;
	int i = count - 1;
	int foundInd = -1;

	while (i > 0 && found == FALSE)
	{
		di = GetItemAt(i);
		if (IsItemNavigableDiff(di))
		{
			foundInd = i;		
			found = TRUE;
		}
		i--;
	}
	return foundInd;
}

// When navigating differences, do we stop at this one ?
bool CDirView::IsItemNavigableDiff(const DIFFITEM & di) const
{
	switch(di.code)
	{
	case FILE_DIFF: return true;
	case FILE_BINDIFF: return true;
	case FILE_LUNIQUE: return true;
	case FILE_RUNIQUE: return true;
	}
	return false;
}

// move focus (& selection if only one selected) from currentInd to i
void CDirView::MoveSelection(int currentInd, int i, int selCount)
{
	if (selCount <= 1)
	{
		m_pList->SetItemState(currentInd, 0, LVIS_SELECTED);
		m_pList->SetItemState(currentInd, 0, LVIS_FOCUSED);
		m_pList->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}

	m_pList->SetItemState(i, LVIS_FOCUSED, LVIS_FOCUSED);
	m_pList->EnsureVisible(i, FALSE);
}

void CDirView::OnUpdateSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

CDirFrame * CDirView::GetParentFrame()
{
	// can't verify cast without introducing more coupling
	// (CDirView doesn't include DirFrame.h)
	return (CDirFrame *)CListViewEx::GetParentFrame();
}

void CDirView::OnRefresh()
{
	GetDocument()->Rescan();
}
