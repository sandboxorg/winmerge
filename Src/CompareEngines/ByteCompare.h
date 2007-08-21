/** 
 * @file  ByteCompare.h
 *
 * @brief Declaration file for ByteCompare
 */
// ID line follows -- this is updated by SVN
// $Id$

#ifndef _BYTE_COMPARE_H_
#define _BYTE_COMPARE_H_

#include "UnicodeString.h"
#include "FileTextStats.h"

class QuickCompareOptions;
class IAbortable;

namespace CompareEngines
{

class ByteCompare
{
public:
	ByteCompare();
	~ByteCompare();

	bool SetCompareOptions(const CompareOptions & options);
	void ClearCompareOptions();
	void SetAdditionalOptions(BOOL stopAfterFirstDiff);
	void SetAbortable(const IAbortable * piAbortable);
	void SetPaths(LPCTSTR path1, LPCTSTR path2);
	void SetFileData(int items, file_data *data);
	int CompareFiles();
	void GetTextStats(int side, FileTextStats *stats);

private:
	QuickCompareOptions *m_pOptions; /**< Compare options for diffutils. */
	IAbortable * m_piAbortable;
	file_data * m_inf; /**< Compared files data (for diffutils). */
	String m_paths[2];
	FileTextStats m_textStats[2];

};

} // namespace CompareEngines

#endif // _BYTE_COMPARE_H_
