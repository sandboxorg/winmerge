/** 
 * @file  LogFile.h
 *
 * @brief Implementation file for CLogFile
 *
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#include "stdafx.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const TCHAR MutexName[] = _T("WINMERGE_LOG_MUTEX");

/**
 * @brief Constructor
 */
CLogFile::CLogFile(LPCTSTR szLogName /*= NULL*/,
	 LPCTSTR szLogPath /*= NULL*/, BOOL bDeleteExisting /*=FALSE*/)
	: m_nMaxSize(1024 * 1024) // 1MB
	, m_bEnabled(FALSE)
	, m_nDefaultLevel(LOGLEVEL::LMSG)
	, m_nMaskLevel(LOGLEVEL::LALL)
{
	SetFile(szLogName, szLogPath, bDeleteExisting);
	m_hLogMutex = CreateMutex(NULL, FALSE, MutexName);
}

/**
 * @brief Destructor
 */
CLogFile::~CLogFile()
{
	EnableLogging(FALSE);
	CloseHandle(m_hLogMutex);
}

/**
 * @brief Set logfilename
 * @param bDelExisting If TRUE, existing logfile with same name
 * is deleted.
 */
CString CLogFile::SetFile(CString strFile, CString strPath,
	BOOL bDelExisting /*= FALSE*/)
{
	TCHAR temp[MAX_PATH] = {0};

	// build the path to the logfile
	if (!strPath.IsEmpty())
		m_strLogPath = strFile;
	else
	{
		if (GetTempPath(MAX_PATH, temp))
			m_strLogPath = temp;
	}
	
	if (m_strLogPath.Right(1) != _T('\\'))
		m_strLogPath += _T('\\');

	m_strLogPath += strFile;

	if (bDelExisting)
		DeleteFile(m_strLogPath);
	
	return m_strLogPath;
}

/**
 * @brief Enable/Disable writing log
 */
void CLogFile::EnableLogging(BOOL bEnable)
{
	CTime t = CTime::GetCurrentTime();
	CString s = t.Format(_T(" %A, %B %d, %Y    %H:%M:%S"));

	if (bEnable)
	{
		m_bEnabled = TRUE;
		Write(_T("\n*******\nLog Started: %s"), s);
		Write(_T("Path: %s\n*******\n"), m_strLogPath);
	}
	else
	{
		m_bEnabled = FALSE;
		Write(_T("\n*******\nLog Stopped: %s\n"), s);
		Write(_T("*******\n"));
	}
}

/**
 * @brief Return default level for log messages
 */
UINT CLogFile::GetDefaultLevel() const
{
	return m_nDefaultLevel;
}

/**
 * @brief Set default level for log messages
 */
void CLogFile::SetDefaultLevel(UINT logLevel)
{
	m_nDefaultLevel = logLevel;
}

/**
 * @brief Get log message mask.
 *
 * Mask allows to select which level messages are written to log.
 */
UINT CLogFile::GetMaskLevel() const
{
	return m_nMaskLevel;
}

/**
 * @brief Set log message mask
 */
void CLogFile::SetMaskLevel(UINT maskLevel)
{
	m_nMaskLevel = maskLevel;
}

/**
 * @brief Write formatted message with default log level
 */
void CLogFile::Write(LPCTSTR pszFormat, ...)
{
	if (!m_bEnabled)
		return;

	if ((m_nDefaultLevel & m_nMaskLevel) == 0)
		return;

	TCHAR buf[2*1024] = {0};
	va_list arglist;
	va_start(arglist, pszFormat);
	if (pszFormat != NULL)
		_vstprintf(buf, pszFormat, arglist);
	va_end(arglist);
	_tcscat(buf, _T("\n"));
	
	CString msg = GetPrefix(m_nDefaultLevel);
	msg += buf;

	Write(msg);
}

/**
 * @brief Write message from resource with default level
 */
void CLogFile::Write(DWORD idFormatString, ...)
{
	if (!m_bEnabled)
		return;

	if ((m_nDefaultLevel & m_nMaskLevel) == 0)
		return;

	TCHAR buf[2*1024]=_T("");
	CString strFormat;

	if (strFormat.LoadString(idFormatString))
	{
		va_list arglist;
		va_start(arglist, idFormatString);
		_vstprintf(buf, strFormat, arglist);
		va_end(arglist);
		_tcscat(buf, _T("\n"));
		
		CString msg = GetPrefix(m_nDefaultLevel);
		msg += buf;
		
		Write(msg);
	}
}

/**
 * @brief Write formatted message to log with given level
 */
void CLogFile::Write(UINT level, LPCTSTR pszFormat, ...)
{
	if (!m_bEnabled)
		return;

	if ((level & m_nMaskLevel) == 0)
		return;

	TCHAR buf[4*1024] = {0};
	va_list arglist;
	va_start(arglist, pszFormat);
	if (pszFormat != NULL)
		_vstprintf(buf, pszFormat, arglist);
	va_end(arglist);
	_tcscat(buf, _T("\n"));
	
	CString msg = GetPrefix(level);
	msg += buf;

	Write(msg);
}

/**
 * @brief Write message from resource to log with given level
 */
void CLogFile::Write(UINT level, DWORD idFormatString, ...)
{
	if (!m_bEnabled)
		return;

	if ((level & m_nMaskLevel) == 0)
		return;

	TCHAR buf[2*1024]=_T("");
	CString strFormat;

	if (strFormat.LoadString(idFormatString))
	{
		va_list arglist;
		va_start(arglist, idFormatString);
		_vstprintf(buf, strFormat, arglist);
		va_end(arglist);
		_tcscat(buf, _T("\n"));
		
		CString msg = GetPrefix(m_nDefaultLevel);
		msg += buf;
		
		Write(msg);
	}
}

/**
 * @brief Write new line to log.
 * @note this function is used only internally by other write-functions.
 */
void CLogFile::Write(CString msg)
{
	DWORD dwWaitRes = WaitForSingleObject(m_hLogMutex, 10000);

	if (dwWaitRes == WAIT_OBJECT_0)
	{
		FILE *f;
		if ((f=_tfopen(m_strLogPath, _T("a"))) != NULL)
		{
			_fputts(msg, f);

			// prune the log if it gets too big
			if (ftell(f) >= (int)m_nMaxSize)
				Prune(f);
			else
				fclose(f);
		}
		ReleaseMutex(m_hLogMutex);
	}
}

void CLogFile::WriteError(CString JobID, CString ProcessID, CString Event, long ecode, CString CIndex)
{
	if (!m_bEnabled)
		return;
	
	JobID.TrimRight();
	ProcessID.TrimRight();
	Event.TrimRight();
	CIndex.TrimRight();
	
	CString sWriteString;
	
	sWriteString.Format(_T("%s %s %s %ld %s"),JobID, ProcessID, Event, ecode, CIndex);
	Write(sWriteString);
}

/**
 * @brief Prune log file if it exceeds max given size.
 */
void CLogFile::Prune(FILE *f)
{
	TCHAR buf[8196] = {0};
	DWORD amt;
	TCHAR tempfile[MAX_PATH];
	FILE *tf;
	GetTempFileName(_T("."),_T("LOG"),0,tempfile);
	DeleteFile(tempfile);
	if ((tf=_tfopen(tempfile,_T("w"))) != NULL)
	{
		fseek(f, ftell(f)/4, SEEK_SET);
		_fputts(_T("#### The log has been truncated due to size limits ####\n"), tf);

		while ((amt=fread(buf, 1, 8196, f)) > 0)
			fwrite(buf, amt, 1, tf);
		fclose(tf);
		fclose(f);
		DeleteFile(m_strLogPath);
		MoveFile(tempfile,m_strLogPath);
	}
}

/**
 * @brief Return message prefix for given loglevel.
 */
CString CLogFile::GetPrefix(UINT level) const
{
	CString str;
	switch (level)
	{
		case LOGLEVEL::LERROR:
			str = _T("ERROR: ");
			break;
		case LOGLEVEL::LWARNING:
			str = _T("WARNING: ");
			break;
		case LOGLEVEL::LNOTICE:
			str = _T("NOTICE: ");
			break;
		case LOGLEVEL::LMSG:
			break;
		case LOGLEVEL::LCODEFLOW:
			str = _T("FLOW: ");
			break;
		case LOGLEVEL::LCOMPAREDATA:
			str = _T("COMPARE: ");
			break;
		default:
			break;
	}
	return str;
}
