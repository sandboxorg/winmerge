//////////////////////////////////////////////////////////////////////
/** 
 * @file  DiffViewBar.cpp
 *
 * @brief Implementation file for CDiffViewBar
 *
 */
// RCS ID line follows -- this is updated by CVS
// $Id$
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Merge.h"

#include "ChildFrm.h"
#include "DiffViewBar.h"

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

	SetSCBStyle(SCBS_EDGETOP | 	SCBS_EDGEBOTTOM);
	
	return 0;
}


int CDiffViewBar::GetPanelHeight()
{
	if (IsWindowVisible() == FALSE)
		return 0;

	int hDetailLeft,hmin;
	m_pwndDetailSplitter->GetRowInfo(0, hDetailLeft, hmin);
	return hDetailLeft;
}


/// update height for both panels
void CDiffViewBar::UpdateBarHeight(int DiffPanelHeight)
{
	if (IsWindowVisible() == FALSE)
		return;

	// first resize the splitter
	CRect rc;
	GetClientRect(rc);
	m_pwndDetailSplitter->MoveWindow(rc);
	m_pwndDetailSplitter->RecalcLayout();

	if (DiffPanelHeight == 0)
		return;

	// and resize the control bar
	int hDetailLeft,hmin;
	int hDetailRight;
	m_pwndDetailSplitter->GetRowInfo(0, hDetailLeft, hmin);
	m_pwndDetailSplitter->GetRowInfo(1, hDetailRight, hmin);
	int sum = hDetailLeft+hDetailRight;
	int dsum = 2*DiffPanelHeight - sum;
	if (DiffPanelHeight >= 0 && dsum != 0)
		extendBar(CPoint (0,dsum));
}





void CDiffViewBar::OnSize(UINT nType, int cx, int cy) 
{
	TViewBarBase::OnSize(nType, cx, cy);
	// the frame window is our parent frame
	// it will resize the splitter (or any other control in the CDiffViewBar)
	CFrameWnd * paren = GetParentFrame();
	// If this is a floating window (for example they try to dock to the side),
	// we get reparented to some minidockbar -- Perry, 2003-08-16
	CChildFrame * pframe = dynamic_cast<CChildFrame *>(paren);
	if (pframe)		
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
