// ChildFrm.cpp : implementation of the COpenFrame class
//
#include "stdafx.h"
#include "Merge.h"

#include "OpenFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenFrame

IMPLEMENT_DYNCREATE(COpenFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(COpenFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// COpenFrame construction/destruction

COpenFrame::COpenFrame()
{
	// TODO: add member initialization code here
}

COpenFrame::~COpenFrame()
{
}


BOOL COpenFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

////////////////
// Recalculate the layout of the view.
// Move the view to the center of the window.
// Paul DiLascia described how to center CFormView in MSJ (Oct. 1996)
// http://www.microsoft.com/msj/archive/S1F64.aspx
//
void COpenFrame::RecalcLayout(BOOL bNotify)
{
	// First, let MFC recalculate the layout as per the 
	// normal thing. MFC will figure out where to place 
	// the view, taking into account which status bars 
	// are displayed, etc.
	//
	CFrameWnd::RecalcLayout(bNotify);   

	CScrollView* pView = (CScrollView*)GetActiveView();
	if (pView) {
		// Move form view to center of main 
		// window if it's smaller.
		//
		CRect rcNormalView;
		pView->GetWindowRect(&m_rcNormalView);
		ScreenToClient(&m_rcNormalView);    // view rect MFC would use
		CSize sz = pView->GetTotalSize();   // length, width
		CRect rc = m_rcNormalView;
		if (rc.Width() > sz.cx)                   // if window > form:
			rc.left += (rc.Width() -sz.cx) >> 1;   // move over half the extra
		if (rc.Height() > sz.cy)                  // ditto for height
			rc.top  += (rc.Height()-sz.cy) >> 1;   // ...

		// This actually moves the view window
		pView->SetWindowPos(NULL, rc.left, rc.top, sz.cx, sz.cy,
			SWP_NOACTIVATE|SWP_NOZORDER);
	}
}

//////////////////
// Moving the view/form to the center of the main
// frame window will leave extra white space in upper 
// left corner. To fix this, need to paint two rectangles.
// Paul DiLascia described how to center CFormView in MSJ (Oct. 1996)
// http://www.microsoft.com/msj/archive/S1F64.aspx
//
void COpenFrame::OnPaint() 
{
   CView* pView = GetActiveView();
   if (pView) {
      CPaintDC dc(this);

      // Easier to use HBRUSH than create CBrush here.
      // Note: be sure to use COLOR_3DFACE to get the
      // right logical color, in case user has customized it.
      //
      HBRUSH hOldBrush = (HBRUSH)::SelectObject(dc.m_hDC, 
         GetSysColorBrush(COLOR_APPWORKSPACE));

	  CRect rc, rcFrame;
	  GetClientRect(&rcFrame);
	  rc = rcFrame;

      CRect rcView;                    // actual view pos (we moved it)
      pView->GetWindowRect(&rcView);   // ...
      ScreenToClient(&rcView);         // ...

      // paint horz rectangle along top edge
      rc.bottom = rcView.top;
      dc.PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),PATCOPY);

      // paint vert rectangle along left side
      rc.bottom = rcView.bottom;
      rc.right  = rcView.left;
      dc.PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),PATCOPY);

      // paint horz rectangle along right edge
      rc.left = rcView.left + rcView.Width();
      rc.right = rcFrame.right;
      rc.bottom = rcFrame.bottom;
      dc.PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),PATCOPY);

      // paint horz rectangle along bottom edge
      rc.left = rcFrame.left;
      rc.right = rcFrame.right;
      rc.top = rcView.top + rcView.Height();
      rc.bottom = rcFrame.bottom;
      dc.PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),PATCOPY);

      ::SelectObject(dc.m_hDC, hOldBrush);
   }
   CMDIChildWnd::OnPaint();
}

/**
 * @brief Handle translation of default messages on the status bar
 */
void COpenFrame::GetMessageString(UINT nID, CString& rMessage) const
{
	// load appropriate string
	const String s = theApp.LoadString(nID);
	if (!AfxExtractSubString(rMessage, s.c_str(), 0))
	{
		// not found
		TRACE1("Warning: no message line prompt for ID 0x%04X.\n", nID);
	}
}

void COpenFrame::ActivateFrame(int nCmdShow) 
{
	if (!GetMDIFrame()->MDIGetActive() && theApp.GetProfileInt(_T("Settings"), _T("ActiveFrameMax"), TRUE))
	{
		nCmdShow = SW_SHOWMAXIMIZED;
	}
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

/**
 * @brief Update any resources necessary after a GUI language change
 */
void COpenFrame::UpdateResources()
{
}

/**
 * @brief Save the window's position, free related resources, and destroy the window
 */
BOOL COpenFrame::DestroyWindow() 
{
	// If we are active, save the restored/maximized state
	// If we are not, do nothing and let the active frame do the job.
 	if (GetParentFrame()->GetActiveFrame() == this)
	{
		WINDOWPLACEMENT wp;
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&wp);
		theApp.WriteProfileInt(_T("Settings"), _T("ActiveFrameMax"), (wp.showCmd == SW_MAXIMIZE));
	}

	return CMDIChildWnd::DestroyWindow();
}

// COpenFrame diagnostics

#ifdef _DEBUG
void COpenFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void COpenFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// COpenFrame message handlers
