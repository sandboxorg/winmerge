/** 
 * @file  OptionsDef.h
 *
 * @brief Constants for option-names
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#ifndef _OPTIONS_DEF_H_
#define _OPTIONS_DEF_H_

// View-menu
const TCHAR OPT_SHOW_UNIQUE_LEFT[] = _T("Settings/ShowUniqueLeft");
const TCHAR OPT_SHOW_UNIQUE_RIGHT[] = _T("Settings/ShowUniqueRight");
const TCHAR OPT_SHOW_DIFFERENT[] = _T("Settings/ShowDifferent");
const TCHAR OPT_SHOW_IDENTICAL[] = _T("Settings/ShowIdentical");
const TCHAR OPT_SHOW_BINARIES[] = _T("Settings/ShowBinaries");
const TCHAR OPT_SHOW_SKIPPED[] = _T("Settings/ShowSkipped");
const TCHAR OPT_HIDE_BACKUP[] = _T("Settings/HideBak");

const TCHAR OPT_SYNTAX_HIGHLIGHT[] = _T("Settings/HiliteSyntax");
const TCHAR OPT_DISABLE_SPLASH[] = _T("Settings/DisableSplash");
const TCHAR OPT_CREATE_BACKUPS[] = _T("Settings/BackupFile");
const TCHAR OPT_VIEW_WHITESPACE[] =  _T("Settings/ViewWhitespace");
const TCHAR OPT_CONNECT_MOVED_BLOCKS[] = _T("Settings/ConnectMovedBlocks");
const TCHAR OPT_SCROLL_TO_FIRST[] =  _T("Settings/ScrollToFirst");

const TCHAR OPT_AUTOMATIC_RESCAN[] = _T("Settings/AutomaticRescan");
const TCHAR OPT_ALLOW_MIXED_EOL[] = _T("Settings/AllowMixedEOL");
const TCHAR OPT_TAB_SIZE[] = _T("Settings/TabSize");
const TCHAR OPT_TAB_TYPE[] = _T("Settings/TabType");
const TCHAR OPT_UNREC_APPLYSYNTAX[] = _T("Settings/ApplyUnrecSyntax");

const TCHAR OPT_EXT_EDITOR_CMD[] = _T("Settings/ExternalEditor");
const TCHAR OPT_USE_RECYCLE_BIN[] = _T("Settings/UseRecycleBin");
const TCHAR OPT_AUTOCLOSE_CMPPANE[] = _T("Settings/AutoCloseCmpPane");
const TCHAR OPT_MERGE_MODE[] = _T("Settings/MergingMode");
const TCHAR OPT_CLOSE_WITH_ESC[] = _T("Settings/CloseWithEsc");
const TCHAR OPT_DETECT_CODEPAGE[] = _T("Settings/DetectCodepage");
const TCHAR OPT_LOGGING[] = _T("Settings/Logging");
const TCHAR OPT_VERIFY_OPEN_PATHS[] = _T("Settings/VerifyOpenPaths");

// Color options
// The difference color
const TCHAR OPT_CLR_DIFF[] = _T("Settings/DifferenceColor");
// The selected difference color
const TCHAR OPT_CLR_SELECTED_DIFF[] = _T("Settings/SelectedDifferenceColor");
// The difference deleted color
const TCHAR OPT_CLR_DIFF_DELETED[] = _T("Settings/DifferenceDeletedColor");
// The selected difference deleted color
const TCHAR OPT_CLR_SELECTED_DIFF_DELETED[] = _T("Settings/SelectedDifferenceDeletedColor");
// The difference text color
const TCHAR OPT_CLR_DIFF_TEXT[] = _T("Settings/DifferenceTextColor");
// The selected difference text color
const TCHAR OPT_CLR_SELECTED_DIFF_TEXT[] = _T("Settings/SelectedDifferenceTextColor");
// The ignored lines color
const TCHAR OPT_CLR_TRIVIAL_DIFF[] = _T("Settings/TrivialDifferenceColor");
// The ignored and deleted lines color
const TCHAR OPT_CLR_TRIVIAL_DIFF_DELETED[] = _T("Settings/TrivialDifferenceDeletedColor");
// The ignored text color
const TCHAR OPT_CLR_TRIVIAL_DIFF_TEXT[] = _T("Settings/TrivialDifferenceTextColor");
// The moved block color
const TCHAR OPT_CLR_MOVEDBLOCK[] = _T("Settings/MovedBlockColor");
// The moved block deleted lines color
const TCHAR OPT_CLR_MOVEDBLOCK_DELETED[] = _T("Settings/MovedBlockDeletedColor");
// The moved block text color
const TCHAR OPT_CLR_MOVEDBLOCK_TEXT[] = _T("Settings/MovedBlockTextColor");
// The selected moved block color
const TCHAR OPT_CLR_SELECTED_MOVEDBLOCK[] = _T("Settings/SelectedMovedBlockColor");
// The selected moved block deleted lines
const TCHAR OPT_CLR_SELECTED_MOVEDBLOCK_DELETED[] = _T("Settings/SelectedMovedBlockDeletedColor");
// The selected moved block text color
const TCHAR OPT_CLR_SELECTED_MOVEDBLOCK_TEXT[] = _T("Settings/SelectedMovedBlockTextColor");

// Compare options
const TCHAR OPT_CMP_IGNORE_WHITESPACE[] = _T("Settings/IgnoreSpace");
const TCHAR OPT_CMP_IGNORE_BLANKLINES[] = _T("Settings/IgnoreBlankLines");
const TCHAR OPT_CMP_IGNORE_CASE[] = _T("Settings/IgnoreCase");
const TCHAR OPT_CMP_EOL_SENSITIVE[] = _T("Settings/EolSensitive");
const TCHAR OPT_CMP_METHOD[] = _T("Settings/CompMethod");
const TCHAR OPT_CMP_MOVED_BLOCKS[] = _T("Settings/MovedBlocks");

#endif // _OPTIONS_DEF_H_
