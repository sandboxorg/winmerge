/** 
 * @file  paths.h
 *
 * @brief Declaration file for path routines
 */
// RCS ID line follows -- this is updated by CVS
// $Id$

#ifndef paths_h_included
#define paths_h_included

typedef enum { DOES_NOT_EXIST, IS_EXISTING_FILE, IS_EXISTING_DIR } PATH_EXISTENCE;
typedef enum { DIRSLASH, NODIRSLASH } DIRSLASH_TYPE;

PATH_EXISTENCE paths_DoesPathExist(LPCTSTR szPath);
void paths_normalize(CString & sPath);
CString paths_GetLongPath(const CString & sPath);
bool paths_CreateIfNeeded(const CString & sPath);
LPCTSTR paths_GetTempPath();
PATH_EXISTENCE GetPairComparability(LPCTSTR pszLeft, LPCTSTR pszRight);
CString ExpandShortcut(const CString &inFile);
CString paths_ConcatPath(const CString & path, const CString & subpath);
CString paths_GetParentPath(CString path);
bool IsSlash(LPCTSTR pszStart, int nPos);
BOOL paths_IsPathAbsolute(const CString &path);

#endif // paths_h_included
