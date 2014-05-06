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
 * @file  ConfigLog.h
 *
 * @brief Declaration file ConfigLog class
 */
// ID line follows -- this is updated by SVN
// $Id: ConfigLog.h 7059 2009-12-27 00:06:59Z kimmov $

#ifndef _CONFIGLOG_H_
#define _CONFIGLOG_H_

#include "UnicodeString.h"
#include <boost/scoped_ptr.hpp>

class UniStdioFile;

/** 
 * @brief Class for saving configuration log file
 */
class CConfigLog
{
public:
	CConfigLog();
	~CConfigLog();

	String GetFileName() const;
	bool WriteLogFile(String &sError);

	// Implementation methods
protected:
	void WriteItem(int indent, const String& key, const TCHAR *value = 0);
	void WriteItem(int indent, const String& key, const String &str);
	void WriteItem(int indent, const String& key, long value);
	void WriteVersionOf1(int indent, const String& path);
	void WriteVersionOf(int indent, const String& path);
	void WriteLocaleSettings(unsigned locid, const String& title);
	void WriteArchiveSupport();
	void WriteVersionOf7z(const String& path);
	void WriteWinMergeConfig(void);

private:
	bool DoFile(String &sError);
	void WritePluginsInLogFile(const wchar_t *transformationEvent);
	String GetWindowsVer();
	String GetBuildFlags() const;
	void FileWriteString(const String& lpsz);
	void CloseFile();

	// Implementation data
private:
	String m_sFileName;
	boost::scoped_ptr<UniStdioFile> m_pfile;
};

#endif /* _CONFIGLOG_H_ */