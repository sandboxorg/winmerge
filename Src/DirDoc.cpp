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
// DirDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Merge.h"
#include "DirDoc.h"
#include "DirFrame.h"
#include "diff.h"
#include "getopt.h"
#include "fnmatch.h"
#include "MainFrm.h"
#include "coretools.h"
#include "logfile.h"

extern int recursive;

int compare_files (LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR, CDiffContext*, int);


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLogFile gLog;

/////////////////////////////////////////////////////////////////////////////
// CDirDoc

IMPLEMENT_DYNCREATE(CDirDoc, CDocument)

CDirDoc::CDirDoc()
{
	m_pView = NULL;
	m_pCtxt=NULL;
}

CDirDoc::~CDirDoc()
{
	mf->m_pDirDoc = NULL;
	if (m_pCtxt != NULL)
		delete m_pCtxt;
}

BOOL CDirDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CString s;
	VERIFY(s.LoadString(IDS_DIRECTORY_WINDOW_TITLE));
	SetTitle(s);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CDirDoc, CDocument)
	//{{AFX_MSG_MAP(CDirDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirDoc diagnostics

#ifdef _DEBUG
void CDirDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDirDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDirDoc serialization

void CDirDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDirDoc commands

/*int
diff_dirs2 (filevec, handle_file, depth)
     struct file_data const filevec[];
     int (*handle_file) PARAMS((char const *, char const *, char const *, char const *, int));
     int depth;
{
  struct dirdata dirdata[2];
  int val = 0;
  int i;

  // Get sorted contents of both dirs.
  for (i = 0; i < 2; i++)
    if (dir_sort (&filevec[i], &dirdata[i]) != 0)
      {
	perror_with_name (filevec[i].name);
	val = 2;
      }

  if (val == 0)
    {
      char const * const *names0 = dirdata[0].names;
      char const * const *names1 = dirdata[1].names;
      char const *name0 = filevec[0].name;
      char const *name1 = filevec[1].name;

      // If `-S name' was given, and this is the topmost level of comparison,
	 //ignore all file names less than the specified starting name.

      if (dir_start_file && depth == 0)
	{
	  while (*names0 && stricmp (*names0, dir_start_file) < 0)
	    names0++;
	  while (*names1 && stricmp (*names1, dir_start_file) < 0)
	    names1++;
	}

      // Loop while files remain in one or both dirs.
      while (*names0 || *names1)
	{
	  // Compare next name in dir 0 with next name in dir 1.
	     At the end of a dir,
	     pretend the "next name" in that dir is very large.
	  int nameorder = (!*names0 ? 1 : !*names1 ? -1
			   : stricmp (*names0, *names1));
	  int v1 = (*handle_file) (name0, 0 < nameorder ? 0 : *names0++,
				   name1, nameorder < 0 ? 0 : *names1++,
				   depth + 1);
	  if (v1 > val)
	    val = v1;
	}
    }

  for (i = 0; i < 2; i++)
    {
      if (dirdata[i].names)
	free (dirdata[i].names);
      if (dirdata[i].data)
	free (dirdata[i].data);
    }

  return val;
}*/


void CDirDoc::Rescan()
{
	ASSERT(m_pCtxt != NULL);
	BeginWaitCursor();

	gLog.Write(_T("Starting directory scan:\r\n\tLeft: %s\r\n\tRight: %s\r\n"),
			m_pCtxt->m_strLeft, m_pCtxt->m_strRight);
	m_pCtxt->RemoveAll();

	compare_files (0, (char const *)(LPCTSTR)m_pCtxt->m_strLeft,
			       0, (char const *)(LPCTSTR)m_pCtxt->m_strRight, m_pCtxt, 0);

	gLog.Write(_T("Directory scan complete\r\n"));

	CString s;
	AfxFormatString2(s, IDS_DIRECTORY_WINDOW_STATUS_FMT, m_pCtxt->m_strLeft, m_pCtxt->m_strRight);
	((CDirFrame*)(m_pView->GetParent()))->SetStatus(s);
	Redisplay();

	EndWaitCursor();
}

// return true if we need to hide this item because it is a backup
static bool IsItemHiddenBackup(const DIFFITEM & di)
{
	return mf->m_bHideBak && FileExtMatches(di.sfilename,BACKUP_FILE_EXT);
}

// returns path of item if user wants to see this item
// preferably left path, but right path if a right-only item
static LPCTSTR GetItemPathIfShowable(const DIFFITEM & di, int llen, int rlen)
{
	if (IsItemHiddenBackup(di))
		return 0;

	LPCTSTR p = 0;

	BOOL leftside = (di.code==FILE_LUNIQUE || di.code==FILE_LDIRUNIQUE);
	BOOL rightside = (di.code==FILE_RUNIQUE || di.code==FILE_RDIRUNIQUE);

	switch (di.code)
	{
	case FILE_DIFF:
		if (mf->m_bShowDiff)
		{
			p = _tcsninc(di.slpath, llen);
		}
		break;
	case FILE_BINSAME:
		if (mf->m_bShowIdent && mf->m_bShowBinaries)
		{
			p = _tcsninc(di.slpath, llen);
		}
		break;
	case FILE_BINDIFF:
		if (mf->m_bShowDiff && mf->m_bShowBinaries)
		{
			p = _tcsninc(di.slpath, llen);
		}
		break;
	case FILE_LUNIQUE:
	case FILE_RUNIQUE:
	case FILE_LDIRUNIQUE:
	case FILE_RDIRUNIQUE:
		if ((mf->m_bShowUniqueLeft && leftside) 
			|| (mf->m_bShowUniqueRight && rightside))
		{
			if (di.code==FILE_LUNIQUE || di.code==FILE_LDIRUNIQUE)
				p = _tcsninc(di.slpath, llen);
			else
				p = _tcsninc(di.srpath, rlen);
		}
		break;
	case FILE_SAME:
		if (mf->m_bShowIdent)
		{
			p = _tcsninc(di.slpath, llen);
		}
		break;
	default: // error
		p = _tcsninc(di.slpath, llen);
		break;
	}
	return p;
}

void CDirDoc::Redisplay()
{
	if (m_pCtxt == NULL)
		return;

	CString s,s2;
	UINT cnt=0;
	int llen = m_pCtxt->m_strLeft.GetLength();
	int rlen = m_pCtxt->m_strRight.GetLength();

	m_pView->DeleteAllDisplayItems();

	POSITION diffpos = m_pCtxt->GetFirstDiffPosition();
	while (diffpos)
	{
		POSITION curdiffpos = diffpos;
		DIFFITEM di = m_pCtxt->GetNextDiffPosition(diffpos);

		LPCTSTR p=GetItemPathIfShowable(di, llen, rlen);

		if (p)
		{
			m_pView->AddItem(cnt, DV_NAME, di.sfilename);
			m_pView->AddItem(cnt, DV_EXT, di.sext); // BSP - Add the current file extension
			s = _T(".");
			s += p;
			m_pView->AddItem(cnt, DV_PATH, s);
			m_pView->SetItemKey(cnt, curdiffpos);
			// This is inefficient, as we have the di in hand right now
			// But to improve this, have to figure out the intricacies of UpdateItemStatus
			UpdateItemStatus(cnt);
			cnt++;
		}
	}

}

CDirView * CDirDoc::GetMainView()
{
	POSITION pos = GetFirstViewPosition(), ps2=pos;

	while (pos != NULL)
	{
		CDirView* pView = (CDirView*)GetNextView(pos);
		if (pView->IsKindOf( RUNTIME_CLASS(CDirView)))
			return pView;
	}
	return (CDirView*)GetNextView(ps2);
}

static long GetModTime(LPCTSTR szPath)
{
	struct stat mystats;
	bzero(&mystats, sizeof(mystats));
	int stat_result = stat(szPath, &mystats);
	if (stat_result!=0)
		return 0;
	return mystats.st_mtime;
}

static void UpdateTimes(DIFFITEM * pdi)
{
	CString sLeft = (CString)pdi->slpath + _T("\\") + pdi->sfilename;
	pdi->ltime = GetFileModTime(sLeft);

	CString sRight = (CString)pdi->srpath + _T("\\") + pdi->sfilename;
	pdi->rtime = GetFileModTime(sRight);
}

static CString
TimeString(const time_t * tim)
{
	if (!tim) return _T("---");
	// _tcsftime does not respect user date customizations from
	// Regional Options/Configuration Regional; COleDateTime::Format does so.
	COleDateTime odt = *tim;
	return odt.Format();
}

void CDirDoc::SetItemStatus(UINT nIdx, LPCTSTR szStatus, int image, const time_t * ltime, const time_t * rtime)
{
	m_pView->AddItem(nIdx, DV_STATUS, szStatus);
	m_pView->SetImage(nIdx, image);
	m_pView->AddItem(nIdx, DV_LTIME, TimeString(ltime));
	m_pView->AddItem(nIdx, DV_RTIME, TimeString(rtime));
}

void CDirDoc::UpdateItemStatus(UINT nIdx)
{
	CString s,s2;
//	These are not used currently
//	UINT cnt=0;
//	int llen = m_pCtxt->m_strLeft.GetLength();
//	int rlen = m_pCtxt->m_strRight.GetLength();
	POSITION diffpos = m_pView->GetItemKey(nIdx);
	DIFFITEM di = m_pCtxt->GetDiffAt(diffpos);

	UpdateTimes(&di); // in case just copied (into existence) or modified

	switch (di.code)
	{
	case FILE_DIFF:
		VERIFY(s.LoadString(IDS_FILES_ARE_DIFFERENT));
		SetItemStatus(nIdx, s, FILE_DIFF, &di.ltime, &di.rtime);
		break;
	case FILE_BINDIFF:
		VERIFY(s.LoadString(IDS_BIN_FILES_DIFF));
		SetItemStatus(nIdx, s, FILE_BINDIFF, &di.ltime, &di.rtime);
		break;
	case FILE_BINSAME:
		VERIFY(s.LoadString(IDS_BIN_FILES_SAME));
		SetItemStatus(nIdx, s, FILE_BINSAME, &di.ltime, &di.rtime);
		break;
	case FILE_LUNIQUE:
	case FILE_LDIRUNIQUE:
		AfxFormatString1(s, IDS_ONLY_IN_FMT, di.slpath);
		SetItemStatus(nIdx, s, di.code, &di.ltime, NULL);
		break;
	case FILE_RUNIQUE:
	case FILE_RDIRUNIQUE:
		AfxFormatString1(s, IDS_ONLY_IN_FMT, di.srpath);
		SetItemStatus(nIdx, s, di.code, NULL, &di.rtime);
		break;
	case FILE_SAME:
		VERIFY(s.LoadString(IDS_IDENTICAL));
		SetItemStatus(nIdx, s, FILE_SAME, &di.ltime, &di.rtime);
		break;
	default: // error
		VERIFY(s.LoadString(IDS_CANT_COMPARE_FILES));
		SetItemStatus(nIdx, s, FILE_ERROR, (di.ltime>0 ? &di.ltime : NULL), (di.rtime>0 ? &di.rtime : NULL));
		break;
	}
}


void CDirDoc::InitStatusStrings()
{

}

void CDirDoc::UpdateResources()
{
	m_pView->UpdateResources();

	CString s;
	VERIFY(s.LoadString(IDS_DIRECTORY_WINDOW_TITLE));
	SetTitle(s);

	Redisplay();
}

void CDirDoc::SetDiffContext(CDiffContext *pCtxt)
{
	if (m_pCtxt != NULL)
		delete m_pCtxt;

	m_pCtxt = pCtxt;
}



CString GetPathOnly( const CString& sString )
{
	int nIdx = sString.GetLength();

	CString	sReturn;

	while ( nIdx-- ) {

		if ( sString.GetAt(nIdx) == '/' || sString.GetAt(nIdx) == '\\' ) {
			if ( nIdx > 1 ) {
				char* pszRet = sReturn.GetBufferSetLength( nIdx );
				strncpy( pszRet, sString, nIdx );
				pszRet[nIdx] = '\0';
				sReturn.ReleaseBuffer();
			}
		}
	}

	return sReturn;
}




BOOL CDirDoc::UpdateItemStatus(LPCTSTR pathLeft, LPCTSTR pathRight,
							   UINT status)
{
	POSITION pos = m_pCtxt->GetFirstDiffPosition();
	POSITION currentPos;
	DIFFITEM current;
	int count = m_pCtxt->GetDiffCount();
	int i = 0;
	BOOL found = FALSE;

	CString path1, file1;
	SplitFilename(pathLeft, &path1, &file1, 0);
	CString path2, file2;
	SplitFilename(pathRight, &path2, &file2, 0);

	// Path can contain (because of difftools?) '/' and '\'
	// so for comparing purposes, convert whole path to use '\\'
	path1.Replace('/', '\\');
	path2.Replace('/', '\\');

	// Filenames must be identical
	if (file1 != file2)
		return FALSE;

	// Get first item
	current = m_pCtxt->GetDiffAt(pos);

	while (i < count && found == FALSE)
	{
		// Save our current pos before getting next
		currentPos = pos;
		current = m_pCtxt->GetNextDiffPosition(pos);

		// Path can contain (because of difftools?) '/' and '\'
		// so for comparing purposes, convert whole path to use '\'
		current.srpath.Replace('/', '\\');
		current.slpath.Replace('/', '\\');

		if (path1 == current.slpath &&
			path2 == current.srpath &&
			file1 == current.sfilename)
		{
			// Right item found!
			// Get index at view, update status to context
			// and tell view to update found item
			int ind = m_pView->GetItemIndex((DWORD)currentPos);
			m_pCtxt->UpdateStatusCode(currentPos, (BYTE)status);
			UpdateItemStatus(ind);
			found = TRUE;
		}
		i++;
	}
	return found;
}
CDirView * CDirDoc::SetView(CDirView * newView)
{
	CDirView * currentView = m_pView;
	m_pView = newView;
	return currentView;
}
