/////////////////////////////////////////////////////////////////////////////
//    see Merge.cpp for license (GPLv2+) statement
//
/////////////////////////////////////////////////////////////////////////////
/**
 *  @file DirActions.cpp
 *
 *  @brief Implementation of methods of CDirView that copy/move/delete files
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

// It would be nice to make this independent of the UI (CDirView)
// but it needs access to the list of selected items.
// One idea would be to provide an iterator over them.
//

#include "stdafx.h"
#include "Merge.h"
#include "DirView.h"
#include "DirDoc.h"
#include "MainFrm.h"
#include "coretools.h"
#include "OutputDlg.h"
#include "paths.h"
#include "7zCommon.h"
#include "CShellFileOp.h"
#include "OptionsDef.h"
#include "WaitStatusCursor.h"
#include "LogFile.h"
#include "DiffItem.h"
#include "FileActionScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLogFile gLog;

// Prompt user to confirm a multiple item copy
static BOOL ConfirmMultipleCopy(int count, int total)
{
	CString s;
	ASSERT(count>1);
	AfxFormatString2(s, IDS_CONFIRM_COPY2DIR, NumToStr(count), NumToStr(total));
	int rtn = AfxMessageBox(s, MB_YESNO | MB_ICONWARNING | MB_DONT_ASK_AGAIN, IDS_CONFIRM_COPY2DIR);
	return (rtn==IDYES);
}

// Prompt user to confirm a single item copy
static BOOL ConfirmSingleCopy(LPCTSTR src, LPCTSTR dest)
{
	CString s;
	AfxFormatString2(s, IDS_CONFIRM_COPY_SINGLE, src, dest);
	int rtn = AfxMessageBox(s, MB_YESNO | MB_ICONWARNING | MB_DONT_ASK_AGAIN, IDS_CONFIRM_COPY_SINGLE);
	return (rtn==IDYES);
}

// Prompt user to confirm a multiple item delete
static BOOL ConfirmMultipleDelete(int count, int total)
{
	CString s;
	AfxFormatString2(s, IDS_CONFIRM_DELETE_ITEMS, NumToStr(count), NumToStr(total));
	int rtn = AfxMessageBox(s, MB_YESNO | MB_ICONWARNING | MB_DONT_ASK_AGAIN, IDS_CONFIRM_DELETE_ITEMS);
	return (rtn==IDYES);
}

// Prompt user to confirm a single item delete
static BOOL ConfirmSingleDelete(LPCTSTR filepath)
{
	CString s;
	AfxFormatString1(s, IDS_CONFIRM_DELETE_SINGLE, filepath);
	int rtn = AfxMessageBox(s, MB_YESNO | MB_ICONWARNING | MB_DONT_ASK_AGAIN, IDS_CONFIRM_DELETE_SINGLE);
	return (rtn==IDYES);
}

// Prompt & copy item from right to left, if legal
void CDirView::DoCopyRightToLeft()
{
	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_COPYFILES));

	// First we build a list of desired actions
	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_COPY;
	int selCount = 0;
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (di.diffcode != 0 && IsItemCopyableToLeft(di))
		{
			GetItemFileNames(sel, slFile, srFile);
			FileActionItem act;
			act.src = srFile;
			act.dest = slFile;
			act.context = sel;
			act.dirflag = di.isDirectory();
			act.atype = actType;
			act.UIResult = FileActionItem::UI_SYNC;
			actionScript.actions.AddTail(act);
		}
		++selCount;
	}

	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}
// Prompt & copy item from left to right, if legal
void CDirView::DoCopyLeftToRight()
{
	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_COPYFILES));

	// First we build a list of desired actions
	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_COPY;
	int selCount = 0;
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (di.diffcode != 0 && IsItemCopyableToRight(di))
		{
			GetItemFileNames(sel, slFile, srFile);
			FileActionItem act;
			act.src = slFile;
			act.dest = srFile;
			act.dirflag = di.isDirectory();
			act.context = sel;
			act.atype = actType;
			act.UIResult = FileActionItem::UI_SYNC;
			actionScript.actions.AddTail(act);
		}
		++selCount;
	}

	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}

// Prompt & delete left, if legal
void CDirView::DoDelLeft()
{
	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_DELETEFILES));

	// First we build a list of desired actions
	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_DEL;
	int selCount = 0;
	int sel=-1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (di.diffcode != 0 && IsItemDeletableOnLeft(di))
		{
			GetItemFileNames(sel, slFile, srFile);
			FileActionItem act;
			act.src = slFile;
			act.dirflag = di.isDirectory();
			act.context = sel;
			act.atype = actType;
			act.UIResult = FileActionItem::UI_DEL_LEFT;
			actionScript.actions.AddTail(act);
		}
		++selCount;
	}

	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}
// Prompt & delete right, if legal
void CDirView::DoDelRight()
{
	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_DELETEFILES));

	// First we build a list of desired actions
	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_DEL;
	int selCount = 0;
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);

		if (di.diffcode != 0 && IsItemDeletableOnRight(di))
		{
			GetItemFileNames(sel, slFile, srFile);
			FileActionItem act;
			act.src = srFile;
			act.dirflag = di.isDirectory();
			act.context = sel;
			act.atype = actType;
			act.UIResult = FileActionItem::UI_DEL_RIGHT;
			actionScript.actions.AddTail(act);
		}
		++selCount;
	}

	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}

/**
 * @brief Prompt & delete both, if legal.
 */
void CDirView::DoDelBoth()
{
	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_DELETEFILES));

	// First we build a list of desired actions
	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_DEL;
	int selCount = 0;
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);

		if (di.diffcode != 0 && IsItemDeletableOnBoth(di))
		{
			GetItemFileNames(sel, slFile, srFile);
			FileActionItem act;
			act.src = srFile;
			act.dest = slFile;
			act.dirflag = di.isDirectory();
			act.context = sel;
			act.atype = actType;
			act.UIResult = FileActionItem::UI_DEL_BOTH;
			actionScript.actions.AddTail(act);
		}
		++selCount;
	}

	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}

/**
 * @brief Delete left, right or both items.
 */
void CDirView::DoDelAll()
{
	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_DELETEFILES));

	// First we build a list of desired actions
	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_DEL;
	int selCount = 0;
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);

		if (di.diffcode != 0)
		{
			GetItemFileNames(sel, slFile, srFile);
			FileActionItem act;
			if (IsItemDeletableOnBoth(di))
			{
				act.src = srFile;
				act.dest = slFile;
			}
			else if (IsItemDeletableOnLeft(di))
			{
				act.src = slFile;
			}
			else if (IsItemDeletableOnRight(di))
			{
				act.src = srFile;
			}
			act.dirflag = di.isDirectory();
			act.context = sel;
			act.atype = actType;
			act.UIResult = FileActionItem::UI_DEL_BOTH;
			actionScript.actions.AddTail(act);
		}
		++selCount;
	}

	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}

/**
 * @brief Copy selected left-side files to user-specified directory
 *
 * When copying files from recursive compare file subdirectory is also
 * read so directory structure is preserved.
 * @note CShellFileOp takes care of much of error handling
 */
void CDirView::DoCopyLeftTo()
{
	CShellFileOp fileOp;
	CString destPath;
	CString startPath;
	CString msg;

	VERIFY(msg.LoadString(IDS_SELECT_DEST_LEFT));
	if (!SelectFolder(destPath, startPath, msg))
		return;

	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_COPYFILES));

	fileOp.SetOperationFlags(FO_COPY, this, FOF_NOCONFIRMMKDIR);
	if (!GetDocument()->GetRecursive())
		fileOp.AddDestFile(destPath);

	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);

		if (di.diffcode != 0 && IsItemCopyableToOnLeft(di))
		{
			if (GetDocument()->GetRecursive())
			{
				CString sFullDest(destPath);
				sFullDest += _T("\\");
				if (!di.sLeftSubdir.IsEmpty())
					sFullDest += di.sLeftSubdir + _T("\\");
				sFullDest += di.sLeftFilename;
				fileOp.AddDestFile(sFullDest);
			}
			GetItemFileNames(sel, slFile, srFile);
			fileOp.AddSourceFile(slFile);
		}
	}

	BOOL bSuccess = FALSE;
	BOOL bAPICalled = FALSE;
	BOOL bAborted = FALSE;
	int  nAPIReturn = 0;
	bSuccess = fileOp.Go(&bAPICalled, &nAPIReturn, &bAborted);
}

/**
 * @brief Copy selected righ-side files to user-specified directory
 *
 * When copying files from recursive compare file subdirectory is also
 * read so directory structure is preserved.
 * @note CShellFileOp takes care of much of error handling
 */
void CDirView::DoCopyRightTo()
{
	CShellFileOp fileOp;
	CString destPath;
	CString startPath;
	CString msg;

	VERIFY(msg.LoadString(IDS_SELECT_DEST_RIGHT));
	if (!SelectFolder(destPath, startPath, msg))
		return;

	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_COPYFILES));

	fileOp.SetOperationFlags(FO_COPY, this, FOF_NOCONFIRMMKDIR);
	if (!GetDocument()->GetRecursive())
		fileOp.AddDestFile(destPath);

	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);

		if (di.diffcode != 0 && IsItemCopyableToOnRight(di))
		{
			if (GetDocument()->GetRecursive())
			{
				CString sFullDest(destPath);
				sFullDest += _T("\\");
				if (!di.sRightSubdir.IsEmpty())
					sFullDest += di.sRightSubdir + _T("\\");
				sFullDest += di.sRightFilename;
				fileOp.AddDestFile(sFullDest);
			}
			GetItemFileNames(sel, slFile, srFile);
			fileOp.AddSourceFile(srFile);
		}
	}

	BOOL bSuccess = FALSE;
	BOOL bAPICalled = FALSE;
	BOOL bAborted = FALSE;
	int  nAPIReturn = 0;
	bSuccess = fileOp.Go( &bAPICalled, &nAPIReturn, &bAborted );
}

/**
 * @brief Move selected left-side files to user-specified directory
 *
 * When moving files from recursive compare file subdirectory is also
 * read so directory structure is preserved.
 * @note CShellFileOp takes care of much of error handling
 */
void CDirView::DoMoveLeftTo()
{
	CString destPath;
	CString startPath;
	CString msg;

	VERIFY(msg.LoadString(IDS_SELECT_DEST_LEFT));
	if (!SelectFolder(destPath, startPath, msg))
		return;

	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_MOVEFILES));

	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_MOVE;
	int selCount = 0;
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);

		if (di.diffcode != 0 && IsItemCopyableToOnLeft(di) && IsItemDeletableOnLeft(di))
		{
			FileActionItem act;
			CString sFullDest(destPath);
			sFullDest += _T("\\");
			if (GetDocument()->GetRecursive())
			{
				if (!di.sLeftSubdir.IsEmpty())
					sFullDest += di.sLeftSubdir + _T("\\");
			}
			sFullDest += di.sLeftFilename;
			act.dest = sFullDest;

			GetItemFileNames(sel, slFile, srFile);
			act.src = slFile;
			act.dirflag = di.isDirectory();
			act.context = sel;
			act.atype = actType;
			act.UIResult = FileActionItem::UI_DEL_LEFT;
			actionScript.actions.AddTail(act);
			++selCount;
		}
	}
	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}

/**
 * @brief Move selected right-side files to user-specified directory
 *
 * When moving files from recursive compare file subdirectory is also
 * read so directory structure is preserved.
 * @note CShellFileOp takes care of much of error handling
 */
void CDirView::DoMoveRightTo()
{
	CString destPath;
	CString startPath;
	CString msg;

	VERIFY(msg.LoadString(IDS_SELECT_DEST_RIGHT));
	if (!SelectFolder(destPath, startPath, msg))
		return;

	WaitStatusCursor waitstatus(LoadResString(IDS_STATUS_MOVEFILES));

	FileActionScript actionScript;
	const FileAction::ACT_TYPE actType = FileAction::ACT_MOVE;
	int selCount = 0;
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);

		if (di.diffcode != 0 && IsItemCopyableToOnRight(di) && IsItemDeletableOnRight(di))
		{
			FileActionItem act;
			CString sFullDest(destPath);
			sFullDest += _T("\\");
			if (GetDocument()->GetRecursive())
			{
				if (!di.sRightSubdir.IsEmpty())
					sFullDest += di.sRightSubdir + _T("\\");
			}
			sFullDest += di.sRightFilename;
			act.dest = sFullDest;

			GetItemFileNames(sel, slFile, srFile);
			act.src = srFile;
			act.dirflag = di.isDirectory();
			act.context = sel;
			act.atype = actType;
			act.UIResult = FileActionItem::UI_DEL_RIGHT;
			actionScript.actions.AddTail(act);
			++selCount;
		}
	}
	// Now we prompt, and execute actions
	ConfirmAndPerformActions(actionScript, selCount);
}

// Confirm with user, then perform the action list
void CDirView::ConfirmAndPerformActions(FileActionScript & actionList, int selCount)
{
	if (selCount == 0) // Not sure it is possible to get right-click menu without
		return;    // any selected items, but may as well be safe

	ASSERT(actionList.GetCount()>0); // Or else the update handler got it wrong

	if (!ConfirmActionList(actionList, selCount))
		return;

	PerformActionList(actionList);
}

/**
 * @brief Confirm actions with user as appropriate
 * (type, whether single or multiple).
 */
BOOL CDirView::ConfirmActionList(const FileActionScript & actionList, int selCount)
{
	// TODO: We need better confirmation for file actions.
	// Maybe we should show a list of files with actions done..
	const FileActionItem & item = actionList.actions.GetHead();

	// special handling for the single item case, because it is probably the most common,
	// and we can give the user exact details easily for it
	switch(item.atype)
	{
	case FileAction::ACT_COPY:
		if (actionList.GetCount() == 1)
		{
			if (!ConfirmSingleCopy(item.src, item.dest))
				return FALSE;
		}
		else
		{
			if (!ConfirmMultipleCopy(actionList.GetCount(), selCount))
				return FALSE;
		}
		break;
		
	// Deleting does not need confirmation, CShellFileOp takes care of it
	case FileAction::ACT_DEL:
	// Moving does not need confirmation, CShellFileOp takes care of it
	case FileAction::ACT_MOVE:
		break;

	// Invalid operation
	default: 
		LogErrorString(_T("Unknown fileoperation in CDirView::ConfirmActionList()"));
		_RPTF0(_CRT_ERROR, "Unknown fileoperation in CDirView::ConfirmActionList()");
		break;
	}
	return TRUE;
}

/**
 * @brief Perform an array of actions
 * @note There can be only COPY or DELETE actions, not both!
 * @sa CMainFrame::SaveToVersionControl()
 * @sa CMainFrame::SyncFilesToVCS()
 */
void CDirView::PerformActionList(FileActionScript & actionScript)
{
	// Reset suppressing VSS dialog for multiple files.
	// Set in CMainFrame::SaveToVersionControl().
	mf->m_CheckOutMulti = FALSE;
	mf->m_bVssSuppressPathCheck = FALSE;

	// Check option and enable putting deleted items to Recycle Bin
	if (GetOptionsMgr()->GetBool(OPT_USE_RECYCLE_BIN))
		actionScript.UseRecycleBin(TRUE);
	else
		actionScript.UseRecycleBin(FALSE);

	actionScript.SetParentWindow(this);

	if (actionScript.Run())
		UpdateAfterFileScript(actionScript);
}

/**
 * @brief Update DirView ui after running FileActionScript.
 */
void CDirView::UpdateAfterFileScript(FileActionScript & actionList)
{
	BOOL bItemsRemoved = FALSE;
	int curSel = GetFirstSelectedInd();
	CDirDoc *pDoc = GetDocument();
	while (actionList.GetCount()>0)
	{
		// Start handling from tail of list, so removing items
		// doesn't invalidate our item indexes.
		FileActionItem act = actionList.actions.RemoveTail();
		POSITION diffpos = GetItemKey(act.context);
		const DIFFITEM & di = pDoc->GetDiffByKey(diffpos);

		switch (act.UIResult)
		{
		case FileActionItem::UI_SYNC:
			if (mf->m_bCheckinVCS)
				mf->CheckinToClearCase(act.dest);

			// Syncronized item is both-sides item
			pDoc->SetDiffSide(DIFFCODE::BOTH, act.context);
			
			// Folders don't have compare flag set!!
			if (act.dirflag)
				pDoc->SetDiffCompare(DIFFCODE::NOCMP, act.context);
			else
				pDoc->SetDiffCompare(DIFFCODE::SAME, act.context);

			pDoc->SetDiffCounts(0, 0, act.context);
			pDoc->ReloadItemStatus(act.context, TRUE, TRUE);

			break;
		
		case FileActionItem::UI_DESYNC:
			// Cannot happen yet since we have only "simple" operations
			break;

		case FileActionItem::UI_DEL_LEFT:
			if (di.isSideLeft())
			{
				pDoc->RemoveDiffByKey(diffpos);
				m_pList->DeleteItem(act.context);
				bItemsRemoved = TRUE;
			}
			else
			{
				pDoc->SetDiffSide(DIFFCODE::RIGHT, act.context);
				pDoc->SetDiffCompare(DIFFCODE::NOCMP, act.context);
				pDoc->ReloadItemStatus(act.context, TRUE, FALSE);
			}
			break;

		case FileActionItem::UI_DEL_RIGHT:
			if (di.isSideRight())
			{
				pDoc->RemoveDiffByKey(diffpos);
				m_pList->DeleteItem(act.context);
				bItemsRemoved = TRUE;
			}
			else
			{
				pDoc->SetDiffSide(DIFFCODE::LEFT, act.context);
				pDoc->SetDiffCompare(DIFFCODE::NOCMP, act.context);
				pDoc->ReloadItemStatus(act.context, FALSE, TRUE);
			}
			break;

		case FileActionItem::UI_DEL_BOTH:
			pDoc->RemoveDiffByKey(diffpos);
			m_pList->DeleteItem(act.context);
			bItemsRemoved = TRUE;
			break;
		}
	}
	
	// Make sure selection is at sensible place if all selected items
	// were removed.
	if (bItemsRemoved == TRUE)
	{
		UINT selected = GetSelectedCount();
		if (selected == 0)
		{
			if (curSel < 1)
				++curSel;
			MoveFocus(0, curSel - 1, selected);
		}
	}
}

/// Get directories of first selected item
BOOL CDirView::GetSelectedDirNames(CString& strLeft, CString& strRight) const
{
	BOOL bResult = GetSelectedFileNames(strLeft, strRight);

	if (bResult)
	{
		strLeft = GetPathOnly(strLeft);
		strRight = GetPathOnly(strRight);
	}
	return bResult;
}

/// is it possible to copy item to left ?
BOOL CDirView::IsItemCopyableToLeft(const DIFFITEM & di) const
{
	// don't let them mess with error items
	if (di.isResultError()) return FALSE;
	// can't copy same items
	if (di.isResultSame()) return FALSE;
	// impossible if only on left
	if (di.isSideLeft()) return FALSE;

	// everything else can be copied to left
	return TRUE;
}
/// is it possible to copy item to right ?
BOOL CDirView::IsItemCopyableToRight(const DIFFITEM & di) const
{
	// don't let them mess with error items
	if (di.isResultError()) return FALSE;
	// can't copy same items
	if (di.isResultSame()) return FALSE;
	// impossible if only on right
	if (di.isSideRight()) return FALSE;

	// everything else can be copied to right
	return TRUE;
}
/// is it possible to delete left item ?
BOOL CDirView::IsItemDeletableOnLeft(const DIFFITEM & di) const
{
	// don't let them mess with error items
	if (di.isResultError()) return FALSE;
	// impossible if only on right
	if (di.isSideRight()) return FALSE;
	// everything else can be deleted on left
	return TRUE;
}
/// is it possible to delete right item ?
BOOL CDirView::IsItemDeletableOnRight(const DIFFITEM & di) const
{
	// don't let them mess with error items
	if (di.isResultError()) return FALSE;
	// impossible if only on right
	if (di.isSideLeft()) return FALSE;

	// everything else can be deleted on right
	return TRUE;
}
/// is it possible to delete both items ?
BOOL CDirView::IsItemDeletableOnBoth(const DIFFITEM & di) const
{
	// don't let them mess with error items
	if (di.isResultError()) return FALSE;
	// impossible if only on right or left
	if (di.isSideLeft() || di.isSideRight()) return FALSE;

	// everything else can be deleted on both
	return TRUE;
}

/// is it possible to open item for compare ?
BOOL CDirView::IsItemOpenable(const DIFFITEM & di) const
{
	// impossible if unique or binary
	if (di.isSideRight() || di.isSideLeft() || di.isBin()) return FALSE;

	// everything else can be opened
	return TRUE;
}
/// is it possible to compare these two items?
BOOL CDirView::AreItemsOpenable(const DIFFITEM & di1, const DIFFITEM & di2) const
{
	CString sLeftBasePath = GetDocument()->GetLeftBasePath();
	CString sRightBasePath = GetDocument()->GetRightBasePath();
	CString sLeftPath1 = paths_ConcatPath(di1.getLeftFilepath(sLeftBasePath), di1.sLeftFilename);
	CString sLeftPath2 = paths_ConcatPath(di2.getLeftFilepath(sLeftBasePath), di2.sLeftFilename);
	CString sRightPath1 = paths_ConcatPath(di1.getRightFilepath(sRightBasePath), di1.sRightFilename);
	CString sRightPath2 = paths_ConcatPath(di2.getRightFilepath(sRightBasePath), di2.sRightFilename);
	// Must not be binary (unless archive)
	if
	(
		(di1.isBin() || di2.isBin())
	&&!	(
			HasZipSupport()
		&&	(sLeftPath1.IsEmpty() || ArchiveGuessFormat(sLeftPath1))
		&&	(sRightPath1.IsEmpty() || ArchiveGuessFormat(sRightPath1))
		&&	(sLeftPath2.IsEmpty() || ArchiveGuessFormat(sLeftPath2))
		&&	(sRightPath2.IsEmpty() || ArchiveGuessFormat(sRightPath2))
		)
	)
	{
		return FALSE;
	}

	// Must be both directory or neither
	if (di1.isDirectory() != di2.isDirectory()) return FALSE;

	// Must be on different sides, or one on one side & one on both
	if (di1.isSideLeft() && (di2.isSideRight() || di2.isSideBoth()))
		return TRUE;
	if (di1.isSideRight() && (di2.isSideLeft() || di2.isSideBoth()))
		return TRUE;
	if (di1.isSideBoth() && (di2.isSideLeft() || di2.isSideRight()))
		return TRUE;

	// Allow to compare items if left & right path refer to same directory
	// (which means there is effectively two files involved). No need to check
	// side flags. If files weren't on both sides, we'd have no DIFFITEMs.
	if (sLeftBasePath.CompareNoCase(sRightBasePath) == 0)
		return TRUE;

	return FALSE;
}
/// is it possible to open left item ?
BOOL CDirView::IsItemOpenableOnLeft(const DIFFITEM & di) const
{
	// impossible if only on right
	if (di.isSideRight()) return FALSE;

	// everything else can be opened on right
	return TRUE;
}
/// is it possible to open right item ?
BOOL CDirView::IsItemOpenableOnRight(const DIFFITEM & di) const
{
	// impossible if only on left
	if (di.isSideLeft()) return FALSE;

	// everything else can be opened on left
	return TRUE;
}
/// is it possible to open left ... item ?
BOOL CDirView::IsItemOpenableOnLeftWith(const DIFFITEM & di) const
{
	return (!di.isDirectory() && IsItemOpenableOnLeft(di));
}
/// is it possible to open with ... right item ?
BOOL CDirView::IsItemOpenableOnRightWith(const DIFFITEM & di) const
{
	return (!di.isDirectory() && IsItemOpenableOnRight(di));
}
/// is it possible to copy to... left item?
BOOL CDirView::IsItemCopyableToOnLeft(const DIFFITEM & di) const
{
	// impossible if only on right
	if (di.isSideRight()) return FALSE;

	// everything else can be copied to from left
	return TRUE;
}
/// is it possible to copy to... right item?
BOOL CDirView::IsItemCopyableToOnRight(const DIFFITEM & di) const
{
	// impossible if only on left
	if (di.isSideLeft()) return FALSE;

	// everything else can be copied to from right
	return TRUE;
}

/// get the file names on both sides for first selected item
BOOL CDirView::GetSelectedFileNames(CString& strLeft, CString& strRight) const
{
	int sel = m_pList->GetNextItem(-1, LVNI_SELECTED);
	if (sel == -1)
		return FALSE;
	GetItemFileNames(sel, strLeft, strRight);
	return TRUE;
}
/// get file name on specified side for first selected item
CString CDirView::GetSelectedFileName(SIDE_TYPE stype) const
{
	CString left, right;
	if (!GetSelectedFileNames(left, right)) return _T("");
	return stype==SIDE_LEFT ? left : right;
}
/**
 * @brief Get the file names on both sides for specified item.
 * @note Return empty strings if item is special item.
 */
void CDirView::GetItemFileNames(int sel, CString& strLeft, CString& strRight) const
{
	POSITION diffpos = GetItemKey(sel);
	if (diffpos == (POSITION)SPECIAL_ITEM_POS)
	{
		strLeft.Empty();
		strRight.Empty();
	}
	else
	{
		const DIFFITEM & di = GetDocument()->GetDiffByKey(diffpos);
		const CString leftrelpath = paths_ConcatPath(di.sLeftSubdir, di.sLeftFilename);
		const CString rightrelpath = paths_ConcatPath(di.sRightSubdir, di.sRightFilename);
		const CString & leftpath = GetDocument()->GetLeftBasePath();
		const CString & rightpath = GetDocument()->GetRightBasePath();
		strLeft = paths_ConcatPath(leftpath, leftrelpath);
		strRight = paths_ConcatPath(rightpath, rightrelpath);
	}
}

/**
 * @brief Get the file names on both sides for specified item.
 * @note Return empty strings if item is special item.
 */
void CDirView::GetItemFileNames(int sel, PathContext * paths) const
{
	CString strLeft;
	CString strRight;
	GetItemFileNames(sel, strLeft, strRight);
	paths->SetLeft(strLeft);
	paths->SetRight(strRight);
}

/**
 * @brief Open selected file with registered application.
 * Uses shell file associations to open file with registered
 * application. We first try to use "Edit" action which should
 * open file to editor, since we are more interested editing
 * files than running them (scripts).
 * @param [in] stype Side of file to open.
 */
void CDirView::DoOpen(SIDE_TYPE stype)
{
	int sel = GetSingleSelectedItem();
	if (sel == -1) return;
	CString file = GetSelectedFileName(stype);
	if (file.IsEmpty()) return;
	int rtn = (int)ShellExecute(::GetDesktopWindow(), _T("edit"), file, 0, 0, SW_SHOWNORMAL);
	if (rtn==SE_ERR_NOASSOC)
		rtn = (int)ShellExecute(::GetDesktopWindow(), _T("open"), file, 0, 0, SW_SHOWNORMAL);
	if (rtn==SE_ERR_NOASSOC)
		DoOpenWith(stype);
}

/// Open with dialog for file on selected side
void CDirView::DoOpenWith(SIDE_TYPE stype)
{
	int sel = GetSingleSelectedItem();
	if (sel == -1) return;
	CString file = GetSelectedFileName(stype);
	if (file.IsEmpty()) return;
	CString sysdir;
	if (!GetSystemDirectory(sysdir.GetBuffer(MAX_PATH), MAX_PATH)) return;
	sysdir.ReleaseBuffer();
	CString arg = (CString)_T("shell32.dll,OpenAs_RunDLL ") + file;
	ShellExecute(::GetDesktopWindow(), 0, _T("RUNDLL32.EXE"), arg, sysdir, SW_SHOWNORMAL);
}

/// Open selected file  on specified side to external editor
void CDirView::DoOpenWithEditor(SIDE_TYPE stype)
{
	int sel = GetSingleSelectedItem();
	if (sel == -1) return;
	CString file = GetSelectedFileName(stype);
	if (file.IsEmpty()) return;

	mf->OpenFileToExternalEditor(file);
}

/**
 * @brief Apply specified setting for prediffing to all selected items
 */
void CDirView::ApplyPluginPrediffSetting(int newsetting)
{
	// Unlike other group actions, here we don't build an action list
	// to execute; we just apply this change directly
	int sel=-1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		const DIFFITEM& di = GetDiffItem(sel);
		if (!di.isDirectory() && !di.isSideLeft() && !di.isSideRight())
		{
			GetItemFileNames(sel, slFile, srFile);
			CString filteredFilenames = slFile + (CString)_T("|") + srFile;
			GetDocument()->SetPluginPrediffSetting(filteredFilenames, newsetting);
		}
	}
}

/**
 * @brief Mark selected items as needing for rescan.
 */
void CDirView::MarkForRescan()
{
	int sel = -1;
	CString slFile, srFile;
	while ((sel = m_pList->GetNextItem(sel, LVNI_SELECTED)) != -1)
	{
		DIFFITEM di = GetDiffItem(sel);
		GetDocument()->SetDiffStatus(DIFFCODE::NEEDSCAN, DIFFCODE::SCANFLAGS, sel);
	}
	GetDocument()->SetMarkedRescan();
}
