/**
 *  @file DirScan.cpp
 *
 *  @brief Implementation of DirScan (q.v.) and helper functions
 */ 
// RCS ID line follows -- this is updated by CVS
// $Id$

#include "stdafx.h"
#include <shlwapi.h>
#include <sys/stat.h>
#include "DirScan.h"
#include "CompareStats.h"
#include "common/unicoder.h"
#include "DiffContext.h"
#include "DiffWrapper.h"
#include "FileFilterHelper.h"
#include "logfile.h"
#include "paths.h"
#include "FileTransform.h"
#include "codepage.h"
#include "DiffItemList.h"
#include "PathContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Static types (ie, types only used locally)
/**
 * @brief directory or file info for one row in diff result
 * @note times are seconds since January 1, 1970.
 */
struct fentry
{
	CString name;
	// storing __time_t if MSVC6 (__MSC_VER<1300)
	// storing __time64_t if MSVC7 (VC.NET)
	__int64 mtime; /**< Last modify time */
	__int64 ctime; /**< Creation modify time */
	__int64 size;
	int attrs;
};
typedef CArray<fentry, fentry&> fentryArray;

// Static functions (ie, functions only used locally)
void CompareDiffItem(DIFFITEM di, CDiffContext * pCtxt);
static void LoadFiles(const CString & sDir, fentryArray * dirs, fentryArray * files);
void LoadAndSortFiles(const CString & sDir, fentryArray * dirs, fentryArray * files, bool casesensitive);
static void Sort(fentryArray * dirs, bool casesensitive);;
static int collstr(const CString & s1, const CString & s2, bool casesensitive);
static void StoreDiffResult(DIFFITEM &di, CDiffContext * pCtxt,
		const DiffFileData * pDiffFileData);
static void AddToList(CString sDir, const fentry * lent, const fentry * rent,
	int code, DiffItemList * pList, CDiffContext *pCtxt);
static void UpdateDiffItem(DIFFITEM & di, BOOL & bExists, CDiffContext *pCtxt);

/** @brief cmpmth is a typedef for a pointer to a method */
typedef int (CString::*cmpmth)(LPCTSTR sz) const;
/** @brief CALL_MEMBER_FN calls a method through a pointer to a method */
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))


/**
 * @brief Collect file- and directory-names to list.
 * 
 * @param [in] paths Root paths of compare
 * @param [in] subdir Current subdirectory under root paths
 * @param [in,out] list List where found items are added
 * @param [in] casesensitive Is filename compare casesensitive?
 * @param [in] depth Levels of subdirectories to scan, -1 scans all
 * @param [in] pCtxt Compare context
 * @param [in] piAbortable Interface allowing compare to be aborted
 * @return 1 normally, -1 if compare was aborted
 */
int DirScan_GetItems(const PathContext &paths, const CString & subdir, DiffItemList *pList,
		bool casesensitive, int depth, CDiffContext * pCtxt, IAbortable * piAbortable)
{
	static const TCHAR backslash[] = _T("\\");

	CString sLeftDir = paths.GetLeft();
	CString sRightDir = paths.GetRight();
	CString subprefix;
	if (!subdir.IsEmpty())
	{
		sLeftDir += backslash + subdir;
		sRightDir += backslash + subdir;
		subprefix = subdir + backslash;
	}

	fentryArray leftDirs, leftFiles, rightDirs, rightFiles;
	LoadAndSortFiles(sLeftDir, &leftDirs, &leftFiles, casesensitive);
	LoadAndSortFiles(sRightDir, &rightDirs, &rightFiles, casesensitive);

	// Allow user to abort scanning
	if (piAbortable && piAbortable->ShouldAbort())
		return -1;

	// Handle directories
	// i points to current directory in left list (leftDirs)
	// j points to current directory in right list (rightDirs)
	int i=0, j=0;
	while (1)
	{
		if (piAbortable && piAbortable->ShouldAbort()) return -1;

		// In debug mode, send current status to debug window
		if (i<leftDirs.GetSize())
			TRACE(_T("Candidate left: leftDirs[i]=%s\n"), (LPCTSTR)leftDirs[i].name);
		if (j<rightDirs.GetSize())
			TRACE(_T("Candidate right: rightDirs[j]=%s\n"), (LPCTSTR)rightDirs[j].name);
		if (i<leftDirs.GetSize() && (j==rightDirs.GetSize() || collstr(leftDirs[i].name, rightDirs[j].name, casesensitive)<0))
		{
			int nDiffCode = DIFFCODE::LEFT | DIFFCODE::DIR;

			// Advance left pointer over left-only entry, and then retest with new pointers
			AddToList(subdir, &leftDirs[i], 0, nDiffCode, pList, pCtxt);
			++i;
			continue;
		}
		if (j<rightDirs.GetSize() && (i==leftDirs.GetSize() || collstr(leftDirs[i].name, rightDirs[j].name, casesensitive)>0))
		{
			int nDiffCode = DIFFCODE::RIGHT | DIFFCODE::DIR;

			// Advance right pointer over right-only entry, and then retest with new pointers
			AddToList(subdir, 0, &rightDirs[j], nDiffCode, pList, pCtxt);
			++j;
			continue;
		}
		if (i<leftDirs.GetSize())
		{
			ASSERT(j<rightDirs.GetSize());
			CString newsub = subprefix + leftDirs[i].name;
			{
				int nDiffCode = DIFFCODE::BOTH | DIFFCODE::DIR;
				if (!depth)
				{
					// Non-recursive compare
					// We are only interested about list of subdirectories to show - user can open them
					// TODO: scan one level deeper to see if directories are identical/different
					AddToList(subdir, &leftDirs[i], &rightDirs[j], nDiffCode, pList, pCtxt);
				}
				else
				{
					// Recursive compare
					// Test against filter so we don't include contents of filtered out directories
					// Also this is only place we can test for both-sides directories in recursive compare
					if (!pCtxt->m_piFilterGlobal->includeDir(newsub))
					{
						nDiffCode |= DIFFCODE::SKIPPED;
						AddToList(subdir, &leftDirs[i], &rightDirs[j], nDiffCode, pList, pCtxt);
					}
					else
					{
						// Scan recursively all subdirectories too, we are not adding folders
						nDiffCode |= DIFFCODE::INCLUDED;

						if (DirScan_GetItems(paths, newsub, pList, casesensitive,
								depth - 1, pCtxt, piAbortable) == -1)
						{
							return -1;
						}
					}
				}
			}
			++i;
			++j;
			continue;
		}
		break;
	}
	// Handle files
	// i points to current file in left list (leftFiles)
	// j points to current file in right list (rightFiles)
	i=0, j=0;
	while (1)
	{
		if (piAbortable && piAbortable->ShouldAbort())
			return -1;

		// In debug mode, send current status to debug window
		if (i<leftFiles.GetSize())
			TRACE(_T("Candidate left: leftFiles[i]=%s\n"), (LPCTSTR)leftFiles[i].name);
		if (j<rightFiles.GetSize())
			TRACE(_T("Candidate right: rightFiles[j]=%s\n"), (LPCTSTR)rightFiles[j].name);
		
		if (i<leftFiles.GetSize() && (j==rightFiles.GetSize() ||
				collstr(leftFiles[i].name, rightFiles[j].name, casesensitive) < 0))
		{
			// Test against filter
			CString newsubfile = subprefix + leftFiles[i].name;
			int nDiffCode = DIFFCODE::LEFT | DIFFCODE::FILE;
			{
				AddToList(subdir, &leftFiles[i], 0, nDiffCode, pList, pCtxt);
			}

			// Advance left pointer over left-only entry, and then retest with new pointers
			++i;
			continue;
		}
		if (j<rightFiles.GetSize() && (i==leftFiles.GetSize() ||
				collstr(leftFiles[i].name, rightFiles[j].name, casesensitive) > 0))
		{
			// Test against filter
			CString newsubfile = subprefix + rightFiles[j].name;
			int nDiffCode = DIFFCODE::RIGHT | DIFFCODE::FILE;
			{
				AddToList(subdir, 0, &rightFiles[j], nDiffCode, pList, pCtxt);
			}
			// Advance right pointer over right-only entry, and then retest with new pointers
			++j;
			continue;
		}
		if (i<leftFiles.GetSize())
		{
			ASSERT(j<rightFiles.GetSize());
			CString newsubfile = subprefix + leftFiles[i].name;
			int nDiffCode = DIFFCODE::BOTH | DIFFCODE::FILE;

			AddToList(subdir, &leftFiles[i], &rightFiles[j], nDiffCode, pList, pCtxt);

			++i;
			++j;
			continue;
		}
		break;
	}
	return 1;
}

/**
 * @brief Compare DiffItems in list and add results to compare context.
 *
 * @param list [in] List of items to compare
 * @param pCtxt [in,out] Compare context: contains list where results are added.
 * @param piAbortable [in] Interface allowing to abort compare
 * @return 1 if compare finished, -1 if compare was aborted
 */
int DirScan_CompareItems(DiffItemList & list, CDiffContext * pCtxt, IAbortable * piAbortable)
{
	int res = 1;
	POSITION pos = list.GetFirstDiffPosition();
	
	while (pos != NULL)
	{
		if (piAbortable && piAbortable->ShouldAbort())
		{
			res = -1;
			break;
		}

		DIFFITEM di = list.GetNextDiffPosition(pos);
		CompareDiffItem(di, pCtxt);
	}
	return res;
}

/**
 * @brief Compare DiffItems in context marked for rescan.
 *
 * @param pCtxt [in,out] Compare context: contains list of items.
 * @param piAbortable [in] Interface allowing to abort compare
 * @return 1 if compare finished, -1 if compare was aborted
 */
int DirScan_CompareItems(CDiffContext * pCtxt, IAbortable * piAbortable)
{
	int res = 1;
	POSITION pos = pCtxt->GetFirstDiffPosition();
	
	while (pos != NULL)
	{
		if (piAbortable && piAbortable->ShouldAbort())
		{
			res = -1;
			break;
		}

		POSITION oldPos = pos;
		DIFFITEM di = pCtxt->GetNextDiffPosition(pos);
		if (di.isScanNeeded())
		{
			BOOL bItemsExist = TRUE;
			pCtxt->RemoveDiff(oldPos);
			UpdateDiffItem(di, bItemsExist, pCtxt);
			if (bItemsExist)
				CompareDiffItem(di, pCtxt);
		}
	}
	return res;
}

/**
 * @brief Update diffitem file/dir infos.
 *
 * Re-tests dirs/files if sides still exists, and updates infos for
 * existing sides. This assumes filenames, or paths are not changed.
 * Since in normal situations (I can think of) they cannot change
 * after first compare.
 *
 * @param [in,out] di DiffItem to update.
 * @param [out] bExists Set to
 *  - TRUE if one of items exists so diffitem is valid
 *  - FALSE if items were deleted, so diffitem is not valid
 * @param [in] pCtxt Compare context
 */
void UpdateDiffItem(DIFFITEM & di, BOOL & bExists, CDiffContext *pCtxt)
{
	struct _stat stats;
	CString leftpath;
	CString rightpath;
	BOOL bLeftExists = FALSE;
	BOOL bRightExists = FALSE;

	bExists = TRUE;
	leftpath = di.getLeftFilepath(pCtxt->GetNormalizedLeft());
	leftpath = paths_ConcatPath(leftpath, di.sfilename);
	rightpath = di.getRightFilepath(pCtxt->GetNormalizedRight());
	rightpath = paths_ConcatPath(rightpath, di.sfilename);
	
	// Re-check if left/right sides still exists (or are added)
	if (_tstat(leftpath, &stats) == 0)
		bLeftExists = TRUE;
	if (_tstat(rightpath, &stats) == 0)
		bRightExists = TRUE;

	// Clear side-info and file-infos
	di.diffcode &= ~DIFFCODE::SIDEFLAG;
	di.left.Clear();
	di.right.Clear();

	// Update infos for existing sides
	if (bLeftExists && bRightExists)
	{
		di.diffcode |= DIFFCODE::BOTH;
		pCtxt->UpdateInfoFromDiskHalf(di, di.left);
		pCtxt->UpdateInfoFromDiskHalf(di, di.right);
	}
	else if (bLeftExists && !bRightExists)
	{
		di.diffcode |= DIFFCODE::LEFT;
		pCtxt->UpdateInfoFromDiskHalf(di, di.left);
	}
	else if (!bLeftExists && bRightExists)
	{
		di.diffcode |= DIFFCODE::RIGHT;
		pCtxt->UpdateInfoFromDiskHalf(di, di.right);
	}
	else if (!bLeftExists && !bRightExists)
	{
		bExists = FALSE;
	}
}

/**
 * @brief Compare two diffitems and add results to difflist in context.
 *
 * @param [in] di DiffItem to compare
 * @param [in,out] pCtxt Compare context: contains difflist, encoding info etc.
 */
void CompareDiffItem(DIFFITEM di, CDiffContext * pCtxt)
{
	// Clear possible rescan-flag
	di.diffcode &= ~DIFFCODE::NEEDSCAN;

	// 1. Test against filters
	if (di.isDirectory())
	{
		if (!pCtxt->m_piFilterGlobal->includeDir(di.sfilename))
			di.diffcode |= DIFFCODE::SKIPPED;
		else
			di.diffcode |= DIFFCODE::INCLUDED;

		// Done for directories now
		StoreDiffResult(di, pCtxt, NULL);
		return;
	}
	else
	{
		if (!pCtxt->m_piFilterGlobal->includeFile(di.sfilename))
			di.diffcode |= DIFFCODE::SKIPPED;
		else
			di.diffcode |= DIFFCODE::INCLUDED;
	}

	// Add unique files
	if (di.isSideLeft())
	{
		if (pCtxt->m_nCompMethod != CMP_DATE)
		{
			// Compare file to itself to detect encoding
			DiffFileData diffdata;
			diffdata.prepAndCompareTwoFiles(pCtxt, di);
			StoreDiffResult(di, pCtxt, &diffdata);
		}
		else
		{
			StoreDiffResult(di, pCtxt, NULL);
		}
		return;
	}
	else if (di.isSideRight())
	{
		if (pCtxt->m_nCompMethod != CMP_DATE)
		{
			// Compare file to itself to detect encoding
			DiffFileData diffdata;
			diffdata.prepAndCompareTwoFiles(pCtxt, di);
			StoreDiffResult(di, pCtxt, &diffdata);

		}
		else
		{
			StoreDiffResult(di, pCtxt, NULL);
		}
		return;
	}
	// Compare two files
	else
	{
		/*LPCTSTR leftname = leftFiles[i].name;
		LPCTSTR rightname = rightFiles[j].name;

		gLog.Write(_T("Comparing: n0=%s, n1=%s, d0=%s, d1=%s"), 
		leftname, rightname, (LPCTSTR)sLeftDir, (LPCTSTR)sRightDir);
		*/

		if (pCtxt->m_nCompMethod == CMP_DATE)
		{
			// Compare by modified date
			__int64 nTimeDiff = di.left.mtime - di.right.mtime;
			// Remove sign
			nTimeDiff = (nTimeDiff > 0 ? nTimeDiff : -nTimeDiff);
			if (pCtxt->m_bIgnoreSmallTimeDiff)
			{
				// If option to ignore small timediffs (couple of seconds)
				// is set, decrease absolute difference by allowed diff
				nTimeDiff -= SmallTimeDiff;
			}
			if (nTimeDiff <= 0)
				di.diffcode |= DIFFCODE::TEXT | DIFFCODE::SAME;
			else
				di.diffcode |= DIFFCODE::TEXT | DIFFCODE::DIFF;
			// report result back to caller
			StoreDiffResult(di, pCtxt, NULL);
			return;
		}
		// Really compare
		DiffFileData diffdata;
		di.diffcode |= diffdata.prepAndCompareTwoFiles(pCtxt, di);
		// report result back to caller
		StoreDiffResult(di, pCtxt, &diffdata);
	}
				
	return;
}

/**
 * @brief Load arrays with all directories & files in specified dir
 */
void LoadAndSortFiles(const CString & sDir, fentryArray * dirs, fentryArray * files, bool casesensitive)
{
	LoadFiles(sDir, dirs, files);
	Sort(dirs, casesensitive);
	Sort(files, casesensitive);
}

/**
 * @brief Load arrays with all directories & files in specified dir
 */
void LoadFiles(const CString & sDir, fentryArray * dirs, fentryArray * files)
{
	CString sPattern = sDir;
	sPattern.TrimRight(_T("\\"));
	sPattern += _T("\\*.*");

	WIN32_FIND_DATA ff;
	HANDLE h = FindFirstFile(sPattern, &ff);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			DWORD dwIsDirectory = ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			if (dwIsDirectory && StrStr(_T(".."), ff.cFileName))
				continue;
			fentry ent;
			// Save filetimes as seconds since January 1, 1970
			ent.ctime = CTime(ff.ftCreationTime).GetTime();
			ent.mtime = CTime(ff.ftLastWriteTime).GetTime();
			if (!dwIsDirectory)
				ent.size = ff.nFileSizeLow + (ff.nFileSizeHigh << 32);
			else
				ent.size = -1;  // No size for directories
			ent.name = ff.cFileName;
			ent.attrs = ff.dwFileAttributes;
			(dwIsDirectory ? dirs : files) -> Add(ent);
		} while (FindNextFile(h, &ff));
		FindClose(h);
	}
}

/**
 * @brief case-sensitive collate function for qsorting an array
 */
static int __cdecl cmpstring(const void *elem1, const void *elem2)
{
	const fentry * s1 = static_cast<const fentry *>(elem1);
	const fentry * s2 = static_cast<const fentry *>(elem2);
	return s1->name.Collate(s2->name);
}

/**
 * @brief case-insensitive collate function for qsorting an array
 */
static int __cdecl cmpistring(const void *elem1, const void *elem2)
{
	const fentry * s1 = static_cast<const fentry *>(elem1);
	const fentry * s2 = static_cast<const fentry *>(elem2);
	return s1->name.CollateNoCase(s2->name);
}

/**
 * @brief sort specified array
 */
void Sort(fentryArray * dirs, bool casesensitive)
{
	fentry * data = dirs->GetData();
	if (!data) return;
	int (__cdecl *comparefnc)(const void *elem1, const void *elem2) = (casesensitive ? cmpstring : cmpistring);
	qsort(data, dirs->GetSize(), sizeof(dirs->GetAt(0)), comparefnc);
}

/**
 * @brief  Compare (NLS aware) two strings, either case-sensitive or case-insensitive as caller specifies
 */
static int collstr(const CString & s1, const CString & s2, bool casesensitive)
{
	if (casesensitive)
		return s1.Collate(s2);
	else
		return s1.CollateNoCase(s2);
}

/**
 * @brief Send one file or directory result back through the diff context
 */
static void StoreDiffResult(DIFFITEM &di, CDiffContext * pCtxt,
		const DiffFileData * pDiffFileData)
{
	if (pDiffFileData)
	{
		di.nsdiffs = pDiffFileData->m_ndiffs - pDiffFileData->m_ntrivialdiffs;
		di.ndiffs = pDiffFileData->m_ndiffs;

		if (!di.isSideLeft())
		{
			di.right.unicoding = pDiffFileData->m_sFilepath[1].unicoding;
			di.right.codepage = pDiffFileData->m_sFilepath[1].codepage;
		}
		
		if (!di.isSideRight())
		{
			di.left.unicoding = pDiffFileData->m_sFilepath[0].unicoding;
			di.left.codepage = pDiffFileData->m_sFilepath[0].codepage;
		}
	}

	gLog.Write
	(
		LOGLEVEL::LCOMPAREDATA, _T("name=<%s>, leftdir=<%s>, rightdir=<%s>, code=%d"),
		(LPCTSTR)di.sfilename, (LPCTSTR)_T("di.left.spath"), (LPCTSTR)_T("di.right.spath"), di.diffcode
	);
	pCtxt->AddDiff(di);
}

/**
 * @brief Add one compare item to list.
 */
static void AddToList(CString sDir, const fentry * lent, const fentry * rent,
	int code, DiffItemList * pList, CDiffContext *pCtxt)
{
	// We must store both paths - we cannot get paths later
	// and we need unique item paths for example when items
	// change to identical

	DIFFITEM di;
	di.left.unicoding = 0;
	di.left.codepage = 0;
	di.right.unicoding = 0;
	di.right.codepage = 0;

	if (!sDir.IsEmpty())
		di.sSubdir = sDir;

	if (lent)
	{
		di.sfilename = lent->name;
		di.left.mtime = lent->mtime;
		di.left.ctime = lent->ctime;
		di.left.size = lent->size;
		di.left.flags.attributes = lent->attrs;
	}

	if (rent)
	{
		di.sfilename = rent->name;
		di.right.mtime = rent->mtime;
		di.right.ctime = rent->ctime;
		di.right.size = rent->size;
		di.right.flags.attributes = rent->attrs;
	}

	di.diffcode = code;

	gLog.Write
	(
		LOGLEVEL::LCOMPAREDATA, _T("name=<%s>, leftdir=<%s>, rightdir=<%s>, code=%d"),
		(LPCTSTR)di.sfilename, (LPCTSTR)_T("di.left.spath"), (LPCTSTR)_T("di.right.spath"), code
	);
	pCtxt->m_pCompareStats->IncreaseTotalItems();
	pList->AddDiff(di);
}

void // static
DirScan_InitializeDefaultCodepage()
{
	// Set thread default codepage
	// This is application-wide initialization
	// but neither MainFrame nor MergeApp included the needed headers
	DiffFileData::SetDefaultCodepage(getDefaultCodepage());
}

