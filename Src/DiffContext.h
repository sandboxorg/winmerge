// DiffContext.h: interface for the CDiffContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIFFCONTEXT_H__D3CC86BE_F11E_11D2_826C_00A024706EDC__INCLUDED_)
#define AFX_DIFFCONTEXT_H__D3CC86BE_F11E_11D2_826C_00A024706EDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RegExp.h"

struct dirdata
{
  char const **names;	/* Sorted names of files in dir, 0-terminated.  */
  char *data;	/* Allocated storage for file names.  */
};

typedef struct tagDIFFITEM {
	TCHAR filename[_MAX_FNAME+_MAX_EXT];
	TCHAR extension[_MAX_EXT];
	TCHAR lpath[MAX_PATH], rpath[MAX_PATH];
	long ltime, rtime;
	BYTE code;
}DIFFITEM;

#define FILE_LUNIQUE		0
#define FILE_RUNIQUE		1
#define FILE_DIFF		2
#define FILE_SAME		3
#define FILE_ERROR		4
#define FILE_BINDIFF	5
#define FILE_LDIRUNIQUE		6
#define FILE_RDIRUNIQUE		7

class CDiffContext  
{
public:
	void SetRegExp(LPCTSTR pszExp);
	void AddDiff(LPCTSTR pszFilename, LPCTSTR pszLeftDir, LPCTSTR pszRightDir, long ltime, long rtime, BYTE code);
	CDiffContext(LPCTSTR pszLeft = NULL, LPCTSTR pszRight = NULL);
	CDiffContext(LPCTSTR pszLeft, LPCTSTR pszRight, CDiffContext& src);
	virtual ~CDiffContext();

	CList<DIFFITEM,DIFFITEM> m_dirlist, *m_pList;
	BOOL m_bRecurse;
	CString m_strLeft;
	CString m_strRight;
	CRegExp m_rgx;
	CString m_strRegExp;

	struct dirdata ddLeft, ddRight;
	char *pNamesLeft;
	char *pNamesRight;
};

#endif // !defined(AFX_DIFFCONTEXT_H__D3CC86BE_F11E_11D2_826C_00A024706EDC__INCLUDED_)
