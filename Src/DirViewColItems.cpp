/** 
 * @file  DirViewColItems.cpp
 *
 * @brief Code for individual columns in the DirView
 *
 * @date  Created: 2003-08-19
 */
// ID line follows -- this is updated by SVN
// $Id: DirViewColItems.cpp 7063 2009-12-27 15:28:16Z kimmov $


#include "stdafx.h"
#include <shlwapi.h>
#include <Poco/Timestamp.h>
#include "UnicodeString.h"
#include "Merge.h"
#include "DirView.h"
#include "DirDoc.h"
#include "MainFrm.h"
#include "resource.h"
#include "DirViewColItems.h"
#include "locality.h"
#include "unicoder.h"
#include "coretools.h"
#include "paths.h"

using Poco::Timestamp;

// shlwapi.h prior to VC6SP6 might lack definition of StrIsIntlEqual
#ifdef UNICODE
#define StrIsIntlEqual StrIsIntlEqualW
#else
#define StrIsIntlEqual StrIsIntlEqualA
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using std::swap;

/**
 * @name Constants for short sizes.
 */
/* @{ */
static const UINT KILO = 1024;
static const UINT MEGA = 1024 * KILO;
static const UINT GIGA = 1024 * MEGA;
static const __int64 TERA = 1024 * (__int64) GIGA;
/**
 * @}
 */

namespace
{
const char *COLHDR_FILENAME     = N_("Filename");
const char *COLHDR_DIR          = N_("Folder");
const char *COLHDR_RESULT       = N_("Comparison result");
const char *COLHDR_LTIMEM       = N_("Left Date");
const char *COLHDR_RTIMEM       = N_("Right Date");
const char *COLHDR_MTIMEM       = N_("Middle Date");
const char *COLHDR_EXTENSION    = N_("Extension");
const char *COLHDR_LSIZE        = N_("Left Size");
const char *COLHDR_RSIZE        = N_("Right Size");
const char *COLHDR_MSIZE        = N_("Middle Size");
const char *COLHDR_RSIZE_SHORT  = N_("Right Size (Short)");
const char *COLHDR_LSIZE_SHORT  = N_("Left Size (Short)");
const char *COLHDR_MSIZE_SHORT  = N_("Middle Size (Short)");
const char *COLHDR_LTIMEC       = N_("Left Creation Time");
const char *COLHDR_RTIMEC       = N_("Right Creation Time");
const char *COLHDR_MTIMEC       = N_("Middle Creation Time");
const char *COLHDR_NEWER        = N_("Newer File");
const char *COLHDR_LVERSION     = N_("Left File Version");
const char *COLHDR_RVERSION     = N_("Right File Version");
const char *COLHDR_MVERSION     = N_("Middle File Version");
const char *COLHDR_RESULT_ABBR  = N_("Short Result");
const char *COLHDR_LATTRIBUTES  = N_("Left Attributes");
const char *COLHDR_RATTRIBUTES  = N_("Right Attributes");
const char *COLHDR_MATTRIBUTES  = N_("Middle Attributes");
const char *COLHDR_LEOL_TYPE    = N_("Left EOL");
const char *COLHDR_MEOL_TYPE    = N_("Middle EOL");
const char *COLHDR_REOL_TYPE    = N_("Right EOL");
const char *COLHDR_LENCODING    = N_("Left Encoding");
const char *COLHDR_RENCODING    = N_("Right Encoding");
const char *COLHDR_MENCODING    = N_("Middle Encoding");
const char *COLHDR_NIDIFFS      = N_("Ignored Diff.");
const char *COLHDR_NSDIFFS      = N_("Differences");
const char *COLHDR_BINARY       = N_("Binary");

const char *COLDESC_FILENAME    = N_("Filename or folder name.");
const char *COLDESC_DIR         = N_("Subfolder name when subfolders are included.");
const char *COLDESC_RESULT      = N_("Comparison result, long form.");
const char *COLDESC_LTIMEM      = N_("Left side modification date.");
const char *COLDESC_RTIMEM      = N_("Right side modification date.");
const char *COLDESC_MTIMEM      = N_("Middle side modification date.");
const char *COLDESC_EXTENSION   = N_("File's extension.");
const char *COLDESC_LSIZE       = N_("Left file size in bytes.");
const char *COLDESC_RSIZE       = N_("Right file size in bytes.");
const char *COLDESC_MSIZE       = N_("Middle file size in bytes.");
const char *COLDESC_LSIZE_SHORT = N_("Left file size abbreviated.");
const char *COLDESC_RSIZE_SHORT = N_("Right file size abbreviated.");
const char *COLDESC_MSIZE_SHORT = N_("Middle file size abbreviated.");
const char *COLDESC_LTIMEC      = N_("Left side creation time.");
const char *COLDESC_RTIMEC      = N_("Right side creation time.");
const char *COLDESC_MTIMEC      = N_("Middle side creation time.");
const char *COLDESC_NEWER       = N_("Tells which side has newer modification date.");
const char *COLDESC_LVERSION    = N_("Left side file version, only for some filetypes.");
const char *COLDESC_RVERSION    = N_("Right side file version, only for some filetypes.");
const char *COLDESC_MVERSION    = N_("Middle side file version, only for some filetypes.");
const char *COLDESC_RESULT_ABBR = N_("Short comparison result.");
const char *COLDESC_LATTRIBUTES = N_("Left side attributes.");
const char *COLDESC_RATTRIBUTES = N_("Right side attributes.");
const char *COLDESC_MATTRIBUTES = N_("Middle side attributes.");
const char *COLDESC_LEOL_TYPE   = N_("Left side file EOL type");
const char *COLDESC_REOL_TYPE   = N_("Right side file EOL type");
const char *COLDESC_MEOL_TYPE   = N_("Middle side file EOL type");
const char *COLDESC_LENCODING   = N_("Left side encoding.");
const char *COLDESC_RENCODING   = N_("Right side encoding.");
const char *COLDESC_MENCODING   = N_("Middle side encoding.");
const char *COLDESC_NIDIFFS     = N_("Number of ignored differences in file. These differences are ignored by WinMerge and cannot be merged.");
const char *COLDESC_NSDIFFS     = N_("Number of differences in file. This number does not include ignored differences.");
const char *COLDESC_BINARY      = N_("Shows an asterisk (*) if the file is binary.");
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
 * @brief Convert __int64 to int sign
 */
static int sign64(__int64 val)
{
  if (val>0) return 1;
  if (val<0) return -1;
  return 0;
}
/**
 * @brief Function to compare two diffcodes for a sort
 * @todo How shall we order diff statuses?
 */
static UINT cmpdiffcode(UINT diffcode1, UINT diffcode2)
{
	// Lower priority of the same items (FIXME:)
	if (((diffcode1 & DIFFCODE::COMPAREFLAGS) == DIFFCODE::SAME) && ((diffcode2 & DIFFCODE::COMPAREFLAGS) != DIFFCODE::SAME))
		return -1;
	if (((diffcode1 & DIFFCODE::COMPAREFLAGS) != DIFFCODE::SAME) && ((diffcode2 & DIFFCODE::COMPAREFLAGS) == DIFFCODE::SAME))
		return 1;
	if ((diffcode1 & DIFFCODE::DIR) && !(diffcode2 & DIFFCODE::DIR))
		return 1;
	if (!(diffcode1 & DIFFCODE::DIR) && (diffcode2 & DIFFCODE::DIR))
		return -1;
	return diffcode1-diffcode2;	
}
/**
 * @brief Function to compare two doubles for a sort
 */
static int cmpfloat(double v1, double v2)
{
	if (v1>v2)
		return 1;
	if (v1<v2)
		return -1;
	return 0;
}
/**
 * @brief Formats a size as a short string.
 *
 * MakeShortSize(500) = "500b"
 * MakeShortSize(1024) = "1Kb"
 * MakeShortSize(12000) = "1.7Kb"
 * MakeShortSize(200000) = "195Kb"
 * @param [in] size File's size to convert.
 * @return Size string with localized suffix.
 * @note Localized suffix strings are read from resource.
 * @todo Can't handle > terabyte filesizes.
 */
static String MakeShortSize(__int64 size)
{
#pragma warning(disable:4244) // warning C4244: '=' : conversion from '__int64' to 'double', possible loss of data
	double fsize = size;
#pragma warning(default:4244) // warning C4244: '=' : conversion from '__int64' to 'double', possible loss of data
	double number = 0;
	int ndigits = 0;
	String suffix;

	if (size < KILO)
	{
		number = fsize;
		suffix = _(" B");
	}
	else if (size < MEGA)
	{
		number = fsize / KILO;
		suffix = _(" KB");
		if (size < KILO * 10)
		{
			ndigits = 2;
		}
		else if (size < KILO * 100)
		{
			ndigits = 1;
		}
	}
	else if (size < GIGA)
	{
		number = fsize / (MEGA);
		suffix = _(" MB");
		if (size < MEGA * 10)
		{
			ndigits = 2;
		}
		else if (size < MEGA * 100)
		{
			ndigits = 1;
		}
	}
	else if (size < (__int64)TERA)
	{
		number = fsize / ((__int64)GIGA);
		suffix = _(" GB");
		if (size < (__int64)GIGA * 10)
		{
			ndigits = 2;
		}
		else if (size < (__int64)GIGA * 100)
		{
			ndigits = 1;
		}
	}
	else
	{
		// overflow (?) -- show ">TB"
		String s(_T(">"));
		suffix = _(" TB");
		s += suffix;
		return s;
	}

	TCHAR buffer[48];
	_sntprintf(buffer, countof(buffer), _T("%lf"), number);
	return locality::GetLocaleStr(buffer, ndigits) + suffix;
}

/**
 * @name Functions to format content of each type of column.
 * These functions all receive two parameters, a pointer to CDiffContext.
 * which contains general compare information. And a void pointer whose type
 * depends on column to format. Function to call for each column, and
 * parameter for the function are defined in static DirColInfo f_cols table.
 */
/* @{ */
/**
 * @brief Format Filename column data.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColFileNameGet(const CDiffContext *, const void *p) //sfilename
{
	const DIFFITEM &di = *static_cast<const DIFFITEM*>(p);
	return
	(
		di.diffFileInfo[0].filename.empty() ? di.diffFileInfo[1].filename :
		di.diffFileInfo[1].filename.empty() ? di.diffFileInfo[0].filename :
		di.diffFileInfo[0].filename == di.diffFileInfo[1].filename ? di.diffFileInfo[0].filename :
		di.diffFileInfo[0].filename + _T("|") + di.diffFileInfo[1].filename
	);
}

/**
 * @brief Format Extension column data.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColExtGet(const CDiffContext *, const void *p) //sfilename
{
	const DIFFITEM &di = *static_cast<const DIFFITEM*>(p);
	// We don't show extension for folder names
	if (di.diffcode.isDirectory())
		return _T("");
	const String &r = di.diffFileInfo[0].filename;
	String s = paths_FindExtension(r);
	return s.c_str() + _tcsspn(s.c_str(), _T("."));
}

/**
 * @brief Format Folder column data.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColPathGet(const CDiffContext *, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM*>(p);
	String s = di.diffFileInfo[1].path;
	const String &t = di.diffFileInfo[0].path;

	// If we have unique path, just print the existing path name
	if (s.length() == 0 || t.length() == 0)
	{
		if (s.length() == 0)
			return t;
		else
			return s;
	}

	int i = 0, j = 0;
	do
	{
		const TCHAR *pi = _tcschr(s.c_str() + i, '\\');
		const TCHAR *pj = _tcschr(t.c_str() + j, '\\');
		int i_ahead = pi ? pi - s.c_str() : std::string::npos;
		int j_ahead = pj ? pj - t.c_str() : std::string::npos;
		int length_s = (i_ahead != std::string::npos ? i_ahead : s.length()) - i;
		int length_t = (j_ahead != std::string::npos ? j_ahead : t.length()) - j;
		if (length_s != length_t ||
			!StrIsIntlEqual(FALSE, s.c_str() + i, t.c_str() + j, length_s))
		{
			String u(t.c_str() + j, length_t + 1);
			u[length_t] = '|';
			s.insert(i, u.c_str());
			i_ahead += u.length();
		}
		i = i_ahead + 1;
		j = j_ahead + 1;
	} while (i && j);
	if (s.empty())
		s = _T(".");
	return s;
}

/**
 * @brief Format Result column data.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColStatusGet(const CDiffContext *pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM*>(p);
	int nDirs = pCtxt->GetCompareDirs();
	// Note that order of items does matter. We must check for
	// skipped items before unique items, for example, so that
	// skipped unique items are labeled as skipped, not unique.
	String s;
	if (di.diffcode.isResultError())
	{
		s = _("Unable to compare files");
	}
	else if (di.diffcode.isResultAbort())
	{
		s = _("Item aborted");
	}
	else if (di.diffcode.isResultFiltered())
	{
		if (di.diffcode.isDirectory())
			s = _("Folder skipped");
		else
			s = _("File skipped");
	}
	else if (di.diffcode.isSideFirstOnly())
	{
		s = string_format_string1(_("Left only: %1"),
				di.getFilepath(0, pCtxt->GetNormalizedLeft()));
	}
	else if (di.diffcode.isSideSecondOnly())
	{
		if (nDirs < 3)
		{
			s = string_format_string1(_("Right only: %1"),
					di.getFilepath(1, pCtxt->GetNormalizedRight()));
		}
		else
		{
			s = string_format_string1(_("Middle only: %1"),
					di.getFilepath(1, pCtxt->GetNormalizedMiddle()));
		}
	}
	else if (di.diffcode.isSideThirdOnly())
	{
		s = string_format_string1(_("Right only: %1"),
				di.getFilepath(2, pCtxt->GetNormalizedRight()));
	}
	else if (nDirs > 2 && !di.diffcode.isExistsFirst())
	{
		s = string_format_string1(_("Does not exist in %1"),
				pCtxt->GetNormalizedLeft());
	}
	else if (nDirs > 2 && !di.diffcode.isExistsSecond())
	{
		s = string_format_string1(_("Does not exist in %1"),
				pCtxt->GetNormalizedMiddle());
	}
	else if (nDirs > 2 && !di.diffcode.isExistsThird())
	{
		s = string_format_string1(_("Does not exist in %1"),
				pCtxt->GetNormalizedRight());
	}
	else if (di.diffcode.isResultSame())
	{
		if (di.diffcode.isText())
			s = _("Text files are identical");
		else if (di.diffcode.isBin())
			s = _("Binary files are identical");
		else
			s = _("Identical");
	}
	else if (di.diffcode.isResultDiff()) // diff
	{
		if (di.diffcode.isText())
			s = _("Text files are different");
		else if (di.diffcode.isBin())
			s = _("Binary files are different");
		else if (di.diffcode.isDirectory())
			s = _("Folders are different");
		else
			s = _("Files are different");
	}
	return s;
}

/**
 * @brief Format Date column data.
 * @param [in] p Pointer to integer (seconds since 1.1.1970).
 * @return String to show in the column.
 */
static String ColTimeGet(const CDiffContext *, const void *p)
{
	const __int64 r = *static_cast<const __int64*>(p) / Timestamp::resolution();
	if (r)
		return locality::TimeString(&r);
	else
		return _T("");
}

/**
 * @brief Format Sizw column data.
 * @param [in] p Pointer to integer containing size in bytes.
 * @return String to show in the column.
 */
static String ColSizeGet(const CDiffContext *, const void *p)
{
	const __int64 &r = *static_cast<const __int64*>(p);
	String s;
	if (r != -1)
	{
		s = locality::NumToLocaleStr(r);
	}
	return s;
}

/**
 * @brief Format Folder column data.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColSizeShortGet(const CDiffContext *, const void *p)
{
	const __int64 &r = *static_cast<const __int64*>(p);
	String s;
	if (r != -1)
	{
		s = MakeShortSize(r);
	}
	return s;
}

/**
 * @brief Format Difference cout column data.
 * @param [in] p Pointer to integer having count of differences.
 * @return String to show in the column.
 */
static String ColDiffsGet(const CDiffContext *, const void *p)
{
	const int &r = *static_cast<const int*>(p);
	String s;
	if (r == CDiffContext::DIFFS_UNKNOWN_QUICKCOMPARE)
	{ // QuickCompare, unknown
		s = _T("*");
	}
	else if (r == CDiffContext::DIFFS_UNKNOWN)
	{ // Unique item
		s = _T("");
	}
	else
	{
		s = locality::NumToLocaleStr(r);
	}
	return s;
}

/**
 * @brief Format Newer/Older column data.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColNewerGet(const CDiffContext *pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	if (pCtxt->GetCompareDirs() < 3)
	{
		if (di.diffcode.isSideFirstOnly())
		{
			return _T("<*<");
		}
		if (di.diffcode.isSideSecondOnly())
		{
			return _T(">*>");
		}
		if (di.diffFileInfo[0].mtime != 0 && di.diffFileInfo[1].mtime != 0)
		{
			if (di.diffFileInfo[0].mtime > di.diffFileInfo[1].mtime)
			{
				return _T("<<");
			}
			if (di.diffFileInfo[0].mtime < di.diffFileInfo[1].mtime)
			{
				return _T(">>");
			}
			return _T("==");
		}
		return _T("***");
	}
	else
	{
		String res;
		int sortno[3] = {0, 1, 2};
		Timestamp sorttime[3] = {di.diffFileInfo[0].mtime, di.diffFileInfo[1].mtime, di.diffFileInfo[2].mtime};
		for (int i = 0; i < 3; i++)
		{
			for (int j = i; j < 3; j++)
			{
				if (sorttime[i] < sorttime[j])
				{
					swap(sorttime[i], sorttime[j]);
					swap(sortno[i], sortno[j]);
				}
			}
		}
		res = _T("LMR")[sortno[0]];
		res += sorttime[0] == sorttime[1] ? _T("==") : _T("<<");
		res += _T("LMR")[sortno[1]];
		res += sorttime[1] == sorttime[2] ? _T("==") : _T("<<");
		res += _T("LMR")[sortno[2]];
		return res;
	}
}

/**
 * @brief Format Version info to string.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] pdi Pointer to DIFFITEM.
 * @param [in] bLeft Is the item left-size item?
 * @return String proper to show in the GUI.
 */
static String GetVersion(const CDiffContext * pCtxt, const DIFFITEM * pdi, int nIndex)
{
	DIFFITEM & di = const_cast<DIFFITEM &>(*pdi);
	DiffFileInfo & dfi = di.diffFileInfo[nIndex];
	if (!dfi.bVersionChecked)
	{
		pCtxt->UpdateVersion(di, nIndex);
	}
	return dfi.version.GetFileVersionString();
}

/**
 * @brief Format Version column data (for left-side).
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColLversionGet(const CDiffContext * pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	return GetVersion(pCtxt, &di, 0);
}

/**
 * @brief Format Version column data (for right-side).
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColRversionGet(const CDiffContext * pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	return GetVersion(pCtxt, &di, 1);
}

/**
 * @brief Format Short Result column data.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColStatusAbbrGet(const CDiffContext *pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	const char *id = 0;
	int nDirs = pCtxt->GetCompareDirs();

	// Note that order of items does matter. We must check for
	// skipped items before unique items, for example, so that
	// skipped unique items are labeled as skipped, not unique.
	if (di.diffcode.isResultError())
	{
		id = N_("Error");
	}
	else if (di.diffcode.isResultAbort())
	{
		id = N_("Item aborted");
	}
	else if (di.diffcode.isResultFiltered())
	{
		if (di.diffcode.isDirectory())
			id = N_("Folder skipped");
		else
			id = N_("File skipped");
	}
	else if (di.diffcode.isSideFirstOnly())
	{
		id = N_("Left Only");
	}
	else if (di.diffcode.isSideSecondOnly())
	{
		if (nDirs < 3)
			id = N_("Left Only");
		else
			id = N_("Middle Only");
	}
	else if (di.diffcode.isSideThirdOnly())
	{
		id = N_("Right Only");
	}
	else if (nDirs > 2 && !di.diffcode.isExistsFirst())
	{
		id = N_("No item in left");
	}
	else if (nDirs > 2 && !di.diffcode.isExistsSecond())
	{
		id = N_("No item in middle");
	}
	else if (nDirs > 2 && !di.diffcode.isExistsThird())
	{
		id = N_("No item in right");
	}
	else if (di.diffcode.isResultSame())
	{
		id = N_("No item in right");
	}
	else if (di.diffcode.isResultDiff())
	{
		id = N_("Different");
	}

	return id ? tr(id) : _T("");
}

/**
 * @brief Format Binary column data.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColBinGet(const CDiffContext *, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);

	if (di.diffcode.isBin())
		return _T("*");
	else
		return _T("");
}

/**
 * @brief Format File Attributes column data.
 * @param [in] p Pointer to file flags class.
 * @return String to show in the column.
 */
static String ColAttrGet(const CDiffContext *, const void *p)
{
	const DiffFileFlags &r = *static_cast<const DiffFileFlags *>(p);
	return r.ToString();
}

/**
 * @brief Format File Encoding column data.
 * @param [in] p Pointer to file information.
 * @return String to show in the column.
 */
static String ColEncodingGet(const CDiffContext *, const void *p)
{
	const DiffFileInfo &r = *static_cast<const DiffFileInfo *>(p);
	return r.encoding.GetName();
}

/**
 * @brief Format EOL type to string.
 * @param [in] p Pointer to DIFFITEM.
 * @param [in] bLeft Are we formatting left-side file's data?
 * @return EOL type as as string.
 */
static String GetEOLType(const CDiffContext *, const void *p, int index)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	const DiffFileInfo & dfi = di.diffFileInfo[index];
	const FileTextStats &stats = dfi.m_textStats;

	if (stats.ncrlfs == 0 && stats.ncrs == 0 && stats.nlfs == 0)
	{
		return String();
	}
	if (di.diffcode.isBin())
	{
		return _("Binary");
	}

	char *id = 0;
	if (stats.ncrlfs > 0 && stats.ncrs == 0 && stats.nlfs == 0)
	{
		id = N_("Win");
	}
	else if (stats.ncrlfs == 0 && stats.ncrs > 0 && stats.nlfs == 0)
	{
		id = N_("Mac");
	}
	else if (stats.ncrlfs == 0 && stats.ncrs == 0 && stats.nlfs > 0)
	{
		id = N_("Unix");
	}
	else
	{
		return string_format(_T("%s:%d/%d/%d"),
			_("Mixed").c_str(),
			stats.ncrlfs, stats.ncrs, stats.nlfs);
	}
	
	return tr(id);
}

/**
 * @brief Format EOL type column data (for left-side file).
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColLEOLTypeGet(const CDiffContext * pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	return GetEOLType(pCtxt, &di, 0);
}

/**
 * @brief Format EOL type column data (for right-side file).
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM.
 * @return String to show in the column.
 */
static String ColMEOLTypeGet(const CDiffContext * pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	return GetEOLType(pCtxt, &di, 1);
}

static String ColREOLTypeGet(const CDiffContext * pCtxt, const void *p)
{
	const DIFFITEM &di = *static_cast<const DIFFITEM *>(p);
	return GetEOLType(pCtxt, &di, pCtxt->GetCompareDirs() < 3 ? 1 : 2);
}

/**
 * @}
 */

/**
 * @name Functions to sort each type of column info.
 * These functions are used to sort information in folder compare GUI. Each
 * column info (type) has its own function to compare the data. Each
 * function receives three parameters:
 * - pointer to compare context
 * - two parameters for data to compare (type varies)
 * Return value is -1, 0, or 1, where 0 means datas are identical.
 */
/* @{ */
/**
 * @brief Compare file names.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM having first name to compare.
 * @param [in] q Pointer to DIFFITEM having second name to compare.
 * @return Compare result.
 */
static int ColFileNameSort(const CDiffContext *pCtxt, const void *p, const void *q)
{
	const DIFFITEM &ldi = *static_cast<const DIFFITEM *>(p);
	const DIFFITEM &rdi = *static_cast<const DIFFITEM *>(q);
	if (ldi.diffcode.isDirectory() && !rdi.diffcode.isDirectory())
		return -1;
	if (!ldi.diffcode.isDirectory() && rdi.diffcode.isDirectory())
		return 1;
	return string_compare_nocase(ColFileNameGet(pCtxt, p), ColFileNameGet(pCtxt, q));
}

/**
 * @brief Compare file name extensions.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p First file name extension to compare.
 * @param [in] q Second file name extension to compare.
 * @return Compare result.
 */
static int ColExtSort(const CDiffContext *pCtxt, const void *p, const void *q)
{
	const DIFFITEM &ldi = *static_cast<const DIFFITEM *>(p);
	const DIFFITEM &rdi = *static_cast<const DIFFITEM *>(q);
	if (ldi.diffcode.isDirectory() && !rdi.diffcode.isDirectory())
		return -1;
	if (!ldi.diffcode.isDirectory() && rdi.diffcode.isDirectory())
		return 1;
	return string_compare_nocase(ColExtGet(pCtxt, p), ColExtGet(pCtxt, q));
}

/**
 * @brief Compare folder names.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM having first folder name to compare.
 * @param [in] q Pointer to DIFFITEM having second folder name to compare.
 * @return Compare result.
 */
static int ColPathSort(const CDiffContext *pCtxt, const void *p, const void *q)
{
	return string_compare_nocase(ColPathGet(pCtxt, p), ColPathGet(pCtxt, q));
}

/**
 * @brief Compare compare results.
 * @param [in] p Pointer to DIFFITEM having first result to compare.
 * @param [in] q Pointer to DIFFITEM having second result to compare.
 * @return Compare result.
 */
static int ColStatusSort(const CDiffContext *, const void *p, const void *q)
{
	const DIFFITEM &ldi = *static_cast<const DIFFITEM *>(p);
	const DIFFITEM &rdi = *static_cast<const DIFFITEM *>(q);
	return cmpdiffcode(rdi.diffcode.diffcode, ldi.diffcode.diffcode);
}

/**
 * @brief Compare file times.
 * @param [in] p First time to compare.
 * @param [in] q Second time to compare.
 * @return Compare result.
 */
static int ColTimeSort(const CDiffContext *, const void *p, const void *q)
{
	const __int64 &r = *static_cast<const __int64*>(p);
	const __int64 &s = *static_cast<const __int64*>(q);
	return cmp64(r, s);
}

/**
 * @brief Compare file sizes.
 * @param [in] p First size to compare.
 * @param [in] q Second size to compare.
 * @return Compare result.
 */
static int ColSizeSort(const CDiffContext *, const void *p, const void *q)
{
	const __int64 &r = *static_cast<const __int64*>(p);
	const __int64 &s = *static_cast<const __int64*>(q);
	return cmp64(r, s);
}

/**
 * @brief Compare difference counts.
 * @param [in] p First count to compare.
 * @param [in] q Second count to compare.
 * @return Compare result.
 */
static int ColDiffsSort(const CDiffContext *, const void *p, const void *q)
{
	const int &r = *static_cast<const int*>(p);
	const int &s = *static_cast<const int*>(q);
	return r - s;
}

/**
 * @brief Compare newer/older statuses.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM having first status to compare.
 * @param [in] q Pointer to DIFFITEM having second status to compare.
 * @return Compare result.
 */
static int ColNewerSort(const CDiffContext *pCtxt, const void *p, const void *q)
{
	return ColNewerGet(pCtxt, p) == ColNewerGet(pCtxt, q);
}

/**
 * @brief Compare left-side file versions.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM having first version to compare.
 * @param [in] q Pointer to DIFFITEM having second version to compare.
 * @return Compare result.
 */
static int ColLversionSort(const CDiffContext *pCtxt, const void *p, const void *q)
{
	return ColLversionGet(pCtxt, p) == ColLversionGet(pCtxt, q);
}

/**
 * @brief Compare right-side file versions.
 * @param [in] pCtxt Pointer to compare context.
 * @param [in] p Pointer to DIFFITEM having first version to compare.
 * @param [in] q Pointer to DIFFITEM having second version to compare.
 * @return Compare result.
 */
static int ColRversionSort(const CDiffContext *pCtxt, const void *p, const void *q)
{
	return ColRversionGet(pCtxt, p) == ColRversionGet(pCtxt, q);
}

/**
 * @brief Compare binary statuses.
 * This function returns a comparison of binary results.
 * @param [in] p Pointer to DIFFITEM having first status to compare.
 * @param [in] q Pointer to DIFFITEM having second status to compare.
 * @return Compare result:
 * - if both items are text files or binary files: 0
 * - if left is text and right is binary: -1
 * - if left is binary and right is text: 1
 */
static int ColBinSort(const CDiffContext *, const void *p, const void *q)
{
	const DIFFITEM &ldi = *static_cast<const DIFFITEM *>(p);
	const DIFFITEM &rdi = *static_cast<const DIFFITEM *>(q);
	const bool i = ldi.diffcode.isBin();
	const bool j = rdi.diffcode.isBin();

	if (!i && !j)
		return 0;
	else if (i && !j)
		return 1;
	else if (!i && j)
		return -1;
	else
		return 0;
}

/**
 * @brief Compare file flags.
 * @param [in] p Pointer to first flag structure to compare.
 * @param [in] q Pointer to second flag structure to compare.
 * @return Compare result.
 */
static int ColAttrSort(const CDiffContext *, const void *p, const void *q)
{
	const DiffFileFlags &r = *static_cast<const DiffFileFlags *>(p);
	const DiffFileFlags &s = *static_cast<const DiffFileFlags *>(q);
	return r.ToString() == s.ToString();
}

/**
 * @brief Compare file encodings.
 * @param [in] p Pointer to first structure to compare.
 * @param [in] q Pointer to second structure to compare.
 * @return Compare result.
 */
static int ColEncodingSort(const CDiffContext *, const void *p, const void *q)
{
	const DiffFileInfo &r = *static_cast<const DiffFileInfo *>(p);
	const DiffFileInfo &s = *static_cast<const DiffFileInfo *>(q);
	return FileTextEncoding::Collate(r.encoding, s.encoding);
}
/* @} */

/**
 * @brief All existing folder compare columns.
 *
 * This table has information for all folder compare columns. Fields are
 * (in this order):
 *  - internal name
 *  - name resource ID: column's name shown in header
 *  - description resource ID: columns description text
 *  - custom function for getting column data
 *  - custom function for sorting column data
 *  - parameter for custom functions: DIFFITEM (if NULL) or one of its fields
 *  - default column order number, -1 if not shown by default
 *  - ascending (TRUE) or descending (FALSE) default sort order
 *  - alignment of column contents: numbers are usually right-aligned
 */
static DirColInfo f_cols[] =
{
	{ _T("Name"), COLHDR_FILENAME, COLDESC_FILENAME, &ColFileNameGet, &ColFileNameSort, 0, 0, true, LVCFMT_LEFT },
	{ _T("Path"), COLHDR_DIR, COLDESC_DIR, &ColPathGet, &ColPathSort, 0, 1, true, LVCFMT_LEFT },
	{ _T("Status"), COLHDR_RESULT, COLDESC_RESULT, &ColStatusGet, &ColStatusSort, 0, 2, true, LVCFMT_LEFT },
	{ _T("Lmtime"), COLHDR_LTIMEM, COLDESC_LTIMEM, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].mtime), 3, false, LVCFMT_LEFT },
	{ _T("Rmtime"), COLHDR_RTIMEM, COLDESC_RTIMEM, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].mtime), 4, false, LVCFMT_LEFT },
	{ _T("Lctime"), COLHDR_LTIMEC, COLDESC_LTIMEC, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].ctime), -1, false, LVCFMT_LEFT },
	{ _T("Rctime"), COLHDR_RTIMEC, COLDESC_RTIMEC, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].ctime), -1, false, LVCFMT_LEFT },
	{ _T("Ext"), COLHDR_EXTENSION, COLDESC_EXTENSION, &ColExtGet, &ColExtSort, 0, 5, true, LVCFMT_LEFT },
	{ _T("Lsize"), COLHDR_LSIZE, COLDESC_LSIZE, &ColSizeGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].size), -1, false, LVCFMT_RIGHT },
	{ _T("Rsize"), COLHDR_RSIZE, COLDESC_RSIZE, &ColSizeGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].size), -1, false, LVCFMT_RIGHT },
	{ _T("LsizeShort"), COLHDR_LSIZE_SHORT, COLDESC_LSIZE_SHORT, &ColSizeShortGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].size), -1, false, LVCFMT_RIGHT },
	{ _T("RsizeShort"), COLHDR_RSIZE_SHORT, COLDESC_RSIZE_SHORT, &ColSizeShortGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].size), -1, false, LVCFMT_RIGHT },
	{ _T("Newer"), COLHDR_NEWER, COLDESC_NEWER, &ColNewerGet, &ColNewerSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Lversion"), COLHDR_LVERSION, COLDESC_LVERSION, &ColLversionGet, &ColLversionSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Rversion"), COLHDR_RVERSION, COLDESC_RVERSION, &ColRversionGet, &ColRversionSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("StatusAbbr"), COLHDR_RESULT_ABBR, COLDESC_RESULT_ABBR, &ColStatusAbbrGet, &ColStatusSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Binary"), COLHDR_BINARY, COLDESC_BINARY, &ColBinGet, &ColBinSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Lattr"), COLHDR_LATTRIBUTES, COLDESC_LATTRIBUTES, &ColAttrGet, &ColAttrSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].flags), -1, true, LVCFMT_LEFT },
	{ _T("Rattr"), COLHDR_RATTRIBUTES, COLDESC_RATTRIBUTES, &ColAttrGet, &ColAttrSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].flags), -1, true, LVCFMT_LEFT },
	{ _T("Lencoding"), COLHDR_LENCODING, COLDESC_LENCODING, &ColEncodingGet, &ColEncodingSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0]), -1, true, LVCFMT_LEFT },
	{ _T("Rencoding"), COLHDR_RENCODING, COLDESC_RENCODING, &ColEncodingGet, &ColEncodingSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1]), -1, true, LVCFMT_LEFT },
	{ _T("Snsdiffs"), COLHDR_NSDIFFS, COLDESC_NSDIFFS, ColDiffsGet, ColDiffsSort, FIELD_OFFSET(DIFFITEM, nsdiffs), -1, false, LVCFMT_RIGHT },
	{ _T("Snidiffs"), COLHDR_NIDIFFS, COLDESC_NIDIFFS, ColDiffsGet, ColDiffsSort, FIELD_OFFSET(DIFFITEM, nidiffs), -1, false, LVCFMT_RIGHT },
	{ _T("Leoltype"), COLHDR_LEOL_TYPE, COLDESC_LEOL_TYPE, &ColLEOLTypeGet, 0, 0, -1, true, LVCFMT_LEFT },
	{ _T("Reoltype"), COLHDR_REOL_TYPE, COLDESC_REOL_TYPE, &ColREOLTypeGet, 0, 0, -1, true, LVCFMT_LEFT },
};
static DirColInfo f_cols3[] =
{
	{ _T("Name"), COLHDR_FILENAME, COLDESC_FILENAME, &ColFileNameGet, &ColFileNameSort, 0, 0, true, LVCFMT_LEFT },
	{ _T("Path"), COLHDR_DIR, COLDESC_DIR, &ColPathGet, &ColPathSort, 0, 1, true, LVCFMT_LEFT },
	{ _T("Status"), COLHDR_RESULT, COLDESC_RESULT, &ColStatusGet, &ColStatusSort, 0, 2, true, LVCFMT_LEFT },
	{ _T("Lmtime"), COLHDR_LTIMEM, COLDESC_LTIMEM, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].mtime), 3, false, LVCFMT_LEFT },
	{ _T("Mmtime"), COLHDR_MTIMEM, COLDESC_MTIMEM, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].mtime), 4, false, LVCFMT_LEFT },
	{ _T("Rmtime"), COLHDR_RTIMEM, COLDESC_RTIMEM, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[2].mtime), 5, false, LVCFMT_LEFT },
	{ _T("Lctime"), COLHDR_LTIMEC, COLDESC_LTIMEC, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].ctime), -1, false, LVCFMT_LEFT },
	{ _T("Mctime"), COLHDR_MTIMEC, COLDESC_MTIMEC, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].ctime), -1, false, LVCFMT_LEFT },
	{ _T("Rctime"), COLHDR_RTIMEC, COLDESC_RTIMEC, &ColTimeGet, &ColTimeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[2].ctime), -1, false, LVCFMT_LEFT },
	{ _T("Ext"), COLHDR_EXTENSION, COLDESC_EXTENSION, &ColExtGet, &ColExtSort, 0, 6, true, LVCFMT_LEFT },
	{ _T("Lsize"), COLHDR_LSIZE, COLDESC_LSIZE, &ColSizeGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].size), -1, false, LVCFMT_RIGHT },
	{ _T("Msize"), COLHDR_MSIZE, COLDESC_MSIZE, &ColSizeGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].size), -1, false, LVCFMT_RIGHT },
	{ _T("Rsize"), COLHDR_RSIZE, COLDESC_RSIZE, &ColSizeGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[2].size), -1, false, LVCFMT_RIGHT },
	{ _T("LsizeShort"), COLHDR_LSIZE_SHORT, COLDESC_LSIZE_SHORT, &ColSizeShortGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].size), -1, false, LVCFMT_RIGHT },
	{ _T("MsizeShort"), COLHDR_MSIZE_SHORT, COLDESC_MSIZE_SHORT, &ColSizeShortGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].size), -1, false, LVCFMT_RIGHT },
	{ _T("RsizeShort"), COLHDR_RSIZE_SHORT, COLDESC_RSIZE_SHORT, &ColSizeShortGet, &ColSizeSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[2].size), -1, false, LVCFMT_RIGHT },
	{ _T("Newer"), COLHDR_NEWER, COLDESC_NEWER, &ColNewerGet, &ColNewerSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Lversion"), COLHDR_LVERSION, COLDESC_LVERSION, &ColLversionGet, &ColLversionSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Mversion"), COLHDR_MVERSION, COLDESC_MVERSION, &ColRversionGet, &ColRversionSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Rversion"), COLHDR_RVERSION, COLDESC_RVERSION, &ColRversionGet, &ColRversionSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("StatusAbbr"), COLHDR_RESULT_ABBR, COLDESC_RESULT_ABBR, &ColStatusAbbrGet, &ColStatusSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Binary"), COLHDR_BINARY, COLDESC_BINARY, &ColBinGet, &ColBinSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Lattr"), COLHDR_LATTRIBUTES, COLDESC_LATTRIBUTES, &ColAttrGet, &ColAttrSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0].flags), -1, true, LVCFMT_LEFT },
	{ _T("Mattr"), COLHDR_MATTRIBUTES, COLDESC_MATTRIBUTES, &ColAttrGet, &ColAttrSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1].flags), -1, true, LVCFMT_LEFT },
	{ _T("Rattr"), COLHDR_RATTRIBUTES, COLDESC_RATTRIBUTES, &ColAttrGet, &ColAttrSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[2].flags), -1, true, LVCFMT_LEFT },
	{ _T("Lencoding"), COLHDR_LENCODING, COLDESC_LENCODING, &ColEncodingGet, &ColEncodingSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[0]), -1, true, LVCFMT_LEFT },
	{ _T("Mencoding"), COLHDR_MENCODING, COLDESC_MENCODING, &ColEncodingGet, &ColEncodingSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[1]), -1, true, LVCFMT_LEFT },
	{ _T("Rencoding"), COLHDR_RENCODING, COLDESC_RENCODING, &ColEncodingGet, &ColEncodingSort, FIELD_OFFSET(DIFFITEM, diffFileInfo[2]), -1, true, LVCFMT_LEFT },
	{ _T("Snsdiffs"), COLHDR_NSDIFFS, COLDESC_NSDIFFS, ColDiffsGet, ColDiffsSort, FIELD_OFFSET(DIFFITEM, nsdiffs), -1, false, LVCFMT_RIGHT },
	{ _T("Snidiffs"), COLHDR_NIDIFFS, COLDESC_NIDIFFS, ColDiffsGet, ColDiffsSort, FIELD_OFFSET(DIFFITEM, nidiffs), -1, false, LVCFMT_RIGHT },
	{ _T("Leoltype"), COLHDR_LEOL_TYPE, COLDESC_LEOL_TYPE, &ColLEOLTypeGet, &ColAttrSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Meoltype"), COLHDR_MEOL_TYPE, COLDESC_MEOL_TYPE, &ColMEOLTypeGet, &ColAttrSort, 0, -1, true, LVCFMT_LEFT },
	{ _T("Reoltype"), COLHDR_REOL_TYPE, COLDESC_REOL_TYPE, &ColREOLTypeGet, &ColAttrSort, 0, -1, true, LVCFMT_LEFT },
};

/**
 * @brief Count of all known columns
 */
const int g_ncols = countof(f_cols);
const int g_ncols3 = countof(f_cols3);

/**
 * @brief Registry base value name for saving/loading info for this column
 */
String
CDirView::GetColRegValueNameBase(int col) const
{
	if (GetDocument()->m_nDirs < 3)
	{
		ASSERT(col>=0 && col<countof(f_cols));
		return string_format(_T("WDirHdr_%s"), f_cols[col].regName);
	}
	else
	{
		ASSERT(col>=0 && col<countof(f_cols3));
		return string_format(_T("WDirHdr_%s"), f_cols3[col].regName);
	}
}

/**
 * @brief Get default physical order for specified logical column
 */
int
CDirView::GetColDefaultOrder(int col) const
{
	if (GetDocument()->m_nDirs < 3)
	{
		ASSERT(col>=0 && col<countof(f_cols));
		return f_cols[col].physicalIndex;
	}
	else
	{
		ASSERT(col>=0 && col<countof(f_cols3));
		return f_cols3[col].physicalIndex;
	}
}

/**
 * @brief Return the info about the specified physical column
 */
const DirColInfo *
CDirView::DirViewColItems_GetDirColInfo(int col) const
{
	if (GetDocument()->m_nDirs < 3)
	{
		if (col < 0 || col >= countof(f_cols))
		{
			ASSERT(0); // fix caller, should not ask for nonexistent columns
			return 0;
		}
		return &f_cols[col];
	}
	else
	{
		if (col < 0 || col >= countof(f_cols3))
		{
			ASSERT(0); // fix caller, should not ask for nonexistent columns
			return 0;
		}
		return &f_cols3[col];
	}
}

/**
 * @brief Check if specified physical column has specified resource id name
 */
bool
CDirView::IsColById(int col, const char *idname) const
{
	int nDirs = GetDocument()->m_nDirs;
	if (nDirs < 3)
	{
		if (col < 0 || col >= countof(f_cols))
		{
			ASSERT(0); // fix caller, should not ask for nonexistent columns
			return false;
		}
		return f_cols[col].idName == idname;
	}
	else
	{
		if (col < 0 || col >= sizeof(f_cols3)/sizeof(f_cols3[0]))
		{
			ASSERT(0); // fix caller, should not ask for nonexistent columns
			return false;
		}
		return f_cols3[col].idName == idname;
	}
}

/**
 * @brief Is specified physical column the name column?
 */
bool
CDirView::IsColName(int col) const
{
	return IsColById(col, COLHDR_FILENAME);
}
/**
 * @brief Is specified physical column the left modification time column?
 */
bool
CDirView::IsColLmTime(int col) const
{
	return IsColById(col, COLHDR_LTIMEM);
}
/**
 * @brief Is specified physical column the middle modification time column?
 */
bool
CDirView::IsColMmTime(int col) const
{
	return IsColById(col, COLHDR_MTIMEM);
}
/**
 * @brief Is specified physical column the right modification time column?
 */
bool
CDirView::IsColRmTime(int col) const
{
	return IsColById(col, COLHDR_RTIMEM);
}
/**
 * @brief Is specified physical column the full status (result) column?
 */
bool
CDirView::IsColStatus(int col) const
{
	return IsColById(col, COLHDR_RESULT);
}
/**
 * @brief Is specified physical column the full status (result) column?
 */
bool
CDirView::IsColStatusAbbr(int col) const
{
	return IsColById(col, COLHDR_RESULT_ABBR);
}
