/////////////////////////////////////////////////////////////////////////////
//    WinMerge:  an interactive diff/merge utility
//    Copyright (C) 1997  Dean P. Grimm
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
 * @file  DirDoc.h
 *
 * @brief Declaration file for CDirDoc
 *
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#if !defined(AFX_DIRDOC_H__0B17B4C1_356F_11D1_95CD_444553540000__INCLUDED_)
#define AFX_DIRDOC_H__0B17B4C1_356F_11D1_95CD_444553540000__INCLUDED_
#pragma once


#include "Merge.h"		// DirDocFilter theApp
#include "DirView.h"
#include "DiffContext.h"
#include "diffThread.h"
#include "DiffWrapper.h"

class CMergeDoc;
typedef CTypedPtrList<CPtrList, CMergeDoc *> MergeDocPtrList;
class DirDocFilter;

/**
 * @brief User-defined Windows-messages
 */
const UINT MSG_UI_UPDATE = WM_USER;
const UINT MSG_STAT_UPDATE = WM_USER + 1;

/////////////////////////////////////////////////////////////////////////////
// CDirDoc document

/**
 * @brief Documentclass for directory compare
 */
class CDirDoc : public CDocument
{
protected:
	CDirDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDirDoc)

// Attributes
public:
// Operations
public:
	CDirView * GetMainView();
	CMergeDoc * GetMergeDocForDiff(BOOL * pNew);
	BOOL ReusingDirDoc();
	bool CanFrameClose();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRecursive(BOOL bRecursive);
	BOOL GetReadOnly(BOOL bLeft) const;
	void SetReadOnly(BOOL bLeft, BOOL bReadOnly);
	void ReadSettings();
	void CompareReady();
	void UpdateChangedItem(LPCTSTR pathLeft, LPCTSTR pathRight, bool unified);
	POSITION FindItemFromPaths(LPCTSTR pathLeft, LPCTSTR pathRight);
	void SetDiffContext(CDiffContext *pCtxt);
	void UpdateResources();
	void InitStatusStrings();
	void ReloadItemStatus(UINT nIdx);
	void Redisplay();
	void Rescan();
	CDiffContext *m_pCtxt;
	virtual ~CDirDoc();
	void SetDirView( CDirView *newView ); // TODO Perry
	void AddMergeDoc(CMergeDoc * pMergeDoc);
	void MergeDocClosing(CMergeDoc * pMergeDoc);
	CDiffThread m_diffThread;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void UpdateScreenItemStatus(UINT nIdx, DIFFITEM di);

	CDiffWrapper m_diffWrapper;

	// Generated message map functions
	//{{AFX_MSG(CDirDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Implementation data
private:
	DirDocFilter * m_pFilter;
	CDirView *m_pDirView;
	MergeDocPtrList m_MergeDocs;
	BOOL m_bReuseMergeDocs; // policy to reuse existing merge docs
	BOOL m_bROLeft;
	BOOL m_bRORight;
	BOOL m_bRecursive;
};

/// callback for file/directory filtering during diff
// actually we just forward these calls to the app, to CMergeApp::includeFile & includeDir
class DirDocFilter : public IDiffFilter
{
public:
	virtual BOOL includeFile(LPCTSTR szFileName) { return theApp.includeFile(szFileName); }
	virtual BOOL includeDir(LPCTSTR szDirName) { return theApp.includeDir(szDirName); }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRDOC_H__0B17B4C1_356F_11D1_95CD_444553540000__INCLUDED_)
