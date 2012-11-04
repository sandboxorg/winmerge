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
 * @file  DiffThread.h
 *
 * @brief Declaration file for CDiffThread
 */
// ID line follows -- this is updated by SVN
// $Id$

#ifndef _DIFFTHREAD_H
#define _DIFFTHREAD_H

#include <boost/scoped_ptr.hpp>
#include <Poco/Thread.h>
#include "DiffContext.h"
#include <windows.h>

namespace Poco
{
class Semaphore;
}
struct DiffFuncStruct;
class DiffThreadAbortable;
class CWinThread;

/**
 * @brief Class for threaded folder compare.
 * This class implements folder compare in two phases and in two threads:
 * - first thread collects items to compare to compare-time list
 *   (m_diffList).
 * - second threads compares items in the list.
 */
class CDiffThread
{
public:
	/** @brief Thread's states. */
	enum ThreadState
	{
		THREAD_NOTSTARTED = 0, /**< Thread not started, idle. */
		THREAD_COMPARING, /**< Thread running (comparing). */
		THREAD_COMPLETED, /**< Thread has completed its task. */
	};

// creation and use, called on main thread
	CDiffThread();
	~CDiffThread();
	void SetContext(CDiffContext * pCtx);
	unsigned CompareDirectories();
	void SetHwnd(HWND hWnd);
	void SetMessageIDs(unsigned updateMsg);
	void SetCompareSelected(bool bSelected = false);

// runtime interface for main thread, called on main thread
	unsigned GetThreadState() const;
	void Abort() { m_bAborting = true; }
	bool IsAborting() const { return m_bAborting; }

// runtime interface for child thread, called on child thread
	bool ShouldAbort() const;

private:
	CDiffContext * m_pDiffContext; /**< Compare context storing results. */
	Poco::Thread m_threads[2]; /**< Compare threads. */
	boost::scoped_ptr<DiffFuncStruct> m_pDiffParm; /**< Structure for sending data to threads. */
	boost::scoped_ptr<DiffThreadAbortable> m_pAbortgate;
	unsigned m_msgUpdateUI; /**< UI-update message number */
	HWND m_hWnd; /**< Handle to folder compare GUI window */
	bool m_bAborting; /**< Is compare aborting? */
	bool m_bOnlyRequested; /**< Are we comparing only requested items (Update?) */
};

/**
 * @brief Structure used in sending data to the threads.
 * As thread functions have only one parameter we must pack all
 * the data we need inside structure.
 */
struct DiffFuncStruct
{
	CDiffContext * context; /**< Compare context. */
	unsigned msgUIUpdate; /**< Windows message for updating GUI. */
	HWND hWindow; /**< Window getting status updates. */
	CDiffThread::ThreadState nThreadState; /**< Thread state. */
	DiffThreadAbortable * m_pAbortgate; /**< Interface for aborting compare. */
	bool bOnlyRequested; /**< Compare only requested items? */
	Poco::Semaphore *pSemaphore; /**< Semaphore for synchronizing threads. */

	DiffFuncStruct()
		: context(NULL)
		, msgUIUpdate(0)
		, hWindow(0)
		, nThreadState(CDiffThread::THREAD_NOTSTARTED)
		, m_pAbortgate(NULL)
		, bOnlyRequested(false)
		, pSemaphore(NULL)
		{}
};

#endif /* _DIFFTHREAD_H */
