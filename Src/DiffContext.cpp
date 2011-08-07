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
/**
 *  @file DiffContext.cpp
 *
 *  @brief Implementation of CDiffContext
 */ 
// ID line follows -- this is updated by SVN
// $Id: DiffContext.cpp 7063 2009-12-27 15:28:16Z kimmov $
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <shlwapi.h>
#include "Merge.h"
#include "CompareOptions.h"
#include "CompareStats.h"
#include "version.h"
#include "FilterList.h"
#include "DiffContext.h"
#include "paths.h"
#include "coretools.h"
#include "codepage_detect.h"
#include "DiffItemList.h"
#include "IAbortable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * @brief Construct CDiffContext.
 *
 * @param [in] pszLeft Initial left-side path.
 * @param [in] pszRight Initial right-side path.
 * @param [in] compareMethod Main compare method for this compare.
 */
CDiffContext::CDiffContext(const PathContext & paths, int compareMethod)
: m_piFilterGlobal(NULL)
, m_piPluginInfos(NULL)
, m_nCompMethod(compareMethod)
, m_nCurrentCompMethod(compareMethod)
, m_bIgnoreSmallTimeDiff(FALSE)
, m_pCompareStats(NULL)
, m_piAbortable(NULL)
, m_bStopAfterFirstDiff(false)
, m_bScanUnpairedDir(FALSE)
, m_pFilterList(NULL)
, m_pDiffWrapper(NULL)
, m_pCompareOptions(NULL)
, m_pOptions(NULL)
, m_bPluginsEnabled(false)
, m_bRecursive(false)
, m_bWalkUniques(true)
{
	int index;
	for (index = 0; index < paths.GetSize(); index++)
		m_paths.SetPath(index, paths[index].c_str());
}

/**
 * @brief Destructor.
 */
CDiffContext::~CDiffContext()
{
}

/**
 * @brief Update info in item in result list from disk.
 * This function updates result list item's file information from actual
 * file in the disk. This updates info like date, size and attributes.
 * @param [in] diffpos DIFFITEM to update.
 * @param [in] bLeft Update left-side info.
 * @param [in] bRight Update right-side info.
 */
void CDiffContext::UpdateStatusFromDisk(UINT_PTR diffpos, BOOL bLeft, BOOL bRight)
{
	DIFFITEM &di = GetDiffRefAt(diffpos);
	if (bLeft)
	{
		di.diffFileInfo[0].ClearPartial();
		if (!di.diffcode.isSideSecondOnly())
			UpdateInfoFromDiskHalf(di, 0);
	}
	if (bRight)
	{
		di.diffFileInfo[1].ClearPartial();
		if (!di.diffcode.isSideFirstOnly())
			UpdateInfoFromDiskHalf(di, 1);
	}
}

/**
 * @brief Update file information from disk for DIFFITEM.
 * This function updates DIFFITEM's file information from actual file in
 * the disk. This updates info like date, size and attributes.
 * @param [in, out] di DIFFITEM to update (selected side, see bLeft param).
 * @param [in] bLeft If TRUE left side information is updated,
 *  right side otherwise.
 * @return TRUE if file exists
 */
BOOL CDiffContext::UpdateInfoFromDiskHalf(DIFFITEM & di, int nIndex)
{
	String filepath = paths_ConcatPath(di.getFilepath(nIndex, GetNormalizedPath(nIndex)), di.diffFileInfo[nIndex].filename);
	DiffFileInfo & dfi = di.diffFileInfo[nIndex];
	if (!dfi.Update(filepath.c_str()))
		return FALSE;
	UpdateVersion(di, nIndex);
	GuessCodepageEncoding(filepath.c_str(), &dfi.encoding, m_iGuessEncodingType);
	return TRUE;
}

/**
 * @brief Determine if file is one to have a version information.
 * This function determines if the given file has a version information
 * attached into it in resource. This is done by comparing file extension to
 * list of known filename extensions usually to have a version information.
 * @param [in] ext Extension to check.
 * @return true if extension has version info, false otherwise.
 */
static bool CheckFileForVersion(LPCTSTR ext)
{
	if (!lstrcmpi(ext, _T(".EXE")) || !lstrcmpi(ext, _T(".DLL")) || !lstrcmpi(ext, _T(".SYS")) ||
	    !lstrcmpi(ext, _T(".DRV")) || !lstrcmpi(ext, _T(".OCX")) || !lstrcmpi(ext, _T(".CPL")) ||
	    !lstrcmpi(ext, _T(".SCR")) || !lstrcmpi(ext, _T(".LANG")))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Load file version from disk.
 * Update fileversion for given item and side from disk. Note that versions
 * are read from only some filetypes. See CheckFileForVersion() function
 * for list of files to check versions.
 * @param [in,out] di DIFFITEM to update.
 * @param [in] bLeft If TRUE left-side file is updated, right-side otherwise.
 */
void CDiffContext::UpdateVersion(DIFFITEM & di, int nIndex) const
{
	DiffFileInfo & dfi = nIndex == 0 ? di.diffFileInfo[0] : di.diffFileInfo[1];
	// Check only binary files
	dfi.version.Clear();
	dfi.bVersionChecked = true;

	if (di.diffcode.isDirectory())
		return;
	
	String spath;
	if (nIndex == 0)
	{
		if (di.diffcode.isSideSecondOnly())
			return;
		LPCTSTR ext = PathFindExtension(di.diffFileInfo[0].filename.c_str());
		if (!CheckFileForVersion(ext))
			return;
		spath = di.getFilepath(0, GetNormalizedLeft());
		spath = paths_ConcatPath(spath, di.diffFileInfo[0].filename);
	}
	else
	{
		if (di.diffcode.isSideFirstOnly())
			return;
		LPCTSTR ext = PathFindExtension(di.diffFileInfo[1].filename.c_str());
		if (!CheckFileForVersion(ext))
			return;
		spath = di.getFilepath(1, GetNormalizedRight());
		spath = paths_ConcatPath(spath, di.diffFileInfo[1].filename);
	}
	
	// Get version info if it exists
	CVersionInfo ver(spath.c_str());
	DWORD verMS = 0;
	DWORD verLS = 0;
	if (ver.GetFixedFileVersion(verMS, verLS))
		dfi.version.SetFileVersion(verMS, verLS);
}

/**
 * @brief Create compare-method specific compare options class.
 * This function creates a compare options class that is specific for
 * main compare method. Compare options class is initialized from
 * given set of options.
 * @param [in] compareMethod Selected compare method.
 * @param [in] options Initial set of compare options.
 * @return TRUE if creation succeeds.
 */
BOOL CDiffContext::CreateCompareOptions(int compareMethod, const DIFFOPTIONS & options)
{
	m_pOptions.reset(new DIFFOPTIONS);
	if (m_pOptions != NULL)
		CopyMemory(m_pOptions.get(), &options, sizeof(DIFFOPTIONS));
	else
		return FALSE;

	m_nCompMethod = compareMethod;
	m_pCompareOptions.reset(GetCompareOptions(m_nCompMethod));
	if (m_pCompareOptions == NULL)
	{
		// For Date and Date+Size compare NULL is ok since they don't have actual
		// compare options.
		if (m_nCompMethod == CMP_DATE || m_nCompMethod == CMP_DATE_SIZE ||
			m_nCompMethod == CMP_SIZE)
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	return TRUE;
}

/**
 * @brief Get compare-type specific compare options.
 * This function returns per-compare method options. The compare options
 * returned are converted from general options to match options for specific
 * comapare type. Not all compare options in general set are available for
 * some other compare type. And some options can have different values.
 * @param [in] compareMethod Compare method used.
 * @return Compare options class.
 */
CompareOptions * CDiffContext::GetCompareOptions(int compareMethod)
{
	//If compare method is same than in previous time, return cached value
	if (compareMethod == m_nCurrentCompMethod && m_pCompareOptions != NULL)
		return m_pCompareOptions.get();

	// Otherwise we have to create new options
	switch (compareMethod)
	{
	case CMP_CONTENT:
		m_pCompareOptions.reset(new DiffutilsOptions());
		break;

	case CMP_QUICK_CONTENT:
		m_pCompareOptions.reset(new QuickCompareOptions());
		break;

	default:
		// No really options to set..
		m_pCompareOptions.reset();
		break;
	}

	m_nCurrentCompMethod = compareMethod;

	if (m_pCompareOptions == NULL)
		return NULL;

	m_pCompareOptions->SetFromDiffOptions(*m_pOptions);

	return m_pCompareOptions.get();
}

/** @brief Forward call to retrieve plugin info (winds up in DirDoc) */
void CDiffContext::FetchPluginInfos(LPCTSTR filteredFilenames,
		PackingInfo ** infoUnpacker, PrediffingInfo ** infoPrediffer)
{
	ASSERT(m_piPluginInfos);
	m_piPluginInfos->FetchPluginInfos(filteredFilenames, infoUnpacker, infoPrediffer);
}

/**
 * @brief Check if user has requested aborting the compare.
 * @return true if user has requested abort, false otherwise.
 */
bool CDiffContext::ShouldAbort() const
{
	return m_piAbortable && m_piAbortable->ShouldAbort();
}
