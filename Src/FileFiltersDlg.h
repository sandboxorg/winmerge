/////////////////////////////////////////////////////////////////////////////
//    License (GPLv2+):
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but
//    WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/////////////////////////////////////////////////////////////////////////////
/**
 * @file  FileFiltersDlg.h
 *
 * @brief Declaration file for File Filters dialog
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#if !defined(AFX_FILTERSDLG_H__6180CBC3_99BB_4C85_B0EA_BA85D219285E__INCLUDED_)
#define AFX_FILTERSDLG_H__6180CBC3_99BB_4C85_B0EA_BA85D219285E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef StringPair_declared
#define StringPair_declared
struct StringPair { CString first; CString second; };
class StringPairArray : public CArray<StringPair, StringPair> { }; // need class so can forward declare
#endif

/**
 * @brief Class for dialog allowing user to select
 * and edit used file filters
 */
class FileFiltersDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(FileFiltersDlg)

// Construction
public:
	FileFiltersDlg();   // standard constructor
	void SetFilterArray(StringPairArray * fileFilters);
	CString GetSelected();
	void SetSelected(CString selected);
	void InitList();

	CString m_sFileFilterPath;

// Dialog Data
	//{{AFX_DATA(FileFiltersDlg)
	enum { IDD = IDD_FILEFILTERS };
	CListCtrl	m_listFilters;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FileFiltersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	StringPairArray * m_Filters;

	void AddToGrid(int filterIndex);
	// Generated message map functions
	//{{AFX_MSG(FileFiltersDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFiltersEditbtn();
	afx_msg void OnDblclkFiltersList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSDLG_H__6180CBC3_99BB_4C85_B0EA_BA85D219285E__INCLUDED_)
