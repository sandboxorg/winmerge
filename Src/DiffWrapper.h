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
 * @file  DiffWrapper.h
 *
 * @brief Declaration file for CDiffWrapper.
 *
 * @date  Created: 2003-08-22
 */
// ID line follows -- this is updated by SVN
// $Id: DiffWrapper.h 7091 2010-01-11 20:27:43Z kimmov $

#ifndef _DIFFWRAPPER_H
#define _DIFFWRAPPER_H

#include "diff.h"
#include "FileLocation.h"
#include "PathContext.h"
#include "FileTextStats.h"
#include "CompareOptions.h"
#include "DiffList.h"
#include "UnicodeString.h"

class CDiffContext;
class PrediffingInfo;
struct DiffFileData;
class PathContext;
struct file_data;
class FilterCommentsManager;
struct FilterCommentsSet;
class MovedLines;
class FilterList;

/** @enum COMPARE_TYPE
 * @brief Different foldercompare methods.
 * These values are the foldercompare methods WinMerge supports.
 */

/** @var CMP_CONTENT
 * @brief Normal by content compare.
 * This compare type is first, default and all-seeing compare type.
 * diffutils is used for producing compare results. So all limitations
 * of diffutils (like buffering) apply to this compare method. But this
 * is also currently only compare method that produces difference lists
 * we can use in file compare.
 */

/** @var CMP_QUICK_CONTENT
 * @brief Faster byte per byte -compare.
 * This type of compare was a response for needing faster compare results
 * in folder compare. It independent from diffutils, and fully customised
 * for WinMerge. It basically does byte-per-byte compare, still implementing
 * different whitespace ignore options.
 *
 * Optionally this compare type can be stopped when first difference is found.
 * Which gets compare as fast as possible. But misses sometimes binary files
 * if zero bytes aren't found before first difference. Also difference counts
 * are not useful with that option.
 */

/** @var CMP_DATE
 * @brief Compare by modified date.
 * This compare type was added after requests and realization that in some
 * situations difference in file's timestamps is enough to judge them
 * different. E.g. when modifying files in local machine, file timestamps
 * are certainly different after modifying them. This method doesn't even
 * open files for reading them. It only reads file's infos for timestamps
 * and compares them.
 *
 * This is no doubt fastest way to compare files.
 */

/** @var CMP_DATE_SIZE
 * @brief Compare by date and then by size.
 * This method is basically same than CMP_DATE, but it adds check for file
 * sizes if timestamps are identical. This is because there are situations
 * timestamps can't be trusted alone, especially with network shares. Adding
 * checking for file sizes adds some more reliability for results with
 * minimal increase in compare time.
 */

/** @var CMP_SIZE
 * @brief Compare by file size.
 * This compare method compares file sizes. This isn't quite accurate method,
 * other than it can detect files that certainly differ. But it can show lot of
 * different files as identical too. Advantage is in some use cases where different
 * size always means files are different. E.g. automatically created logs - when
 * more data is added size increases.
 */
enum COMPARE_TYPE
{
	CMP_CONTENT = 0,
	CMP_QUICK_CONTENT,
	CMP_DATE,
	CMP_DATE_SIZE,
	CMP_SIZE,
};

/**
 * @brief Additional options for creating patch files
 */
struct PATCHOPTIONS
{
	enum output_style outputStyle; /**< Patch file style. */
	int nContext; /**< Number of context lines. */
	bool bAddCommandline; /**< Add diff-style commandline to patch file. */
};

typedef enum {
	IDENTLEVEL_NONE,
	IDENTLEVEL_ALL,
	IDENTLEVEL_EXCEPTLEFT,
	IDENTLEVEL_EXCEPTMIDDLE,
	IDENTLEVEL_EXCEPTRIGHT,
} IDENTLEVEL;
/**
 * @brief Diffutils returns this statusdata about files compared
 */
struct DIFFSTATUS
{
	bool bMissingNL[3]; /**< file is missing EOL before EOF */
	bool bBinaries; /**< Files are binaries */
	IDENTLEVEL Identical; /**< diffutils said files are identical */
	bool bPatchFileFailed; /**< Creating patch file failed */

	DIFFSTATUS() { memset(this, 0, sizeof(*this)); } // start out with all flags clear
};

class FilterCommentsManager;

/**
 * @brief Wrapper class for diffengine (diffutils and ByteComparator).
 * Diffwappre class is used to run selected diffengine. For folder compare
 * there are several methods (COMPARE_TYPE), but for file compare diffutils
 * is used always. For file compare diffutils can output results to external
 * DiffList or to patch file. Output type must be selected with member
 * functions SetCreatePatchFile() and SetCreateDiffList().
 */
class CDiffWrapper
{
public:
	CDiffWrapper();
	~CDiffWrapper();
	void SetCreatePatchFile(const String &filename);
	void SetCreateDiffList(DiffList *diffList);
	void SetDiffList(DiffList *diffList);
	void GetOptions(DIFFOPTIONS *options);
	void SetOptions(const DIFFOPTIONS *options);
	void SetTextForAutomaticPrediff(const String &text);
	void SetPrediffer(PrediffingInfo * prediffer =NULL);
	void GetPrediffer(PrediffingInfo * prediffer);
	void SetPatchOptions(const PATCHOPTIONS *options);
	void SetDetectMovedBlocks(bool bDetectMovedBlocks);
	bool GetDetectMovedBlocks() { return (m_pMovedLines[0] != NULL); }
	void SetAppendFiles(bool bAppendFiles);
	void SetPaths(const PathContext &files, bool tempPaths);
	void SetAlternativePaths(const PathContext &altPaths);
	void SetCodepage(int codepage) { m_codepage = codepage; }
	bool RunFileDiff();
	void GetDiffStatus(DIFFSTATUS *status);
	void AddDiffRange(DiffList *pDiffList, unsigned begin0, unsigned end0, unsigned begin1, unsigned end1, OP_TYPE op);
	void AddDiffRange(DiffList *pDiffList, DIFFRANGE &dr);
	void FixLastDiffRange(int nFiles, int bufferLines[], bool bMissingNL[], bool bIgnoreBlankLines);
	MovedLines * GetMovedLines(int index) { return m_pMovedLines[index]; }
	void SetCompareFiles(const PathContext &originalFile);
	int Make3wayDiff(DiffList& diff3, DiffList& diff02, DiffList& diff10, DiffList& diff21);
	void WritePatchFileHeader(enum output_style output_style, bool bAppendFiles);
	void WritePatchFileTerminator(enum output_style output_style);
	void SetFilterList(const String& filterStr);
	void EnablePlugins(bool enable);
	bool IsTrivialBytes(const char* Start, const char* End,
		const FilterCommentsSet& filtercommentsset);
	bool IsTrivialLine(const std::string &Line, const char * StartOfComment,
	   const char * EndOfComment, const char * InLineComment,
	   const FilterCommentsSet& filtercommentsset);
	bool PostFilter(int StartPos, int EndPos, int Direction,
		int QtyLinesInBlock, OP_TYPE &Op, int FileNo,
		const FilterCommentsSet& filtercommentsset);
	void PostFilter(int LineNumberLeft, int QtyLinesLeft, int LineNumberRight,
		int QtyLinesRight, OP_TYPE &Op, const FilterCommentsManager &filtercommentsmanager,
		const String& FileNameExt);

protected:
	String FormatSwitchString();
	bool Diff2Files(struct change ** diffs, DiffFileData *diffData,
		int * bin_status, int * bin_file);
	void LoadWinMergeDiffsFromDiffUtilsScript(struct change * script, const file_data * inf);
	void WritePatchFile(struct change * script, file_data * inf);
public:
	void LoadWinMergeDiffsFromDiffUtilsScript3(
		struct change * script10, struct change * script02, struct change * script12,
		const file_data * inf10, const file_data * inf02, const file_data * inf12);
	void FreeDiffUtilsScript3(struct change * & script10, struct change * & script12, struct change * & script02);
	bool RegExpFilter(int StartPos, int EndPos, int FileNo);

private:
	DiffutilsOptions m_options;
	DIFFSTATUS m_status; /**< Status of last compare */
	FilterList * m_pFilterList; /**< List of linefilters. */
	PathContext m_files; /**< Full path to diff'ed file. */
	PathContext m_alternativePaths; /**< file's alternative path (may be relative). */
	PathContext m_originalFile; /**< file's original (NON-TEMP) path. */

	String m_sPatchFile; /**< Full path to created patch file. */
	bool m_bPathsAreTemp; /**< Are compared paths temporary? */
	/// prediffer info are stored only for MergeDoc
	PrediffingInfo * m_infoPrediffer;
	/// prediffer info are stored only for MergeDoc
	String m_sToFindPrediffer;
	bool m_bUseDiffList; /**< Are results returned in difflist? */
	bool m_bCreatePatchFile; /**< Do we create a patch file? */
	bool m_bAddCmdLine; /**< Do we add commandline to patch file? */
	bool m_bAppendFiles; /**< Do we append to existing patch file? */
	int m_nDiffs; /**< Difference count */
	int m_codepage; /**< Codepage used in line filter */
	DiffList *m_pDiffList; /**< Pointer to external DiffList */
	MovedLines * m_pMovedLines[3];
	FilterCommentsManager * m_FilterCommentsManager; /**< Comments filtering manager */
	bool m_bPluginsEnabled; /**< Are plugins enabled? */
};


#endif // _DIFFWRAPPER_H
