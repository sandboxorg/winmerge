/** 
 * @file  DiffViewBar.cpp
 *
 * @brief Implementation file for CDiffViewBar
 *
 */

#include "stdafx.h"
#include "DiffViewBar.h"
#include "Merge.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDiffViewBar, TViewBarBase);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiffViewBar::CDiffViewBar()
: m_pwndDetailSplitter(0)
, m_hwndFrame(NULL)
{
}


CDiffViewBar::~CDiffViewBar()
{
}


BEGIN_MESSAGE_MAP(CDiffViewBar, TViewBarBase)
	//{{AFX_MSG_MAP(CRegBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiffViewBar message handlers

/**
* @brief Just create ourself
*
* @note The control are created in the parent frame CChildFrame
*
*/
BOOL CDiffViewBar::Create(
	CWnd* pParentWnd,
	LPCTSTR lpszWindowName,
	DWORD dwStyle,
	UINT nID)
{
	return TViewBarBase::Create(
		lpszWindowName,
		pParentWnd,
		nID,
		dwStyle);
}


///	Create the frame window associated with the view bar. */
int CDiffViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TViewBarBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSCBStyle(SCBS_EDGETOP | SCBS_EDGEBOTTOM | SCBS_SIZECHILD);
	
	return 0;
}

/// update height for both panels
void CDiffViewBar::UpdateBarHeight(int DiffPanelHeight)
{
	if (m_pwndDetailSplitter == NULL || ::IsWindow(m_pwndDetailSplitter->m_hWnd) == FALSE)
		return;

	// first resize the splitter
	CRect rc;
	GetClientRect(rc);
	m_pwndDetailSplitter->MoveWindow(rc);
	m_pwndDetailSplitter->RecalcLayout();
}

void CDiffViewBar::OnSize(UINT nType, int cx, int cy) 
{
	TViewBarBase::OnSize(nType, cx, cy);
	UpdateBarHeight(-1);
}

/**
* @note The window must always be docked after movement
* there are too much troubles if we get reparented to some minidockbar 
*
*/
void CDiffViewBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	TViewBarBase::OnLButtonDown(nFlags, point);
	if (m_pDockBar != NULL)
	{
		if (IsHorzDocked() == FALSE)
			m_pDockContext->ToggleDocking();
	}
}

/** 
 * @brief Informs parent frame (CChildFrame) when bar is closed.
 *
 * After bar is closed parent frame saves bar states.
 */
void CDiffViewBar::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	TViewBarBase::OnWindowPosChanged(lpwndpos);

	if (m_hwndFrame != NULL)
	{
		// If WINDOWPOS.flags has SWP_HIDEWINDOW flag set
		if ((lpwndpos->flags & SWP_HIDEWINDOW) != 0)
			::PostMessage(m_hwndFrame, MSG_STORE_PANESIZES, 0, 0);
	}
}

/** 
 * @brief Stores HWND of frame window (CChildFrame).
 */
void CDiffViewBar::SetFrameHwnd(HWND hwndFrame)
{
	m_hwndFrame = hwndFrame;
}
/**
 * @brief Update any resources necessary after a GUI language change
 */
void CDiffViewBar::UpdateResources()
{
	String sCaption = _("Diff Pane");
	SetWindowText(sCaption.c_str());
}
