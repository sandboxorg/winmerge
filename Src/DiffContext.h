/**
 *  @file DiffContext.h
 *
 *  @brief Declarations of CDiffContext and diff structures
 */ 
// RCS ID line follows -- this is updated by CVS
// $Id$

#if !defined(AFX_DIFFCONTEXT_H__D3CC86BE_F11E_11D2_826C_00A024706EDC__INCLUDED_)
#define AFX_DIFFCONTEXT_H__D3CC86BE_F11E_11D2_826C_00A024706EDC__INCLUDED_
#pragma once

#include "RegExp.h"

struct dirdata
{
  char const **names;	/* Sorted names of files in dir, 0-terminated.  */
  char *data;	/* Allocated storage for file names.  */
};

// values for DIFFITEM.code
namespace DIFFCODE {
	enum { 
		// We use extra bits so that no valid values are 0
		// and each set of flags is in a different hex digit
		// to make debugging easier
		// These can always be packed down in the future
		TEXTFLAG=0x3, TEXT=0x1, BIN=0x2,
		DIRFLAG=0x30, FILE=0x10, DIR=0x20,
		SIDEFLAG=0x300, LEFT=0x100, RIGHT=0x200, BOTH=0x300,
		COMPAREFLAGS=0x7000, SAME=0x1000, DIFF=0x2000, SKIPPED=0x3000, CMPERR=0x4000
	};
};
// old codes
/* TODO: delete
#define FILE_LUNIQUE     0
#define FILE_RUNIQUE     1
#define FILE_DIFF        2
#define FILE_SAME        3
#define FILE_ERROR       4
#define FILE_BINSAME     5
#define FILE_BINDIFF     6
#define FILE_LDIRUNIQUE  7
#define FILE_RDIRUNIQUE  8
#define FILE_SKIP        9
#define FILE_DIRSKIP    10
*/

struct FileFlags
{
	int flags;
	FileFlags() : flags(0) { }
	void reset() { flags = 0; }
	CString toString() const { return flags ? _T("RO") : _T(""); }

	enum { RO=1 };
};

struct DIFFITEM;

/**
 * @brief information for file on one side of a diff
 */
struct DiffFileInfo
{
	// storing __time_t if MSVC6 (__MSC_VER<1300)
	// storing __time64_t if MSVC7 (VC.NET)
	__int64 mtime; /**< time of last modification */
	__int64 ctime; /**< time of creation */
	__int64 size; /**< file size in bytes */
	CString version; /**< string of fixed file version, eg, 1.2.3.4 */
	CString spath; /**< fully qualified directory of file */
	FileFlags flags; /**< file attributes */
	DiffFileInfo() : mtime(0), ctime(0), size(0) { }
	// We could stash a pointer here to the parent DIFFITEM
	// but, I ran into trouble with, I think, the DIFFITEM copy constructor
};

/**
 * @brief information about one diff (including files on both sides)
 */
struct DIFFITEM
{
	DiffFileInfo left;
	DiffFileInfo right;
	CString sfilename;
	CString sSubdir; //*< Common subdirectory from root of comparison */
	CString sext;
	int diffcode;

	DIFFITEM() : diffcode(0) { }

	CString getLeftFilepath() const;
	CString getRightFilepath() const;

	// file/directory
	bool isDirectory() const { return ((diffcode & DIFFCODE::DIRFLAG) == DIFFCODE::DIR); }
	// left/right
	bool isSideLeft() const { return ((diffcode & DIFFCODE::SIDEFLAG) == DIFFCODE::LEFT); }
	bool isSideRight() const { return ((diffcode & DIFFCODE::SIDEFLAG) == DIFFCODE::RIGHT); }
	// result filters
	bool isResultError() const { return ((diffcode & DIFFCODE::COMPAREFLAGS) == DIFFCODE::CMPERR); }
	bool isResultSame() const { return ((diffcode & DIFFCODE::COMPAREFLAGS) == DIFFCODE::SAME); }
	bool isResultSkipped() const { return ((diffcode & DIFFCODE::COMPAREFLAGS) == DIFFCODE::SKIPPED); }
	// type
	bool isBin() const { return ((diffcode & DIFFCODE::TEXTFLAG) == DIFFCODE::BIN); }
};

// Interface for reporting current file, as diff traverses file tree
class IDiffStatus
{
public:
	virtual void rptFile(BYTE code)=0;
};

// Interface for testing files & directories for exclusion, as diff traverses file tree
class IDiffFilter
{
public:
	virtual BOOL includeFile(LPCTSTR szFileName)=0;
	virtual BOOL includeDir(LPCTSTR szDirName)=0;
};

class CDiffContext  
{
public:
	CDiffContext(LPCTSTR pszLeft, LPCTSTR pszRight);
	CDiffContext(LPCTSTR pszLeft, LPCTSTR pszRight, CDiffContext& src);
	virtual ~CDiffContext();

	void SetRegExp(LPCTSTR pszExp);

	// add & remove differences
	void AddDiff(LPCTSTR pszFilename, LPCTSTR szSubdir, LPCTSTR pszLeftDir, LPCTSTR pszRightDir
		, __int64 lmtime, __int64 rmtime, __int64 lctime, __int64 rctime
		, __int64 lsize, __int64 rsize, int diffcode);
	void AddDiff(DIFFITEM di);
	void RemoveDiff(POSITION diffpos);
	void RemoveAll();
	void UpdateFieldsNeededForNewItems(DIFFITEM & di, DiffFileInfo & dfi);
	void UpdateVersion(DIFFITEM & di, DiffFileInfo & dfi);

	// to iterate over all differences on list
	POSITION GetFirstDiffPosition();
	DIFFITEM GetNextDiffPosition(POSITION & diffpos);
//	DIFFITEM GetDiffAt(POSITION diffpos);
	const DIFFITEM & GetDiffAt(POSITION diffpos) const;
//	int GetDiffStatus(POSITION diffpos);
	int GetDiffCount();

	// change an existing difference
	void SetDiffStatusCode(POSITION diffpos, UINT diffcode, UINT mask);
	void UpdateInfoFromDisk(DIFFITEM & di);
	void UpdateInfoFromDiskHalf(DIFFITEM & di, DiffFileInfo & dfi);
	void UpdateStatusFromDisk(POSITION diffpos);

	BOOL m_bRecurse;
	CString m_strLeft;
	CString m_strRight;
	CString m_strNormalizedLeft;
	CString m_strNormalizedRight;
	CRegExp m_rgx;
	CString m_strRegExp;
	IDiffFilter * m_piFilter;
	UINT m_msgUpdateStatus;
	HWND m_hMainFrame;

	struct dirdata ddLeft, ddRight;
	char *pNamesLeft;
	char *pNamesRight;

private:
	CList<DIFFITEM,DIFFITEM> m_dirlist, *m_pList; // master list of differences
};

#endif // !defined(AFX_DIFFCONTEXT_H__D3CC86BE_F11E_11D2_826C_00A024706EDC__INCLUDED_)
