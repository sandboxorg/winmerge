#if !defined(AFX_PROPGENERAL_H__30AD07B0_E420_11D1_BBC5_00A024706EDC__INCLUDED_)
#define AFX_PROPGENERAL_H__30AD07B0_E420_11D1_BBC5_00A024706EDC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropGeneral dialog

class CPropGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropGeneral)

// Construction
public:
	CPropGeneral();
	~CPropGeneral();

// Dialog Data
	//{{AFX_DATA(CPropGeneral)
	enum { IDD = IDD_PROPPAGE_LARGE };
	BOOL	m_bBackup;
	BOOL	m_bScroll;
	UINT	m_nTabSize;
	BOOL	m_bIgnoreCase;
	BOOL	m_bIgnoreBlankLines;
	BOOL	m_bEolSensitive;
	BOOL	m_bDisableSplash;
	int		m_nIgnoreWhite;
	int		m_nTabType;
	BOOL	m_bAutomaticRescan; 
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CPropGeneral)
	afx_msg void OnAnyWhitespaceChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPGENERAL_H__30AD07B0_E420_11D1_BBC5_00A024706EDC__INCLUDED_)
