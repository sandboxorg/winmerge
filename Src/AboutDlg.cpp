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
 * @file  AboutDlg.cpp
 *
 * @brief Implementation of the About-dialog.
 *
 */
// ID line follows -- this is updated by SVN
// $Id: AboutDlg.cpp 6380 2009-01-25 22:13:36Z kimmov $

#include "stdafx.h"
#include "Constants.h"
#include "Merge.h"
#include "AboutDlg.h"
#include "version.h"
#include "paths.h"
#include "Environment.h"


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_CONTRIBUTORS, OnBnClickedOpenContributors)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_COMPANY, m_ctlCompany);
	DDX_Control(pDX, IDC_WWW, m_ctlWWW);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
	DDX_Text(pDX, IDC_PRIVATEBUILD, m_strPrivateBuild);
	//}}AFX_DATA_MAP
}

/** 
 * @brief Read version info from resource to dialog.
 */
BOOL CAboutDlg::OnInitDialog() 
{
	theApp.TranslateDialog(m_hWnd);
	CDialog::OnInitDialog();

	// Load application icon
	HICON icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (icon != NULL) {
		CStatic * pIcon = (CStatic *) GetDlgItem(IDC_ABOUTBOX_ICON);
		pIcon->SetIcon(icon);
	}

	CVersionInfo version(AfxGetResourceHandle());
	String sVersion = version.GetProductVersion();
	m_strVersion = LangFormatString1(IDS_VERSION_FMT, sVersion.c_str()).c_str();

#ifdef _UNICODE
	m_strVersion += _T(" ");
	m_strVersion += theApp.LoadString(IDS_UNICODE).c_str();
#endif

#if defined _M_IX86
	m_strVersion += _T(" x86");
#elif defined _M_IA64
	m_strVersion += _T(" IA64");
#elif defined _M_X64
	m_strVersion += _T(" ");
	m_strVersion += theApp.LoadString(IDS_WINX64).c_str();
#endif

	String sPrivateBuild = version.GetPrivateBuild();
	if (!sPrivateBuild.empty())
	{
		m_strPrivateBuild = LangFormatString1(IDS_PRIVATEBUILD_FMT,
			sPrivateBuild.c_str()).c_str();
	}

	String copyright = version.GetLegalCopyright();
	m_ctlCompany.SetWindowText(copyright.c_str());
	m_ctlWWW.m_link = WinMergeURL;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
 * @brief Show contributors list.
 * Opens Contributors.txt into notepad.
 */
void CAboutDlg::OnBnClickedOpenContributors()
{
	String defPath = env_GetProgPath();
	// Don't add quotation marks yet, CFile doesn't like them
	String docPath = defPath + ContributorsPath;
	HINSTANCE ret = 0;
	
	if (paths_DoesPathExist(docPath) == IS_EXISTING_FILE)
	{
		// Now, add quotation marks so ShellExecute() doesn't fail if path
		// includes spaces
		docPath.insert(0, _T("\""));
		docPath.insert(docPath.length(), _T("\""));
		ret = ShellExecute(m_hWnd, NULL, _T("notepad"), docPath.c_str(), defPath.c_str(), SW_SHOWNORMAL);

		// values < 32 are errors (ref to MSDN)
		if ((int)ret < 32)
		{
			// Try to open with associated application (.txt)
			ret = ShellExecute(m_hWnd, _T("open"), docPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
			if ((int)ret < 32)
				ResMsgBox1(IDS_ERROR_EXECUTE_FILE, _T("Notepad.exe"), MB_ICONSTOP);
		}
	}
	else
		ResMsgBox1(IDS_ERROR_FILE_NOT_FOUND, docPath.c_str(), MB_ICONSTOP);
}


