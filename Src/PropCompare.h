#ifndef _PROPPAGE_COMPARE_H_
#define _PROPPAGE_COMPARE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropCompare.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropCompare dialog

/**
 * class CPropCompare Property page to set the compare method for files.
 *                     Currently 2 methods are available: - compare by contents, 
 *                                                        - compare by modified date
 *
 * @author Tim Musschoot
 */
class CPropCompare : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropCompare)

// Construction
public:
    enum CompareMethod { BY_CONTENTS, BY_DATE};

	CPropCompare();
	~CPropCompare();

// Dialog Data
	//{{AFX_DATA(CPropCompare)
	enum { IDD = IDD_PROPPAGE_COMPARE };
	int		m_compareMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropCompare)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropCompare)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _PROPPAGE_COMPARE_H_
