#ifndef PropRegistry_h_included
#define PropRegistry_h_included

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropRegistry.h : header file
//
// $Id$

/////////////////////////////////////////////////////////////////////////////
// CPropRegistry dialog

class CPropRegistry : public CPropertyPage
{
// Construction
public:

	CPropRegistry();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropRegistry)
	enum { IDD = IDD_PROPPAGE_REGISTRY };
	BOOL m_bContextAdded;
	CString m_strPath;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropRegistry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation methods
protected:
	bool IsRegisteredForDirectory() const;
	void UpdateButton();
	void EnableContextHandler(bool enabling);
	void GetContextRegValues();

	// Generated message map functions
	//{{AFX_MSG(CPropRegistry)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddToExplorer();
	afx_msg void OnSavePath();
	afx_msg void OnBrowsePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Implementation data
protected:
	bool m_enabled;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PropRegistry_h_included
