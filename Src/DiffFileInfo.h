/////////////////////////////////////////////////////////////////////////////
//    License (GPLv2+):
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but
//    WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/////////////////////////////////////////////////////////////////////////////
/** 
 * @file  DiffFileInfo.h
 *
 * @brief Declaration file for DiffFileInfo
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#ifndef _DIFF_FILE_INFO_H_INCLUDED
#define _DIFF_FILE_INFO_H_INCLUDED

struct FileFlags
{
	int flags;
	FileFlags() : flags(0) { }
	void reset() { flags &= 0xFFFFFFFE; }
	CString toString() const
		{
			CString sflags;
			if (flags & RO)
				sflags += _T("R");
			if ((flags & coding) == UTF_8)
				sflags += _T("8");
			if ((flags & coding) == UCS_2BE)
				sflags += _T("B");
			if ((flags & coding) == UCS_2LE)
				sflags += _T("L");
			if ((flags & coding) == UCS_4)
				sflags += _T("4");
			return sflags;
		}

	enum { RO=1, coding=0x7000, UTF_8=0x1000, UCS_4=0x2000, UCS_2BE=0x3000, UCS_2LE=0x4000 };
};

/**
 * @brief Information for file
 */
struct DiffFileInfo
{
	// storing __time_t if MSVC6 (__MSC_VER<1300)
	// storing __time64_t if MSVC7 (VC.NET)
	__int64 ctime; /**< time of creation */
	__int64 mtime;
	__int64 size; /**< file size in bytes, -1 means file does not exist*/
	CString version; /**< string of fixed file version, eg, 1.2.3.4 */
	FileFlags flags; /**< file attributes */
	int codepage; /**< 8bit codepage, if applicable, 0 is unknown or N/A */
	int unicoding; /**< Unicode encoding (ucr::CODESET) */
	DiffFileInfo() : ctime(0), mtime(0), size(-1), codepage(0), unicoding(0) { }

	CString getEncodingString() const;
	// We could stash a pointer here to the parent DIFFITEM
	// but, I ran into trouble with, I think, the DIFFITEM copy constructor
	
	void Update(CString sFilePath);
};

#endif // _DIFF_FILE_INFO_H_INCLUDED
