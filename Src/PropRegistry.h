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
 * @file  PropRegistry.h
 *
 * @brief Declaration file CPropRegistry
 */
// ID line follows -- this is updated by SVN
// $Id$

#ifndef PropRegistry_h_included
#define PropRegistry_h_included

#include "IOptionsPanel.h"

class COptionsMgr;

/////////////////////////////////////////////////////////////////////////////
// CPropRegistry dialog

/**
 * @brief Property page for system options; used in options property sheet.
 *
 * This class implements property sheet for what we consider System-options.
 * It allows user to select options like whether to use Recycle Bin for
 * deleted files and External text editor.
 */
class CPropRegistry : public CPropertyPage, public IOptionsPanel
{
// Construction
public:
	CPropRegistry(COptionsMgr *optionsMgr);

// Implement IOptionsPanel
	virtual void ReadOptions();
	virtual void WriteOptions();

// Dialog Data
	//{{AFX_DATA(CPropRegistry)
	enum { IDD = IDD_PROPPAGE_SYSTEM };
	CString	m_strEditorPath;
	BOOL	m_bUseRecycleBin;
	CString m_strUserFilterPath;
	int		m_tempFolderType;
	CString m_tempFolder;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropRegistry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation methods
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropRegistry)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseEditor();
	afx_msg void OnBrowseFilterPath();
	afx_msg void OnBrowseTmpFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Implementation data
private:
	COptionsMgr * m_pOptionsMgr; /**< Pointer to options manager */
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PropRegistry_h_included
