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
// DiffContext.cpp: implementation of the CDiffContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "merge.h"
#include "DiffContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiffContext::CDiffContext(LPCTSTR pszLeft /*=NULL*/, LPCTSTR pszRight /*=NULL*/, IDiffStatus * piStatus)
{
	m_bRecurse=FALSE;
	m_strLeft = pszLeft;
	m_strRight = pszRight;
	m_pList = &m_dirlist;

	pNamesLeft = NULL;
	pNamesRight = NULL;
	m_piStatus = piStatus;
}

CDiffContext::CDiffContext(LPCTSTR pszLeft, LPCTSTR pszRight, CDiffContext& src)
{
	// This is used somehow in recursive comparisons
	// I think that it is only used during rescan to copy into temporaries
	// which modify the original (because pointers are copied below)
	// and then the temporary goes away
	// so the temporary never exists while the user is interacting with the GUI

	m_bRecurse=src.m_bRecurse;
	m_strLeft = pszLeft;
	m_strRight = pszRight;
	m_pList = src.m_pList;
	SetRegExp(src.m_strRegExp);
	m_piStatus = src.m_piStatus;

	pNamesLeft = NULL;
	pNamesRight = NULL;
}

CDiffContext::~CDiffContext()
{
	if (pNamesLeft != NULL)
		free(pNamesLeft);
	if (pNamesRight != NULL)
		free(pNamesRight);
}
	

void CDiffContext::AddDiff(LPCTSTR pszFilename, LPCTSTR pszLeftDir, LPCTSTR pszRightDir, 
						   long ltime, long rtime, BYTE code)
{
	DIFFITEM di;
	strcpy(di.filename,pszFilename);
	strcpy(di.lpath, pszLeftDir);
	strcpy(di.rpath, pszRightDir);
	di.ltime = ltime;
	di.rtime = rtime;
	di.code = code;
	AddDiff(di);
}

void CDiffContext::AddDiff(DIFFITEM di)
{
	// BSP - Capture the extension; from the end of the file name to the last '.'     
	int j = 0;
	TCHAR *pDest = _tcsrchr(di.filename, _T('.') );

	if(pDest)	// handle no extensions case.
	{
		++pDest;	// advance past dot.
		while(*pDest!=_T('\0'))
		{
			di.extension[j++] = tolower(*pDest++);
		}
	}
	di.extension[j] = _T('\0');


	m_pList->AddTail(di);

	if (m_piStatus)
		m_piStatus->rptFile(di.code);
}

void CDiffContext::RemoveDiff(POSITION diffpos)
{
	m_pList->RemoveAt(diffpos);
}

void CDiffContext::SetRegExp(LPCTSTR pszExp)
{
	m_strRegExp = pszExp;
	m_rgx.RegComp( pszExp );
}

void CDiffContext::RemoveAll()
{
	m_pList->RemoveAll();
}

POSITION CDiffContext::GetFirstDiffPosition()
{
	return m_pList->GetHeadPosition();
}

DIFFITEM CDiffContext::GetNextDiffPosition(POSITION & diffpos)
{
	return m_pList->GetNext(diffpos);
}

DIFFITEM CDiffContext::GetDiffAt(POSITION diffpos)
{
	return m_pList->GetAt(diffpos);
}

BYTE CDiffContext::GetDiffStatus(POSITION diffpos)
{
	return m_pList->GetAt(diffpos).code;
}


void CDiffContext::UpdateStatusCode(POSITION diffpos, BYTE status)
{
	DIFFITEM & di = m_pList->GetAt(diffpos);
	di.code = status;
}

