/** 
 * @file  DirViewColItems.h
 *
 * @brief Declaration file for DirColInfo
 *
 * @date  Created: 2003-08-19
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#ifndef DirViewColItems_h
#define DirViewColItems_h


typedef CString (*ColGetFnc)(const DIFFITEM & di);
typedef int (*ColSortFnc)(const DIFFITEM & ldi, const DIFFITEM &rdi);

/**
 * @brief Information about one column of dirview list info
 */
struct DirColInfo
{
	LPCTSTR regName;
	// localized string resources
	int idName;
	int idDesc;
	ColGetFnc getfnc; /**< Handler giving display string */
	ColSortFnc sortfnc; /**< Handler for sorting this column */
	int physicalIndex; /**< Current physical index, -1 if not displayed */
	bool defSortUp; /**< Does column start with ascending sort (most do) */
};

extern DirColInfo g_cols[];
extern int g_ncols;


#endif // DirViewColItems_h
