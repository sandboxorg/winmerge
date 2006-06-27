/////////////////////////////////////////////////////////////////////////////
//    License (GPLv2+):
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or (at
//    your option) any later version.
//    
//    This program is distributed in the hope that it will be useful, but
//    WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/////////////////////////////////////////////////////////////////////////////
/** 
 * @file  DiffFileInfo.cpp
 *
 * @brief Implementation for DiffFileInfo routines
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#include "stdafx.h"
#include "FileInfo.h"
#include "DiffFileInfo.h"
#include "unicoder.h"

/**
 * @brief Convert a FILETIME to a long (standard time)
 */
static __int64 FileTimeToInt64(FILETIME & ft)
{
	return CTime(ft).GetTime();
}

/**
 * @brief Update fileinfo from given file
 * @param [in] sFilePath Full path to file/directory to update
 * @return TRUE if file exists
 */
BOOL DiffFileInfo::Update(LPCTSTR sFilePath)
{
	// CFileFind doesn't expose the attributes
	// CFileStatus doesn't expose 64 bit size
	BOOL Update = FALSE;
	WIN32_FIND_DATA wfd;
	HANDLE h = FindFirstFile(sFilePath, &wfd);
	__int64 mtime64 = 0;
	size = -1;
	flags.reset();
	mtime = 0;
	if (h != INVALID_HANDLE_VALUE)
	{
		mtime64 = FileTimeToInt64(wfd.ftLastWriteTime);
		flags.attributes = wfd.dwFileAttributes;

		// Folders don't have a size (size remains as -1)
		if ((flags.attributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			size = FileInfo::GetSizeFromFindData(wfd);
		FindClose(h);
		Update = TRUE;
	}
	mtime = mtime64;
	return Update;
}

/**
 * @brief Clears FileInfo data.
 */
void DiffFileInfo::Clear()
{
	FileInfo::Clear();
	bVersionChecked = false;
	encoding.Clear();
	m_textStats.clear();
}


/**
 * @brief Return true if file is in any Unicode encoding
 */
bool DiffFileInfo::IsEditableEncoding() const
{
	return encoding.m_bom == false;
}
