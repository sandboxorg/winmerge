/** 
 * @file  DirViewColItems.cpp
 *
 * @brief Code for individual columns in the DirView
 *
 * @date  Created: 2003-08-19
 */
// RCS ID line follows -- this is updated by CVS
// $Id$


#include "stdafx.h"
#include "Merge.h"
#include "DirView.h"
#include "DirDoc.h"
#include "MainFrm.h"
#include "resource.h"
#include "DirViewColItems.h"
#include "locality.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 * @brief Return time displayed appropriately, as string
 */
static CString
TimeString(const __int64 * tim)
{
	if (!tim) return _T("---");
	// _tcsftime does not respect user date customizations from
	// Regional Options/Configuration Regional; COleDateTime::Format does so.
#if _MSC_VER < 1300
		// MSVC6
	COleDateTime odt = (time_t)*tim;
#else
		// MSVC7 (VC.NET)
	COleDateTime odt = *tim;
#endif
	return odt.Format();
}
/**
 * @brief Function to compare two __int64s for a sort
 */
static int cmp64(__int64 i1, __int64 i2)
{
	if (i1==i2) return 0;
	return i1>i2 ? 1 : -1;
}
/**
 * @brief Function to compare two diffcodes
 */
static int cmpdiffcode(int diffcode1, int diffcode2)
{
	// TODO: How shall we order these ?
	return diffcode1-diffcode2;	
}

/**
 * @{ Functions to display each type of column info
 */
static CString ColNameGet(const DIFFITEM & di)
{
	return di.sfilename;
}
static CString ColPathGet(const DIFFITEM & di)
{
	if (di.sSubdir.IsEmpty())
		return _T(".");
	else
		return di.sSubdir;
}
static CString ColStatusGet(const DIFFITEM & di)
{
	// Note that order of items does matter. We must check for
	// skipped items before unique items, for example, so that
	// skipped unique items are labeled as skipped, not unique.
	CString s;
	if (di.isResultError())
	{
		VERIFY(s.LoadString(IDS_CANT_COMPARE_FILES));
	}
	else if (di.isSideLeft())
	{
		AfxFormatString1(s, IDS_ONLY_IN_FMT, di.getLeftFilepath());
	}
	else if (di.isSideRight())
	{
		AfxFormatString1(s, IDS_ONLY_IN_FMT, di.getRightFilepath());
	}
	else if (di.isResultSkipped())
	{
		if (di.isDirectory())
			VERIFY(s.LoadString(IDS_DIR_SKIPPED));
		else
			VERIFY(s.LoadString(IDS_FILE_SKIPPED));
	}
	else if (di.isResultSame())
	{
		if (di.isBin())
			VERIFY(s.LoadString(IDS_BIN_FILES_SAME));
		else
			VERIFY(s.LoadString(IDS_IDENTICAL));
	}
	else // diff
	{
		if (di.isBin())
			VERIFY(s.LoadString(IDS_BIN_FILES_DIFF));
		else
			VERIFY(s.LoadString(IDS_FILES_ARE_DIFFERENT));
	}
	return s;
}
static CString ColLmtimeGet(const DIFFITEM & di)
{
	if (di.left.mtime)
		return TimeString(&di.left.mtime);
	else
		return _T("");
}
static CString ColRmtimeGet(const DIFFITEM & di)
{
	if (di.right.mtime)
		return TimeString(&di.right.mtime);
	else
		return _T("");
}
static CString ColLctimeGet(const DIFFITEM & di)
{
	if (di.left.ctime)
		return TimeString(&di.left.ctime);
	else
		return _T("");
}
static CString ColRctimeGet(const DIFFITEM & di)
{
	if (di.right.ctime)
		return TimeString(&di.right.ctime);
	else
		return _T("");
}
static CString ColExtGet(const DIFFITEM & di)
{
	return di.sext;
}
static CString ColLsizeGet(const DIFFITEM & di)
{
	CString s;
	s.Format(_T("%I64d"), di.left.size);
	return locality::GetLocaleStr(s);
}
static CString ColRsizeGet(const DIFFITEM & di)
{
	CString s;
	s.Format(_T("%I64d"), di.right.size);
	return locality::GetLocaleStr(s);
}
static CString ColNewerGet(const DIFFITEM & di)
{
	if (di.left.mtime && di.right.mtime)
	{
		if (di.left.mtime > di.right.mtime)
			return _T("<<<");
		if (di.left.mtime < di.right.mtime)
			return _T(">>>");
		return _T("===");
	}
	else if (di.left.mtime)
	{
		return _T("<*<");
	}
	else if (di.right.mtime)
	{
		return _T(">*>");
	}
	else
	{
		return _T("***");
	}
}
static CString ColLverGet(const DIFFITEM & di)
{
	return di.left.version;
}
static CString ColRverGet(const DIFFITEM & di)
{
	return di.right.version;
}
static CString ColStatusAbbrGet(const DIFFITEM & di)
{
	int id;

	if (di.isResultError())
	{
		id = IDS_CANT_COMPARE_FILES;
	}
	else if (di.isResultSkipped())
	{
		if (di.isDirectory())
			id = IDS_DIR_SKIPPED;
		else
			id = IDS_FILE_SKIPPED;
	}
	else if (di.isSideLeft())
	{
		id = IDS_LEFTONLY;
	}
	else if (di.isSideRight())
	{
		id = IDS_RIGHTONLY;
	}
	else if (di.isResultSame())
	{
		if (di.isBin())
			id = IDS_BIN_FILES_SAME;
		else
			id = IDS_IDENTICAL;
	}
	else // diff
	{
		if (di.isBin())
			id = IDS_BIN_FILES_DIFF;
		else
			id = IDS_FILES_ARE_DIFFERENT;
	}

	CString s;
	VERIFY(s.LoadString(id));
	return s;
}
static CString ColLattrGet(const DIFFITEM & di)
{
	return di.left.flags.toString();
}
static CString ColRattrGet(const DIFFITEM & di)
{
	return di.right.flags.toString();
}
/**
 * @}
 */

/**
 * @{ Functions to sort each type of column info
 */
static int ColNameSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ldi.sfilename.CompareNoCase(rdi.sfilename);
}
static int ColPathSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ldi.sSubdir.CompareNoCase(rdi.sSubdir);
}
static int ColStatusSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return cmpdiffcode(rdi.diffcode, ldi.diffcode);
}
static int ColLmtimeSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return cmp64(rdi.left.mtime, ldi.left.mtime);
}
static int ColRmtimeSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return cmp64(rdi.right.mtime, ldi.right.mtime);
}
static int ColLctimeSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return cmp64(rdi.left.ctime, ldi.left.ctime);
}
static int ColRctimeSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return cmp64(rdi.right.ctime, ldi.right.ctime);
}
static int ColExtSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ldi.sext.CompareNoCase(rdi.sext);
}
static int ColLsizeSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return cmp64(rdi.left.size, ldi.left.size);
}
static int ColRsizeSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return cmp64(rdi.right.size, ldi.right.size);
}
static int ColNewerSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ColNewerGet(ldi).Compare(ColNewerGet(rdi));
}
static int ColLverSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ldi.left.version.Compare(rdi.left.version);
}
static int ColRverSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ldi.right.version.Compare(rdi.right.version);
}
static int ColLattrSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ldi.left.flags.toString().Compare(rdi.left.flags.toString());
}
static int ColRattrSort(const DIFFITEM & ldi, const DIFFITEM &rdi)
{
	return ldi.right.flags.toString().Compare(rdi.right.flags.toString());
}
/**
 * @}
 */

/**
 * @brief All existing columns
 */
DirColInfo g_cols[] =
{
	{ _T("Name"), IDS_COLHDR_FILENAME, -1, &ColNameGet, &ColNameSort, 0, true }
	, { _T("Path"), IDS_COLHDR_DIR, -1, &ColPathGet, &ColPathSort, 1, true }
	, { _T("Status"), IDS_COLHDR_RESULT, -1, &ColStatusGet, &ColStatusSort, 2, true}
	, { _T("Lmtime"), IDS_COLHDR_LTIMEM, -1, &ColLmtimeGet, &ColLmtimeSort, 3, false }
	, { _T("Rmtime"), IDS_COLHDR_RTIMEM, -1, &ColRmtimeGet, &ColRmtimeSort, 4, false }
	, { _T("Lctime"), IDS_COLHDR_LTIMEC, -1, &ColLctimeGet, &ColLctimeSort, -1, false }
	, { _T("Rctime"), IDS_COLHDR_RTIMEC, -1, &ColRctimeGet, &ColRctimeSort, -1, false }
	, { _T("Ext"), IDS_COLHDR_EXTENSION, -1, &ColExtGet, &ColExtSort, 5, true }
	, { _T("Lsize"), IDS_COLHDR_LSIZE, -1, &ColLsizeGet, &ColLsizeSort, -1, true }
	, { _T("Rsize"), IDS_COLHDR_RSIZE, -1, &ColRsizeGet, &ColRsizeSort, -1, true }
	, { _T("Newer"), IDS_COLHDR_NEWER, -1, &ColNewerGet, &ColNewerSort, -1, true }
	, { _T("Lversion"), IDS_COLHDR_LVERSION, -1, &ColLverGet, &ColLverSort, -1, true }
	, { _T("Rversion"), IDS_COLHDR_RVERSION, -1, &ColRverGet, &ColRverSort, -1, true }
	, { _T("StatusAbbr"), IDS_COLHDR_RESULT_ABBR, -1, &ColStatusAbbrGet, &ColStatusSort, -1, true }
	, { _T("Lattr"), IDS_COLHDR_LATTRIBUTES, -1, &ColLattrGet, &ColLattrSort, -1, true }
	, { _T("Rattr"), IDS_COLHDR_RATTRIBUTES, -1, &ColRattrGet, &ColRattrSort, -1, true }
};

/**
 * @brief Count of all known columns
 */
int g_ncols = countof(g_cols);

/**
 * @brief Registry base value name for saving/loading info for this column
 */
CString CDirView::GetColRegValueNameBase(int col) const
{
	ASSERT(col>=0 && col<countof(g_cols));
	CString regName;
	regName.Format(_T("WDirHdr_%s"), g_cols[col].regName);
	return regName;
}

/**
 * @brief Get default physical order for specified logical column
 */
int CDirView::GetColDefaultOrder(int col) const
{
	ASSERT(col>=0 && col<countof(g_cols));
	return g_cols[col].physicalIndex;
}
