/** 
 *  @file   UniFile.cpp
 *  @author Perry Rapp, Creator, 2003
 *  @date   Created: 2003-10
 *  @date   Edited:  2003-11-14 (Perry)
 *
 *  @brief Implementation of Memory-Mapped Unicode enabled file class
 */

/* The MIT License
Copyright (c) 2003 Perry Rapp
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdafx.h"
#include "UniFile.h"
#include "unicoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UniMemFile::UniMemFile(LPCTSTR filename)
: m_statusFetched(0)
, m_filesize(0)
, m_handle(INVALID_HANDLE_VALUE)
, m_hMapping(INVALID_HANDLE_VALUE)
, m_base(NULL)
, m_data(NULL)
, m_current(NULL)
, m_lineno(-1)
, m_readbom(false)
, m_unicoding(ucr::NONE)
, m_charsize(0)
, m_codepage(0)
{
	m_filename = filename;
	m_filepath = m_filename;
}

void UniMemFile::Close()
{
	m_lastError.ClearError();

	if (m_base)
	{
		UnmapViewOfFile(m_base);
		m_base = 0;
	}
	m_data = NULL;
	m_current = NULL;
	m_lineno = -1;
	if (m_hMapping != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hMapping);
		m_hMapping = INVALID_HANDLE_VALUE;
	}
	if (m_handle != INVALID_HANDLE_VALUE)
	{
		FlushFileBuffers(m_handle);
		CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
	m_statusFetched = 0;
	m_readbom = false;
}

/** @brief Get file status into member variables */
bool UniMemFile::GetFileStatus()
{
	m_lastError.ClearError();
	m_statusFetched = -1;

	if (!CFile::GetStatus(m_filepath, m_filestatus))
	{
		LastError(_T("CFile::GetStatus"), 0);
		return false;
	}
	m_filepath = m_filestatus.m_szFullName;

	if (m_handle == INVALID_HANDLE_VALUE)
	{
		LastErrorCustom(_T("UniMemFile::GetFileStatus called without open file handle"));
		return false;
	}
	DWORD sizehi=0;
	DWORD sizelo = GetFileSize(m_handle, &sizehi);
	int errnum = GetLastError();
	if (errnum != NO_ERROR)
	{
		LastError(_T("GetFileSize"), errnum);
		return false;
	}
	m_filesize = sizelo + (sizehi << 32);
	m_statusFetched = 1;
	return true;
}

/** @brief Open file for generic read-only access */
bool UniMemFile::OpenReadOnly()
{
	DWORD dwOpenAccess = GENERIC_READ;
	DWORD dwOpenShareMode = FILE_SHARE_READ;
	DWORD dwOpenCreationDispostion = OPEN_EXISTING;
	DWORD dwMappingProtect = PAGE_READONLY;
	DWORD dwMapViewAccess = FILE_MAP_READ;
	return Open(dwOpenAccess, dwOpenShareMode, dwOpenCreationDispostion, dwMappingProtect, dwMapViewAccess);
}

/** @brief Open file for generic read-write access */
bool UniMemFile::Open()
{
	DWORD dwOpenAccess = GENERIC_WRITE;
	DWORD dwOpenShareMode = 0;
	DWORD dwOpenCreationDispostion = OPEN_EXISTING;
	DWORD dwMappingProtect = PAGE_READWRITE;
	DWORD dwMapViewAccess = FILE_MAP_WRITE;
	return Open(dwOpenAccess, dwOpenShareMode, dwOpenCreationDispostion, dwMappingProtect, dwMapViewAccess);
}

/** @brief Open file with specified arguments */
bool UniMemFile::Open(DWORD dwOpenAccess, DWORD dwOpenShareMode, DWORD dwOpenCreationDispostion, DWORD dwMappingProtect, DWORD dwMapViewAccess)
{
	// We use an internal workhorse to make it easy to close on any error
	if (!DoOpen(dwOpenAccess, dwOpenShareMode, dwOpenCreationDispostion, dwMappingProtect, dwMapViewAccess))
	{
		Close();
		return false;
	}
	return true;
}

/** @brief Internal implementation of Open */
bool UniMemFile::DoOpen(DWORD dwOpenAccess, DWORD dwOpenShareMode, DWORD dwOpenCreationDispostion, DWORD dwMappingProtect, DWORD dwMapViewAccess)
{
	if (m_lastError.hasError()) return false;

	Close();

	m_handle = CreateFile(m_filename, dwOpenAccess, dwOpenShareMode, NULL, dwOpenCreationDispostion, 0, 0);
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		LastError(_T("CreateFile"), GetLastError());
		return false;
	}
	if (!GetFileStatus())
		return false;

	DWORD sizehi = (DWORD)(m_filesize >> 32);
	DWORD sizelo = (DWORD)(m_filesize & 0xFFFFFFFF);

	if (sizehi)
	{
		LastErrorCustom(_T("UniMemFile cannot handle files over 4 gigabytes"));
		return false;
	}

	if (sizelo == 0)
	{
		// Allow opening empty file, but memory mapping doesn't work on such
		// m_base and m_current are 0 from the Close call above
		// so ReadString will correctly return empty EOF immediately
		m_lineno = 0;
		return true;
	}

	LPSECURITY_ATTRIBUTES lpAttributes = NULL; // default security
	LPCTSTR lpName = NULL; // nameless mapping
	m_hMapping = CreateFileMapping(m_handle, lpAttributes, dwMappingProtect, sizehi, sizelo, lpName);
	if (!m_hMapping)
	{
		LastError(_T("CreateFileMapping"), GetLastError());
		return false;
	}


	DWORD dwFileOffsetHigh = 0;
	DWORD dwFileOffsetLow = 0;
	SIZE_T dwNumberOfBytesToMap = sizelo;
	m_base = (LPBYTE)MapViewOfFile(m_hMapping, dwMapViewAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
	if (!m_base)
	{
		LastError(_T("MapViewOfFile"), GetLastError());
		return false;
	}
	m_data = m_base;
	m_current = m_base;
	m_lineno = 0;

	return true;
}

/** @brief Record an API call failure */
void UniMemFile::LastError(LPCTSTR apiname, int syserrnum)
{
	m_lastError.ClearError();

	m_lastError.apiname = apiname;
	m_lastError.syserrnum = syserrnum;
}

/** @brief Record a custom error */
void UniMemFile::LastErrorCustom(LPCTSTR desc)
{
	m_lastError.ClearError();

	m_lastError.desc = desc;
}

/**
 * @brief Check for Unicode BOM (byte order mark) at start of file
 *
 * @note This code only checks for UCS-2LE, UCS-2BE, and UTF-8 BOMs (no UCS-4).
 */
bool UniMemFile::ReadBom()
{
	byte * lpByte = (byte *)m_base;
	m_current = m_data = m_base;
	m_charsize = 1;
	if (m_filesize >= 2)
	{
		if (lpByte[0] == 0xFF && lpByte[1] == 0xFE)
		{
			m_unicoding = ucr::UCS2LE;
			m_charsize = 2;
			m_data = lpByte+2;
		}
		else if (lpByte[0] == 0xFE && lpByte[1] == 0xFF)
		{
			m_unicoding = ucr::UCS2BE;
			m_charsize = 2;
			m_data = lpByte+2;
		}
	}
	if (m_filesize >=3)
	{
		if (lpByte[0] == 0xEF && lpByte[1] == 0xBB && lpByte[2] == 0xBF)
		{
			m_unicoding = ucr::UTF8;
			m_data = lpByte+3;
		}
	}
	m_readbom = true;
	m_current = m_data;
	return (m_data != m_base);
}

/**
 * @brief Read one (DOS or UNIX or Mac) line. Do not include eol chars.
 */
BOOL UniMemFile::ReadString(CString & line)
{
	CString eol;
	BOOL ok = ReadString(line, eol);
	return ok;
}

/**
 * @brief Read one (DOS or UNIX or Mac) line
 */
BOOL UniMemFile::ReadString(CString & line, CString & eol)
{
	line = _T("");
	eol = _T("");
	// shortcut methods in case file is in the same encoding as our CStrings
#ifdef _UNICODE
	if (m_unicoding == ucr::UCS2LE)
	{
		// If there aren't any wchars left in the file, return FALSE to indicate EOF
		if (m_current - m_base + 1 >= m_filesize)
			return FALSE;
		// Loop through wchars, watching for eol chars or zero
		while (m_current - m_base + 1 < m_filesize)
		{
			wchar_t wch = *(wchar_t *)m_current;
			m_current += 2;
			if (wch == '\n' || wch == '\r')
			{
				eol += wch;
				if (wch == '\r')
				{
					if (m_current - m_base + 1 < m_filesize && *(wchar_t *)m_current == '\n')
					{
						eol += '\n';
						m_current += 2;
						++m_txtstats.ncrlfs;
					}
					else
					{
						++m_txtstats.ncrs;
					}
				}
				else
				{
					++m_txtstats.nlfs;
				}
				++m_lineno;
				return TRUE;
			}
			if (!wch)
			{
				++m_txtstats.nzeros;
				return TRUE;
			}
			line += wch;
		}
		return TRUE;
	}
#else
	if (m_unicoding == ucr::NONE && !m_codepage)
	{
		// If there aren't any bytes left in the file, return FALSE to indicate EOF
		if (m_current - m_base >= m_filesize)
			return FALSE;
		// Loop through chars, watching for eol chars or zero
		while (m_current - m_base < m_filesize)
		{
			char ch = *m_current;
			++m_current;
			if (ch == '\n' || ch == '\r')
			{
				eol += ch;
				if (ch == '\r')
				{
					if (m_current - m_base < m_filesize && *m_current == '\n')
					{
						eol += '\n';
						++m_current;
						++m_txtstats.ncrlfs;
					}
					else
					{
						++m_txtstats.ncrs;
					}
				}
				else
				{
					++m_txtstats.nlfs;
				}
				++m_lineno;
				return TRUE;
			}
			if (!ch)
			{
				++m_txtstats.nzeros;
				return TRUE;
			}
			line += ch;
		}
		return TRUE;
	}
#endif

	if (m_current - m_base + (m_charsize-1) >= m_filesize)
		return FALSE;

	// Handle 8-bit strings in line chunks because of multibyte codings (eg, 936)
	if (m_unicoding == ucr::NONE && m_codepage)
	{
		bool eof=true;
		for (LPBYTE eolptr = m_current; (eolptr - m_base + (m_charsize-1) < m_filesize); ++eolptr)
		{
			if (*eolptr == '\n' || *eolptr == '\r')
			{
				eof=false;
				break;
			}
		}
		bool lossy=false;
		line = ucr::maketstring((LPCSTR)m_current, eolptr-m_current, m_codepage, &lossy);
		if (!eof)
		{
			eol += (TCHAR)*eolptr;
			if (*eolptr == '\r' && (eolptr - m_base + (m_charsize-1) < m_filesize) && eolptr[1] == '\n')
				eol += '\n';
			++m_lineno;
		}
		m_current = eolptr + eol.GetLength();
		// TODO: What do we do if save was lossy ?
		return !eof;
	}

	while (m_current - m_base + (m_charsize-1) < m_filesize)
	{
		UINT ch=0;
		UINT utf8len=0;
		bool doneline=false;

		if (m_unicoding == ucr::UTF8)
		{
			// check for end in middle of UTF-8 character
			utf8len = ucr::Utf8len_fromLeadByte(*m_current);
			if (m_current - m_base + utf8len > m_filesize)
			{
				ch = '?';
				m_current = m_base + m_filesize;
				doneline = true;
			}
			// Handle bad UTF-8 or UTF-8 outside of UCS-2
			// (Convert bad bytes individually to '?'
			else if (utf8len < 1 || utf8len > 4)
			{
				ch = '?';
				utf8len=1;
			}
			else
			{
				ch = ucr::GetUtf8Char(m_current);
			}
		}
		else
		{
			ch = ucr::get_unicode_char(m_current, (ucr::UNICODESET)m_unicoding, m_codepage);
			if (!ch)
				doneline = true;
		}
		// convert from Unicode codepoint to TCHAR string
		// could be multicharacter if decomposition took place, for example
		bool lossy = false; // try to avoid lossy conversion
		CString sch = ucr::maketchar(ch, lossy);
		if (lossy)
			++m_txtstats.nlosses;
		if (sch.GetLength() >= 1)
			ch = sch[0];
		else
			ch = 0;


		if (ch == '\r')
		{
			eol = _T("\r");
			doneline = true;
			bool crlf = false;
			// check for crlf pair
			if (m_current - m_base + 2 * m_charsize - 1 < m_filesize)
			{
				// For UTF-8, this ch will be wrong if character is non-ASCII
				// but we only check it against \n here, so it doesn't matter
				UINT ch = ucr::get_unicode_char(m_current+m_charsize, (ucr::UNICODESET)m_unicoding);
				if (ch == '\n')
				{
					crlf = true;
				}
			}
			if (crlf)
			{
				eol = _T("\r\n");
				++m_txtstats.ncrlfs;
				// advance an extra character to skip the following lf
				m_current += m_charsize;
			}
			else
			{
				++m_txtstats.ncrs;
			}
		}
		else if (ch == '\n')
		{
			eol = _T("\n");
			doneline = true;
			++m_txtstats.nlfs;
		}
		else if (!ch)
		{
			doneline = true;
			++m_txtstats.nzeros;
		}
		// always advance to next character
		if (m_unicoding == ucr::UTF8)
		{
			m_current += utf8len;
		}
		else
		{
			m_current += m_charsize;
		}
		if (doneline)
		{
			if (!eol.IsEmpty())
				++m_lineno;
			return TRUE;
		}
		line += sch;
	}
	return TRUE;
}

