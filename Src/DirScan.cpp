/**
 *  @file DirScan.cpp
 *
 *  @brief Implementation of DirScan (q.v.) and helper functions
 */ 
// RCS ID line follows -- this is updated by CVS
// $Id$

#include "stdafx.h"
#include "DirScan.h"
#include "DiffContext.h"
#include "logfile.h"
#include "paths.h"
#include "FileTransform.h"

extern bool just_compare_files (LPCTSTR filepath1, LPCTSTR filepath2, int depth, bool * diff, bool * bin);
extern CLogFile gLog;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Static types (ie, types only used locally)
/**
 * @brief directory or file info for one row in diff result
 */
struct fentry
{
	CString name;
	// storing __time_t if MSVC6 (__MSC_VER<1300)
	// storing __time64_t if MSVC7 (VC.NET)
	__int64 mtime;
	__int64 ctime;
	_int64 size;
	int attrs;
};
typedef CArray<fentry, fentry&> fentryArray;

// Static functions (ie, functions only used locally)
static void LoadFiles(const CString & sDir, fentryArray * dirs, fentryArray * files);
void LoadAndSortFiles(const CString & sDir, fentryArray * dirs, fentryArray * files, bool casesensitive);
static void Sort(fentryArray * dirs, bool casesensitive);;
static int collstr(const CString & s1, const CString & s2, bool casesensitive);
static void StoreDiffResult(const CString & sDir, const fentry * lent, const fentry *rent, int code, CDiffContext * pCtxt);
static int prepAndCompareTwoFiles(const fentry & lent, const fentry & rent, const CString & sLeftDir, const CString & sRightDir);


/** @brief cmpmth is a typedef for a pointer to a method */
typedef int (CString::*cmpmth)(LPCTSTR sz) const;
/** @brief CALL_MEMBER_FN calls a method through a pointer to a method */
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))


/**
 * @brief Compare two directories and store results (via the context)
 *
 * Compare two directories & output all results found via calls to StoreDiffResult
 * base directories to compare are in the CDiffContext
 * and this call is responsible for diff'ing just the subdir specified
 * (ie, if subdir is empty, this is the base call)
 * return 1 normally, return -1 if aborting
 */
int DirScan(const CString & subdir, CDiffContext * pCtxt, bool casesensitive,
	int depth, IAbortable * piAbortable)
{
	CString sLeftDir = pCtxt->m_strNormalizedLeft;
	CString sRightDir = pCtxt->m_strNormalizedRight;
	CString subprefix;
	if (!subdir.IsEmpty())
	{
		sLeftDir += _T("\\") + subdir;
		sRightDir += _T("\\") + subdir;
		subprefix = subdir + _T("\\");
	}

	fentryArray leftDirs, leftFiles, rightDirs, rightFiles;
	LoadAndSortFiles(sLeftDir, &leftDirs, &leftFiles, casesensitive);
	LoadAndSortFiles(sRightDir, &rightDirs, &rightFiles, casesensitive);

	if (piAbortable && piAbortable->ShouldAbort()) return -1;

	// Handle directories
	// i points to current directory in left list (leftDirs)
	// j points to current directory in right list (rightDirs)
	int i=0, j=0;
	while (1)
	{
		if (piAbortable && piAbortable->ShouldAbort()) return -1;

		// In debug mode, send current status to debug window
		if (i<leftDirs.GetSize())
			TRACE(_T("Candidate left: leftDirs[i]=%s\n"), leftDirs[i]);
		if (j<rightDirs.GetSize())
			TRACE(_T("Candidate right: rightDirs[j]=%s\n"), rightDirs[j]);
		if (i<leftDirs.GetSize() && (j==rightDirs.GetSize() || collstr(leftDirs[i].name, rightDirs[j].name, casesensitive)<0))
		{
			int nDiffCode = DIFFCODE::LEFT + DIFFCODE::DIR;

			// Test against filter
			// If comparing non-recursively filter out subdirectories
			CString newsub = subprefix + leftDirs[i].name;
			if (!depth || !pCtxt->m_piFilterUI->includeDir(newsub) || !pCtxt->m_piFilterGlobal->includeDir(newsub))
				nDiffCode |= DIFFCODE::SKIPPED;

			// Advance left pointer over left-only entry, and then retest with new pointers
			StoreDiffResult(subdir, &leftDirs[i], 0, nDiffCode, pCtxt);
			++i;
			continue;
		}
		if (j<rightDirs.GetSize() && (i==leftDirs.GetSize() || collstr(leftDirs[i].name, rightDirs[j].name, casesensitive)>0))
		{
			int nDiffCode = DIFFCODE::RIGHT + DIFFCODE::DIR;

			// Test against filter
			// If comparing non-recursively filter out subdirectories
			CString newsub = subprefix + rightDirs[j].name;
			if (!depth || !pCtxt->m_piFilterUI->includeDir(newsub) || !pCtxt->m_piFilterGlobal->includeDir(newsub))
				nDiffCode |= DIFFCODE::SKIPPED;

			// Advance right pointer over right-only entry, and then retest with new pointers
			StoreDiffResult(subdir, 0, &rightDirs[j], nDiffCode, pCtxt);
			++j;
			continue;
		}
		if (i<leftDirs.GetSize())
		{
			ASSERT(j<rightDirs.GetSize());
			CString newsub = subprefix + leftDirs[i].name;
			if (!depth || !pCtxt->m_piFilterUI->includeDir(newsub) || !pCtxt->m_piFilterGlobal->includeDir(newsub))
			{
				StoreDiffResult(subdir, &leftDirs[i], &rightDirs[j], DIFFCODE::SKIPPED+DIFFCODE::DIR, pCtxt);
			}
			else
			{
				if (DirScan(newsub, pCtxt, casesensitive, depth-1, piAbortable) == -1)
					return -1;
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
		if (piAbortable && piAbortable->ShouldAbort()) return -1;

		// In debug mode, send current status to debug window
		if (i<leftFiles.GetSize())
			TRACE(_T("Candidate left: leftFiles[i]=%s\n"), leftFiles[i]);
		if (j<rightFiles.GetSize())
			TRACE(_T("Candidate right: rightFiles[j]=%s\n"), rightFiles[j]);
		if (i<leftFiles.GetSize() && (j==rightFiles.GetSize() || collstr(leftFiles[i].name, rightFiles[j].name, casesensitive)<0))
		{
			int nDiffCode = DIFFCODE::LEFT + DIFFCODE::FILE;

			// Test against filter
			CString newsubfile = subprefix + leftFiles[i].name;
			if (!pCtxt->m_piFilterUI->includeFile(newsubfile) || !pCtxt->m_piFilterGlobal->includeFile(newsubfile))
				nDiffCode |= DIFFCODE::SKIPPED;

			// Advance left pointer over left-only entry, and then retest with new pointers
			StoreDiffResult(subdir, &leftFiles[i], 0, nDiffCode, pCtxt);
			++i;
			continue;
		}
		if (j<rightFiles.GetSize() && (i==leftFiles.GetSize() || collstr(leftFiles[i].name, rightFiles[j].name, casesensitive)>0))
		{
			int nDiffCode = DIFFCODE::RIGHT + DIFFCODE::FILE;

			// Test against filter
			CString newsubfile = subprefix + rightFiles[j].name;
			if (!pCtxt->m_piFilterUI->includeFile(newsubfile) || !pCtxt->m_piFilterGlobal->includeFile(newsubfile))
				nDiffCode |= DIFFCODE::SKIPPED;

			// Advance right pointer over right-only entry, and then retest with new pointers
			StoreDiffResult(subdir, 0, &rightFiles[j], nDiffCode, pCtxt);
			++j;
			continue;
		}
		if (i<leftFiles.GetSize())
		{
			ASSERT(j<rightFiles.GetSize());
			CString newsubfile = subprefix + leftFiles[i].name;
			if (!pCtxt->m_piFilterUI->includeFile(newsubfile) || !pCtxt->m_piFilterGlobal->includeFile(newsubfile))
			{
				StoreDiffResult(subdir, &leftFiles[i], &rightFiles[j], DIFFCODE::SKIPPED+DIFFCODE::FILE, pCtxt);
			}
			else
			{

				int code = prepAndCompareTwoFiles(leftFiles[i], rightFiles[j], 
					sLeftDir, sRightDir);
				
				// report result back to caller
				StoreDiffResult(subdir, &leftFiles[i], &rightFiles[j], code, pCtxt);
			}
			++i;
			++j;
			continue;
		}
		break;
	}
	return 1;
}

/**
 * @brief Prepare files (run plugins) & compare them, and return diffcode
 */
static int
prepAndCompareTwoFiles(const fentry & lent, const fentry & rent, 
	const CString & sLeftDir, const CString & sRightDir
	)
{
	// If options are binary equivalent, we could check for filesize
	// difference here, and bail out if files are clearly different
	// But, then we don't know if file is ascii or binary, and this
	// affects behavior (also, we don't have an icon for unknown type)

	// Similarly if user desired to make some comparison shortcut
	// based on file date, it could be done here, with the same caveat
	// as above

	gLog.Write(_T("Comparing: n0=%s, n1=%s, d0=%s, d1=%s")
		, lent.name, rent.name, sLeftDir, sRightDir);
	CString filepath1 = paths_ConcatPath(sLeftDir, lent.name);
	CString filepath2 = paths_ConcatPath(sRightDir, rent.name);

	// compareok equals true as long as everything is fine
	BOOL compareok = TRUE;

	// For user chosen plugins, define bAutomaticUnpacker as false and use the chosen infoHandler
	// but how can we receive the infoHandler ? DirScan actually only 
	// returns info, but can not use file dependent information.

	// Transformation happens here
	// text used for automatic mode : plugin filter must match it
	CString filteredFilenames = filepath1 + "|" + filepath2;
	// Use temporary plugins info
	PackingInfo infoUnpacker(m_bUnpackerMode);
	PackingInfo infoPrediffer(m_bPredifferMode);
	// plugin may alter filepaths to temp copies
	CString filepathTransformed1 = filepath1;
	CString filepathTransformed2 = filepath2;

	// first step : unpack (plugins)
	if (infoUnpacker.bToBeScanned)
		compareok = FileTransform_Unpacking(filepathTransformed1, filteredFilenames, &infoUnpacker, &infoUnpacker.subcode);
	else
		compareok = FileTransform_Unpacking(filepathTransformed1, infoUnpacker, &infoUnpacker.subcode);
	// second step : normalize Unicode to OLECHAR (most of time, do nothing) (OLECHAR = UCS-2LE in Windows)
	BOOL bMayOverwrite1 = (filepathTransformed1 != filepath1);
	if (compareok)
		compareok = FileTransform_NormalizeUnicode(filepathTransformed1, bMayOverwrite1);
	// third step : prediff (plugins)
	bMayOverwrite1 = (filepathTransformed1 != filepath1);
	if (compareok)
		compareok = FileTransform_Prediffing(filepathTransformed1, filteredFilenames, &infoPrediffer, bMayOverwrite1);
	// fourth step : prepare for diffing
	bMayOverwrite1 = (filepathTransformed1 != filepath1);
	if (compareok)
		compareok = FileTransform_UCS2ToUTF8(filepathTransformed1, bMayOverwrite1);

	// we use the same plugins for both files, so they must be defined before second file
	ASSERT(infoUnpacker.bToBeScanned == FALSE);
	ASSERT(infoPrediffer.bToBeScanned == FALSE);

	// first step : unpack (plugins)
	if (compareok)
		compareok = FileTransform_Unpacking(filepathTransformed2, infoUnpacker, &infoUnpacker.subcode);
	// second step : normalize Unicode to OLECHAR (most of time, do nothing)
	BOOL bMayOverwrite2 = (filepathTransformed2 != filepath2);
	if (compareok)
		compareok = FileTransform_NormalizeUnicode(filepathTransformed2, bMayOverwrite2);
	// third step : prediff (plugins)
	bMayOverwrite2 = (filepathTransformed2 != filepath2);
	if (compareok)
		compareok = FileTransform_Prediffing(filepathTransformed2, infoPrediffer, bMayOverwrite2);
	// fourth step : prepare for diffing
	bMayOverwrite1 = (filepathTransformed1 != filepath1);
	if (compareok)
		compareok = FileTransform_UCS2ToUTF8(filepathTransformed1, bMayOverwrite1);

	// Actually compare the files
	// just_compare_files is a fairly thin front-end to diffutils
	bool diff=false, bin=false;
	if (compareok)
		compareok = just_compare_files (filepathTransformed1, filepathTransformed2, 0, &diff, &bin);

	// delete the temp files after comparison
	if (filepathTransformed1 != filepath1)
		::DeleteFile(filepathTransformed1);
	if (filepathTransformed2 != filepath2)
		::DeleteFile(filepathTransformed2);

	// assemble bit flags for result code
	int code = DIFFCODE::FILE;
	if (!compareok)
	{
		code |= DIFFCODE::CMPERR;
	}
	else
	{
		code |= (diff ? DIFFCODE::DIFF : DIFFCODE::SAME);
		code |= (bin ? DIFFCODE::BIN : DIFFCODE::TEXT);
	}
	return code;
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
	if (!sPattern.IsEmpty() && sPattern[sPattern.GetLength()-1]!='\\')
		sPattern += '\\';
	sPattern += _T("\\*.*");
	CFileFind finder;
	BOOL bWorking = finder.FindFile(sPattern);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		fentry ent;
		CTime mtim;
		finder.GetLastWriteTime(mtim);
		CTime ctim;
		finder.GetCreationTime(ctim);
		ent.mtime = mtim.GetTime();
		ent.ctime = ctim.GetTime();
#if _MSC_VER < 1300
		// MSVC6
		ent.size = finder.GetLength64(); // __int64
#else
		// MSVC7 (VC.NET)
		ent.size = finder.GetLength(); // ULONGLONG
#endif
		ent.name = finder.GetFileName();
		ent.attrs = 0;
		if (finder.IsDirectory())
			dirs->Add(ent);
		else
			files->Add(ent);
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
static void StoreDiffResult(const CString & sDir, const fentry * lent, const fentry * rent, int code, CDiffContext * pCtxt)
{
	CString name, leftdir, rightdir;
	_int64 rmtime=0, lmtime=0, rctime=0, lctime=0;
	_int64 lsize=0, rsize=0;
	int lattrs=0, rattrs=0;

	// We must store both paths - we cannot get paths later
	// and we need unique item paths for example when items
	// change to identical
	leftdir = paths_ConcatPath(pCtxt->m_strNormalizedLeft, sDir);
	rightdir = paths_ConcatPath(pCtxt->m_strNormalizedRight, sDir);

	if (lent)
	{
		lmtime = lent->mtime;
		lctime = lent->ctime;
		lsize = lent->size;
		name = lent->name;
		lattrs = lent->attrs;
	}
	if (rent)
	{
		rmtime = rent->mtime;
		rctime = rent->ctime;
		rsize = rent->size;
		name = rent->name;
		rattrs = rent->attrs;
	}
	gLog.Write(_T("name=<%s>, leftdir=<%s>, rightdir=<%s>, code=%d")
		, (LPCTSTR)name, (LPCTSTR)leftdir, (LPCTSTR)rightdir, code);
	pCtxt->AddDiff(name, sDir, leftdir, rightdir
		, lmtime, rmtime, lctime, rctime, lsize, rsize, code, lattrs, rattrs);
}
