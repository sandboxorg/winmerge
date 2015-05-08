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
 * @file  FilePathEdit.cpp
 *
 * @brief Implementation of the CFilepathEdit class.
 */

#include "stdafx.h"
#include "FilepathEdit.h"
#include "Merge.h"
#include "BCMenu.h"
#include "ClipBoard.h"
#include "Shlwapi.h"
#include "paths.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int FormatFilePathForDisplayWidth(CDC * pDC, int maxWidth, CString & sFilepath);

BEGIN_MESSAGE_MAP(CFilepathEdit, CEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


/** 
 * @brief Format the path for display in header control. 
 *
 * Formats path so it fits to given length, tries to end lines after
 * slash characters.
 *
 * @param [in] pDC Pointer to draw context.
 * @param [in] maxWidth Maximum width of the string in the GUI.
 * @param [in,out] sFilepath:
 * - in: string to format
 * - out: formatted string
 * @return Number of lines path is splitted to.
 */
int FormatFilePathForDisplayWidth(CDC * pDC, int maxWidth, String & sFilepath)
{
	int iBegin = 0;
	int nLines = 1;
	
	while (1)
	{
		String line;

		// find the next truncation point
		int iEndMin = 0;
		int iEndMax = sFilepath.length() - iBegin + 1;
		while(1)
		{
			int iEnd = (iEndMin + iEndMax) / 2;
			if (iEnd == iEndMin)
				break;
			line = sFilepath.substr(iBegin, iEnd);
			int width = (pDC->GetTextExtent(line.c_str())).cx;
			if (width > maxWidth)
				iEndMax = iEnd;
			else
				iEndMin = iEnd;
		};
		ASSERT(iEndMax == iEndMin+1);

		// here iEndMin is the last character displayed in maxWidth

		// exit the loop if we can display the remaining characters with no truncation
		if (iBegin + iEndMin == sFilepath.length())
			break;

		// truncate the text to the previous "\" if possible
		line = sFilepath.substr(iBegin, iEndMin);
		int lastSlash = line.rfind('\\');
		if (lastSlash != String::npos)
			iEndMin = lastSlash + 1;

		sFilepath.insert(iBegin + iEndMin, _T("\n"));
		iBegin += iEndMin + 2;
		nLines ++;
	}

	return nLines;
}

/**
 * @brief Constructor.
 * Set text color to black and background white by default.
 */
CFilepathEdit::CFilepathEdit()
 : m_crBackGnd(RGB(255, 255, 255))
 , m_crText(RGB(0,0,0))
 , m_bActive(FALSE)
{
}

/**
 * @brief Subclass the control.
 * @param [in] nID ID of the control to subclass.
 * @param [in] pParent Parent control of the control to subclass.
 * @return TRUE if succeeded, FALSE otherwise.
 */
BOOL CFilepathEdit::SubClassEdit(UINT nID, CWnd* pParent)
{
	m_bActive = FALSE;
	return SubclassDlgItem(nID, pParent);
};

/**
 * @brief Return the control's original text.
 * @return Control's original text.
 */
void CFilepathEdit::GetOriginalText(String& rString) const
{		
	rString = m_sOriginalText;
}

/**
 * @brief Set the text to show in the control.
 * This function sets the text (original text) to show in the control.
 * The control may modify the text for displaying in the GUI.
 */
void CFilepathEdit::SetOriginalText(const String& sString)
{
	if (m_sOriginalText.compare(sString) == 0)
		return;

	m_sOriginalText = sString;

	RefreshDisplayText();
}

/**
 * @brief Re-format the displayed text and update GUI.
 * This method formats the visible text from original text.
 */
void CFilepathEdit::RefreshDisplayText()
{
	String line = m_sOriginalText;

	// we want to keep the first and the last path component, and in between,
	// as much characters as possible from the right
	// PathCompactPath keeps, in between, as much characters as possible from the left
	// so we reverse everything between the first and the last component before calling PathCompactPath
	size_t iBeginLast = line.rfind('\\');
	size_t iEndIntro = line.find('\\');
	if (iBeginLast != String::npos && iEndIntro != iBeginLast)
	{
		String textToReverse = line.substr(iEndIntro + 1, iBeginLast -
				(iEndIntro + 1));
		std::reverse(textToReverse.begin(), textToReverse.end());
		line = line.substr(0, iEndIntro + 1) + textToReverse + line.substr(iBeginLast);
	}

	// get a device context object
	CClientDC lDC(this);
	// and use the correct font
	CFont *pFontOld = lDC.SelectObject(GetFont());	

	// compact the path
	CRect rect;
	GetRect(rect);
	// take GetBuffer (lenght +3) to count for ellipsis
	std::vector<TCHAR> tmp(line.length() + 3);
	std::copy(line.begin(), line.end(), tmp.begin());
	PathCompactPath(lDC.GetSafeHdc(), &tmp[0],	rect.Width());
	line = &tmp[0];
	
	// set old font back
	lDC.SelectObject(pFontOld);

	// we reverse back everything between the first and the last component
	// it works OK as "..." reversed = "..." again
	iBeginLast = line.rfind('\\');
	iEndIntro = line.find('\\');
	if (iBeginLast != String::npos && iEndIntro != iBeginLast)
	{
		String textToReverse = line.substr(iEndIntro + 1, iBeginLast -
				(iEndIntro+1));
		std::reverse(textToReverse.begin(), textToReverse.end());
		line = line.substr(0, iEndIntro + 1) + textToReverse + line.substr(iBeginLast);
	}

	SetWindowText(line.c_str());
}

/**
 * @brief Updates and returns the tooltip for this edit box
 */
const String& CFilepathEdit::GetUpdatedTipText(CDC * pDC, int maxWidth)
{
	GetOriginalText(m_sToolTipString);
	FormatFilePathForDisplayWidth(pDC, maxWidth, m_sToolTipString);
	return m_sToolTipString;
}

/**
 * @brief retrieve text from the OriginalText
 *
 * @note The standard Copy function works with the (compacted) windowText 
 */
void CFilepathEdit::CustomCopy(int iBegin, int iEnd /*=-1*/)
{
	if (iEnd == -1)
		iEnd = m_sOriginalText.length();

	PutToClipboard(m_sOriginalText.substr(iBegin, iEnd - iBegin), m_hWnd);
}

/**
 * @brief Format the context menu.
 */
void CFilepathEdit::OnContextMenu(CWnd*, CPoint point)
{
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		BCMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_EDITOR_HEADERBAR));
		theApp.TranslateMenu(menu.m_hMenu);

		BCMenu* pPopup = static_cast<BCMenu *>(menu.GetSubMenu(0));
		ASSERT(pPopup != NULL);

		DWORD sel = GetSel();
		if (HIWORD(sel) == LOWORD(sel))
			pPopup->EnableMenuItem(ID_EDITOR_COPY, MF_GRAYED);
		if (paths_EndsWithSlash(m_sOriginalText))
			// no filename, we have to disable the unwanted menu entry
			pPopup->EnableMenuItem(ID_EDITOR_COPY_FILENAME, MF_GRAYED);

		// invoke context menu
		// we don't want to use the main application handlers, so we
		// use flags TPM_NONOTIFY | TPM_RETURNCMD
		// and handle the command after TrackPopupMenu
		int command = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON |
			TPM_NONOTIFY  | TPM_RETURNCMD, point.x, point.y, AfxGetMainWnd());

		// compute the beginning of the text to copy (in OriginalText)
		int iBegin = 0;
		switch (command)
		{
		case ID_EDITOR_COPY:
			Copy();
			return;
		case ID_EDITOR_COPY_FILENAME:
			{
			size_t lastSlash = m_sOriginalText.rfind('\\');
			if (lastSlash == String::npos)
				lastSlash = m_sOriginalText.rfind('/');
			if (lastSlash != String::npos)
				iBegin = lastSlash+1;
			else
				iBegin = 0;
			}
			break;
		case ID_EDITOR_COPY_PATH:
			// pass the heading "*" for modified files
			if (m_sOriginalText.at(0) == '*')
				iBegin = 2;
			else
				iBegin = 0;
			break;
		default:
			return;
		}
		
		CustomCopy(iBegin);
	}
}

void CFilepathEdit::OnNcPaint()
{
	CWindowDC dc(this);
	CRect rect;
	GetWindowRect(rect);
	rect.OffsetRect(-rect.TopLeft());
	dc.FillSolidRect(rect, m_crBackGnd);
}

/**
 * @brief Set the control to look active/inactive.
 * This function sets control to look like an active control. We don't
 * have real focus on this control, but editor pane below it. However
 * for user this active look informs which editor pane is active.
 * @param [in] bActive If TRUE set control look like active control.
 */
void CFilepathEdit::SetActive(bool bActive)
{
	m_bActive = bActive;

	if (m_hWnd == NULL)
		return;

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	if (bActive)
	{
		SetTextColor(::GetSysColor(COLOR_CAPTIONTEXT));
		SetBackColor(::GetSysColor(COLOR_ACTIVECAPTION));
	}
	else
	{
		SetTextColor(::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
		SetBackColor(::GetSysColor(COLOR_INACTIVECAPTION));
	}
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

/**
 * @brief Set control's colors.
 * @param [in] pDC pointer to device context.
 * @param [in] nCtlColor Control color to set.
 * @note Parameter @p nCtlColor is not used but must be present as this method
 * is called by framework.
 * @return Brush for background.
 */
HBRUSH CFilepathEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	UNUSED_ALWAYS(nCtlColor);
	// Return a non-NULL brush if the parent's 
	//handler should not be called

	//set text color
	pDC->SetTextColor(m_crText);

	//set the text's background color
	pDC->SetBkColor(m_crBackGnd);

	//return the brush used for background this sets control background
	return m_brBackGnd;
}

/**
 * @brief Set control's bacground color.
 * @param [in] rgb Color to set as background color.
 */
void CFilepathEdit::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background)
	m_crBackGnd = rgb;
	
	//free brush
	if (m_brBackGnd.GetSafeHandle())
		m_brBackGnd.DeleteObject();
	//set brush to new color
	m_brBackGnd.CreateSolidBrush(rgb);
	
	//redraw
	Invalidate(TRUE);
}

/**
 * @brief Set control's text color.
 * @param [in] Color to set as text color.
 */
void CFilepathEdit::SetTextColor(COLORREF rgb)
{
	//set text color ref
	m_crText = rgb;

	//redraw
	Invalidate(TRUE);
}
