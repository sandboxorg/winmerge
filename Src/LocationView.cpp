//////////////////////////////////////////////////////////////////////
/** 
 * @file  LocationView.cpp
 *
 * @brief Implementation file for CLocationView
 *
 */
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "merge.h"
#include "MergeEditView.h"
#include "LocationView.h"
#include "MergeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMergeDiffDetailView

IMPLEMENT_DYNCREATE(CLocationView, CView)


CLocationView::CLocationView()
	: m_view0(0)
	, m_view1(0)
{
}

BEGIN_MESSAGE_MAP(CLocationView, CView)
	//{{AFX_MSG_MAP(CLocationView)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLocationView diagnostics
#ifdef _DEBUG
CMergeDoc* CLocationView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMergeDoc)));
	return (CMergeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLocationView message handlers

/** 
 * @brief Update view.
 */
void CLocationView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{
	CMergeDoc* pDoc = GetDocument();
	m_view0 = pDoc->GetLeftView();
	m_view1 = pDoc->GetRightView();
	Invalidate();
}

/** 
 * @brief Draw maps of files.
 *
 * Draws maps of differences in files. Difference list is walked and
 * every difference is drawn with same colors than in editview.
 * @note We MUST use doubles when calculating coords to avoid rounding
 * to integers. Rounding causes miscalculation of coords.
 * @todo Use of GetNextRect() is inefficient, it reads diffs sometimes
 * twice when it first asks next diff when line is not in any diff. And
 * then reads same diff again when in diff.
 * @sa CLocationView::DrawRect()
 */
void CLocationView::OnDraw(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);

	if (m_view0 == NULL || m_view1 == NULL)
		return;

	const int w = rc.Width() / 4;
	const int x = (rc.Width() - 2 * w) / 3;
	const int x2 = 2 * x + w;
	const int w2 = 2 * x + 2 * w;
	const double yOffset = 5; // Few pixels of empty space around bars
	const double hTotal = rc.Height() - (2 * yOffset); // Height of draw area
	const int nbLines = min(m_view0->GetLineCount(), m_view1->GetLineCount());
	COLORREF cr0 = CLR_NONE; // Left side color
	COLORREF cr1 = CLR_NONE; // Right side color
	COLORREF crt = CLR_NONE; // Text color
	BOOL bwh = FALSE;
	int nstart0 = -1;
	int nend0 = -1;

	while (true)
	{
		BOOL ok0 = GetNextRect(nend0);
		if (!ok0)
			break;

		const double nBeginY = (nstart0 + 1) * hTotal / nbLines + yOffset;
		const double nEndY = (nend0 + 1) * hTotal / nbLines + yOffset;
		nstart0++;

		// Draw left side block
		m_view0->GetLineColors(nstart0, cr0, crt, bwh);
		CRect r0(x, nBeginY, x + w, nEndY);
		DrawRect(pDC, r0, cr0, ((CMergeEditView*)m_view0)->IsLineInCurrentDiff(nstart0));
		
		// Draw right side block
		m_view1->GetLineColors(nstart0, cr1, crt, bwh);
		CRect r1(x2, nBeginY, w2, nEndY);
		DrawRect(pDC, r1, cr1, ((CMergeEditView*)m_view0)->IsLineInCurrentDiff(nstart0));
		nstart0 = nend0;

		// Connected line
		if (ok0 && (cr0 == CLR_NONE) && (cr1 == CLR_NONE))
		{
			CPen* oldObj = (CPen*)pDC->SelectStockObject(BLACK_PEN);
			pDC->MoveTo(r0.right, r0.CenterPoint().y);
			pDC->LineTo(r1.left, r1.CenterPoint().y);
			pDC->SelectObject(oldObj);
		}
	}
}

/** 
 * @brief Return end of block.
 * 
 * Starting from lineindex (not number!) given, finds last line in same block.
 * A block is either all the lines of a diff, or all the lines that separates
 * two diffs, or the beginning lines before the first diff, or the last lines
 * after the last diff.
 * @param nLineIndex [in,out]
 *  - [in] Lineindex where search begins
 *  - [out] Lineindex of last line in same block
 * @return TRUE if last line found.
 */
BOOL CLocationView::GetNextRect(int &nLineIndex)
{
	CMergeDoc *pDoc = GetDocument();
	BOOL bInDiff = FALSE;
	int nextDiff = -1;
	const int nbLines = min(pDoc->GetLineCount(TRUE), pDoc->GetLineCount(FALSE));
	
	++nLineIndex;
	if (nLineIndex >= nbLines)
		return FALSE;

	bInDiff = pDoc->GetNextDiff(nLineIndex, nextDiff);
	
	// No diffs left, return last line of file.
	if (nextDiff == -1)
	{
		nLineIndex = nbLines - 1;
		return TRUE;
	}

	DIFFRANGE di = {0};
	if (!pDoc->GetDiff(nextDiff, di))
		return FALSE;

	// Line not in diff. Return last non-diff line.
	if (bInDiff == FALSE)
	{
		nLineIndex = di.dbegin0 - 1;
		return TRUE;
	}

	// Line is in diff. Get last line from side where all lines are present.
	if (di.op == OP_LEFTONLY || di.op == OP_RIGHTONLY || di.op == OP_DIFF)
	{
		if (di.blank0 == -1)
			nLineIndex = di.dend1;
		else
			nLineIndex = di.dend0;
	}

	return TRUE;
}

/** 
 * @brief Draw one block of map.
 */
void CLocationView::DrawRect(CDC* pDC, const CRect& r, COLORREF cr, BOOL border)
{
	if (cr==CLR_NONE)
	{
		CPen* oldObj = (CPen*)pDC->SelectStockObject(BLACK_PEN);
		pDC->Rectangle(r);
		pDC->SelectObject(oldObj);
	}
	// colored rectangle
	else
	{
		CBrush brush(cr);
		//pDC->FillRect(r,&brush);
		pDC->FillSolidRect(r, cr);
		if (border)
		{
			// outter rectangle
			CRect outter = r;
			outter.InflateRect(2,2);
			// dont erase inside rect
			CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
			CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
			CPen* oldPen = pDC->SelectObject(&pen);
			pDC->Rectangle(outter);
			pDC->SelectObject(oldPen);
			pDC->SelectObject(oldBrush);
		}
	}
}

/// User left double-clicked mouse
void CLocationView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!GotoLocation(point))
		CView::OnLButtonDblClk(nFlags, point);
}

/// Move both views to point given (if in one of the file columns, else return FALSE)
BOOL CLocationView::GotoLocation(CPoint point)
{
	CRect rc;
	GetClientRect(rc);

	if (m_view0 == NULL || m_view1 == NULL)
		return FALSE;

	const int w = rc.Width() / 4;
	const int x = (rc.Width() - 2 * w) / 3;

	bool leftside = (point.x >= x && point.x < x+w);
	bool rightside = (point.x >= 2 * x + w && point.x < 2 * x + 2 * w);
	if (!leftside && !rightside)
		return FALSE;

	const int nbLines = min(m_view0->GetLineCount(), m_view1->GetLineCount());

	int line = point.y * nbLines / rc.Height();

	m_view0->GoToLine(line, false);
	m_view1->GoToLine(line, false);

	return TRUE;
}
