#include "stdafx.h"
#include "diffcontext.h"
#include "diffthread.h"
#include "diff.h"

// Static structure for sharing data with thread
static DiffFuncStruct diffParam;

int compare_files (LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR, CDiffContext*, int);

CDiffThread::CDiffThread()
{
	m_pDiffContext = NULL;
	m_thread = NULL;
	diffParam.nThreadState = THREAD_NOTSTARTED;
}

CDiffThread::~CDiffThread()
{

}

CDiffContext * CDiffThread::SetContext(CDiffContext * pCtx)
{
	CDiffContext *pTempContext = m_pDiffContext;
	m_pDiffContext = pCtx;
	return pTempContext;
}

UINT CDiffThread::CompareDirectories(CString dir1, CString dir2)
{
	diffParam.path1 = dir1;
	diffParam.path2 = dir2;
	diffParam.context = m_pDiffContext;
	diffParam.msgUIUpdate = m_msgUpdateUI;
	diffParam.hWindow = m_hWnd;

	diffParam.nThreadState = THREAD_COMPARING;
	m_thread = AfxBeginThread(DiffThread, (LPVOID)&diffParam);
	return 1;
}

void CDiffThread::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CDiffThread::SetMessageIDs(UINT updateMsg, UINT statusMsg)
{
	m_msgUpdateUI = updateMsg;
	m_msgUpdateStatus = statusMsg;
}

UINT CDiffThread::GetThreadState()
{
	return diffParam.nThreadState;
}

UINT DiffThread(LPVOID lpParam)
{
	DiffFuncStruct *myStruct = (DiffFuncStruct *) lpParam;
	HWND hWnd = myStruct->hWindow;
	UINT msgID = myStruct->msgUIUpdate;

	compare_files (0, (char const *)(LPCTSTR)myStruct->path1, 
			       0, (char const *)(LPCTSTR)myStruct->path2,
				   myStruct->context, 0);

	// Send message to UI to update
	diffParam.nThreadState = THREAD_COMPLETED;
	SendMessage(hWnd, msgID, NULL, NULL);
	return 1;
}
