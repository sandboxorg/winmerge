2003-08-07 Kimmo
 PATCH: [ 781013 ] 256 Color icon
  editlib/res: Merge.ico

2003-08-07 Perry
 Fix GetItemFileNames to not pull its data from listview (b/c columns may be reordered).
 (Various const fixes needed.)
 WinMerge: DiffContext.cpp DiffContext.h DirActions.cpp DirView.cpp DirView.h

2003-08-05 Laoran
 PATCH: [ 781582 ] Timer for FlushAndRescan
  WinMerge: MergeDoc.cpp MergeDoc.h MergeEditView.cpp MergeEditView.h

2003-08-04 Laoran
 PATCH: [ 782866 ] round off in CMergeDoc::RescanIfNeeded
  WinMerge: MergeDoc.cpp

2003-08-04 Kimmo
 PATCH: [ 780359 ] Open identical files
  WinMerge: DirView.cpp MainFrm.cpp

2003-07-31 Laoran
 PATCH: [ 780827 ] Tooltips for DirView
  Submitted by Kimmo
  WinMerge: DirView.cpp DirView.h

2003-07-31 Laoran
 PATCH: [ 780508 ] Convert Line Length array to CArray
  WinMerge: ccrystaltextview.cpp ccrystaltextview.h 

2003-07-31 Laoran
 PATCH: [ 780067 ] SUndoRecord::SetText must free previous text
  WinMerge: ccrystaltextbuffer.cpp ccrystaltextbuffer.h 

2003-07-30 Laoran
 PATCH: [ 779470 ] disable unwanted options in PropGeneral
  WinMerge: PropGeneral.cpp PropGeneral.h 

2003-07-30 Laoran
 PATCH: [ 777871 ] New headerbar for the file paths 
  WinMerge: ChildFrm.cpp ChildFrm.h dllver.cpp dllver.h Merge.dsp Merge.rc resource.h
  WinMerge new files : EditorFilepathBar.cpp EditorFilepathBar.h FilepathEdit.cpp FilepathEdit.h

2003-07-30 Perry
 PATCH: [ 778379 ] Drag&drop reordering columns
  WinMerge: DirDoc.cpp DirView.cpp DirView.h Merge.dsp dllver.cpp dllver.h
 Annotate UpdateResources methods
  WinMerge: DirDoc.cpp MainFrm.cpp MergeEditView.cpp

2003-07-29 Perry
 PATCH: [ 779224 ] Fix sort bug in right date
  WinMerge: DirView.cpp
 bugfix for PATCH: [ 765225]
  editlib: ccrystaltextview.cpp ccrystaltextview.h

2003-07-29 Kimmo
 PATCH: [ 779172 ] Saving fix for 2.0 and 2.1
  WinMerge: MergeDoc.cpp
 PATCH: [ 779005 ] No temp files when saving local temp files
  WinMerge: MergeDoc.cpp
 Fix Register.bat comments. Batch files use ":" for comments...
  ShellExtension: Register.bat
  
2003-07-29 Perry
 Some fixes to MergeChineseTraditional.rc from Koko
  Languages\ChineseTraditional\MergeChineseTraditional.rc

2003-07-28 Perry
 PATCH: [ 778234 ] Fix DirScan ASSERT when run out of right list
  WinMerge: DirScan.cpp
 PATCH: [ 765225 ] Convert parse cookie array to be CArray
  editlib: ccrystaltextview.cpp ccrystaltextview.h

2003-07-27 Kimmo
 PATCH: [ 778144 ] Use CopyFile instead of DeleteFile() and MoveFile()
  WinMerge: MainFrm.cpp
 PATCH: [ 778384 ] Sync OnDropFiles() from MainFrame to SuperComboBox
  common: SuperComboBox.cpp
 PATCH: [ 778403 ] Fix accel keys in lang files
  Languages: ChineseSimplified, ChineseTraditional 

2003-07-27 Perry
 PATCH: [ 778290 ] Typo in comment
  WinMerge: MergeDoc.cpp

2003-07-26 Kimmo
 BUG: [ 778166 ] Shell integration missing quotes (2.1)
  ShellExtension: WinMergeShell.cpp

2003-07-25 Laoran
 [ 776114 ] french keyboard shortcut
  Languages: french.rc

2003-07-25 Kimmo
 PATCH: [ 773296 ] Add shell integration
  Added new module ShellExtension
  WinMerge: Merge.rc PropRegistry.h PropRegistry.cpp resource.h
  Languages: all *.rc and resource.h files

2003-07-24 Kimmo
 PATCH: [ 776106 ] Fix for TEMP dir error
  Submitted by Laoran
  WinMerge: MergeDoc.cpp

2003-07-24 Perry
 PATCH: [ 766287 ] Use our own dir traversal (instead of diff's)
  WinMerge: Diff.cpp DiffContext.h DiffThread.cpp DirDoc.cpp Merge.dsp 
   DirScan.h DirScan.cpp
 PATCH: [ 773981 ] Modify language build process to eliminate msgboxes
  (also added a version resource to MakeResDll, as it lacked one)
  Languages: BuildAll.bat BuildDll.bat
  MakeResDll: MakeResDll.cpp MakeResDll.rc
 Added F4 accelerator to all languages besides English
  Languages: all *.rc files

2003-07-24 Kimmo
 PATCH: [ 776525 ] Close doc after failed load
  WinMerge: MainFrm.cpp

2003-07-23 Perry
 PATCH: [ 776046 ] Fixes for <Highlight Line Diff> menu item
  WinMerge: Merge.rc MergeEditView.cpp
  Languages: All Merge*.rc files and resource.h files
 PATCH: [ 771666 ] Fix for English descriptions of Chinese
  common: LanguageSelect.rc
  WinMerge: Merge.rc

2003-07-21 Perry
 Added ChineseTraditional, ChineseSimplified, Korean, translations.
 PATCH: [ 775000 ] Improve const correctness in crystaltextbuffer
  editlib: crystaltextbuffer.cpp crystaltextbuffer.h

2003-07-18 Perry
 PATCH: [ 767147 ] Don't clear existing dirdoc until user saves
  WinMerge: DirDoc.cpp
 PATCH: [ 763616 ] New menu item: Highlight line diff
  WinMerge: Merge.rc MergeDoc.cpp MergeDoc.h MergeEditView.cpp MergeEditView.h resource.h
  editlib: ccrystaltextbuffer.cpp ccrystaltextbuffer.h ccrystaltextview.cpp ccrystaltextview.h

2003-07-18 Kimmo
 PATCH: [ 772843 ] Remember custom colors
  WinMerge: PropColors.h PropColors.cpp

2003-07-18 Laoran
 PATCH : [ 771587 ] Undo/Redo functions 
  WinMerge: MainFrm.cpp MergeDoc.cpp MergeDoc.h MergeEditView.cpp
  editlib: ccrystaleditview.cpp ccrystaleditview.h ccrystaltextbuffer.cpp ccrystaltextbuffer.h 

2003-07-17 Laoran
 PATCH : [ 772908 ] ResetView must reset m_nTopSubLine
  editlib: ccrystaltextview.cpp

2003-07-17 Laoran
 PATCH : [ 769535 ] Preserve cursor during Rescan
  WinMerge: MergeDoc.cpp 
  editlib: ccrystaltextbuffer.cpp ccrystaltextbuffer.h ccrystaltextview.cpp ccrystaltextview.h

2003-07-17 Laoran
 PATCH: [ 767878 ] Better handling of ghost flag and EOL
  WinMerge: MergeDoc.cpp MergeEditView.cpp
  editlib: ccrystaleditview.cpp ccrystaltextbuffer.cpp ccrystaltextbuffer.h

2003-07-17 Kimmo
 PATCH: [ 772228 ] Quick bin check for 2.0
  WinMerge: files.h files.cpp MainFrm.cpp MergeDoc.cpp

2003-07-16 Laoran
 PATCH: [ 765926 ] Merge empty last line to the last diffrange (change)
 new version
  WinMerge: MergeDoc.cpp

2003-07-16 Laoran
 PATCH: [ 771525 ] Updating cursor in ListCopy
  WinMerge: MergeDoc.cpp MergeDoc.h
  editlib: ccrystaltextbuffer.cpp ccrystaltextbuffer.h

2003-07-16 Laoran
 PATCH: [ 771503 ] correct UpdateView context in internalInsertText
  editlib: ccrystaltextbuffer.cpp

2003-07-16 Kimmo
 PATCH: [ 768740 ] Additional color options
  Submitted by Gilbert Wellisch
  WinMerge: MainFrm.cpp Merge.cpp Merge.h Merge.rc MergeEditView.cpp
   PropColors.cpp PropColors.h resource.h
  Updated other languages

2003-07-15 Laoran
 PATCH: [ 771625 ] regression for patch [765399]
  WinMerge: MergeEditView.cpp 

2003-07-15 Perry
 PATCH: [ 770361 ] Slovak translation
  Languages: New Slovak subdirectory

2003-07-12 Laoran
 PATCH: [ 765399 ] Careful view initialization in PrimeListWithFile
  WinMerge: MergeDoc.cpp MergeEditView.cpp 
  editlib: ccrystaltextview.cpp

2003-07-12 Laoran
 PATCH: [ 769122 ] Keep top screen line within the shorter panel
  WinMerge: MergeEditView.cpp 

2003-07-11 Perry
 PATCH: [ 765150 ] Fix char count in edit view bottom status line
  WinMerge: ChildFrm.cpp ChildFrm.h Merge.rc MergeEditStatus.h 
   MergeEditView.cpp resource.h
  editlib: ccrystaltextview.cpp ccrystaltextview.h
  Languages: Updated all resource.h and rc files to include two new strings

2003-07-11 Kimmo
 PATCH: [ 769295 ] Fix for threaded dir compare crash
  WinMerge: DiffThread.h DiffThread.cpp DirDoc.h DirDoc.cpp DirView.h DirView.cpp

2003-07-10 Kimmo
 PATCH: [ 766727 ] Threaded dir compare
  WinMerge: DiffContext.h DiffContext.cpp DirDoc.h DirDoc.cpp
   DirView.h DirView.cpp MainFrm.h MainFrm.cpp Merge.dsp
   New files: DiffThread.h DiffThread.cpp

2003-07-10 Laoran
 PATCH: [ 768378 ] use Rescan Timer for Replace All 
  WinMerge: MergeEditView.cpp

2003-07-09 Kimmo
 PATCH: [ 765768 ] Only one Rescan on AllToLeft
  Submitted by Perry
  WinMerge: MergeDoc.h MergeDoc.cpp MergeEditView.cpp

2003-07-08 Kimmo
 PATCH: [ 766473 ] Update identical files
  WinMerge: MergeDoc.cpp

2003-07-08 Laoran
 PATCH: [ 767159 ] merge LF_RIGHT_ONLY and LF_LEFT_ONLY -> LF_GHOST 
  WinMerge: MergeDoc.cpp MergeEditView.cpp
  editlib: ccrystaltextbuffer.h ccrystaltextview.cpp

2003-07-08 Laoran
 PATCH: [ 767051 ] code cleanup for GetTextWithoutEmptys 
  WinMerge: MergeDoc.cpp  MergeDoc.h MergeEditView.cpp
  editlib: ccrystaltextbuffer.cpp ccrystaltextbuffer.h

2003-07-08 Perry
 PATCH: [ 765165 ] Log error string if CreateFile fails (to debug window)
  WinMerge: files.cpp MainFrm.cpp MainFrm.h MergeDoc.cpp StdAfx.cpp StdAfx.h

2003-07-07 Kimmo
 PATCH: [ 767204 ] Fix for assert when rescanning with selection
  Submitted by Gilbert Wellisch
  editlib: ccrystaltextview.cpp

2003-07-05 Laoran
 PATCH: [ 765399 ] resize m_pdwParseCookies before GoToLine
  WinMerge: MergeDoc.cpp  MergeEditView.cpp

2003-07-05 Laoran
 PATCH: [ 765926 ] Merge empty last line to the last diffrange (change)
  WinMerge: MergeDoc.cpp

2003-07-04 Laoran
 PATCH: [ 766038 ] stay in the viewing area during OnFirstdiff
  WinMerge: MergeEditView.cpp

2003-07-04 Kimmo
 PATCH: [ 765463 ] Simplify temp path handling a little
  WinMerge: MergeDoc.h MergeDoc.cpp

2003-07-03 Kimmo
 PATCH: [ 764571 ] Ask about saving when closing with esc
  WinMerge: MainFrm.cpp MergeDoc.cpp MergeEditView.cpp
 PATCH: [ 763499 ] update french language
  Submitted by Laoran
  Lang/MergeFrench.rc

2003-07-03 Perry
 PATCH: [ 763265 ] Fix handling of .. and . components
  WinMerge: paths.cpp

2003-07-02 Perry
 PATCH: [ 764789 ] Fix Private Build string in rc file
  WinMerge: Merge.rc

2003-07-02 Kimmo
 PATCH: [ 763817 ] Hilight diffs when syntax hilight is off
  WinMerge: MergeEditView.cpp
 PATCH: [ 762531 ] Inproper globals in editlib/cs2cs.cpp
  Submitted by jtuc
  editlib: cs2cs.cpp
 PATCH: [ 762529 ] Dirframe trying to clear non-existing pane
  Submitted by jtuc
  WinMerge: DirFrame.cpp
  
2003-06-30 WinMerge beta 2.1.0.0 released

2003-06-30 Laoran
 [ 762394 ] update german language
  Languages: German\MergeGerman.rc

2003-06-30 Perry
 PATCH: [ 762835 ] Display line number etc beneath editor
  WinMerge: ChildFrm.cpp ChildFrm.h DirFrame.cpp Merge.cpp Merge.dsp 
    MergeEditStatus.h MergeEditView.cpp MergeEditView.h
  editlib: ccrystaltextbuffer.h ccrystaltextview.cpp ccrystaltextview.h

2003-06-29 Perry
 PATCH: [ 758912 ] Combine duplicated code in CMergeEditView::OnEditCopy
  WinMerge: MergeEditView.cpp
 PATCH: [ 762533 ] Stdafx.h has dublicated and unneeded includes
  WinMerge: StdAfx.h
 PATCH: [ 762271 ] Alter ListCopy to copy eol chars
  WinMerge: MergeDoc.cpp MergeDoc.h
  editlib: ccrystaltextbuffer.h ccrystaltextbuffer.cpp

2003-06-29 Kimmo
 PATCH: [ 762749 ] Forced rescan after options changed
  WinMerge: MainFrm.cpp

2003-06-28 Kimmo
 PATCH: [ 761874 ] Fix unlocalisable acceskeys
  WinMerge: Merge.rc
  editlib: editres.rc
  Updated language files too
 PATCH: [ 761827 ] Current diff item to select inactive diff
  WinMerge: MergeDoc.h MergeDoc.cpp MergeEditView.cpp
 PATCH: [ 762098 ] Code cleanups from Jochen
  Submitted by jtuc
  WinMerge: MainFrm.h MainFrm.cpp MergeDoc.cpp MergeEditView.cpp

2003-06-28 Perry
 PATCH: [ 758930 ] Display EOL chars with View Whitespace
  WinMerge: MainFrm.cpp
  editlib: ccrystaltextview.h ccrystaltextview.cpp
 PATCH: [ 762268 ] Fix typo in comment (wenn -> when)
  editlib: ccrystaltextview.cpp

2003-06-27 Kimmo
 PATCH: [ 761951 ] Fix scrolling corruption
  Submitted by Jochen
  editlib: ccrystaltextview.cpp

2003-06-27 Laoran
 PATCH: [ 760745 ] Fix empty search field detection and better history in FindDlg and EditReplaceDlg
  editlib : ccrystaleditview.cpp ccrystaltextview.cpp ceditreplacedlg.cpp ceditreplacedlg.h
    cfindtextdlg.cpp cfindtextdlg.h memcombo.cpp memcombo.h
  
2003-06-26 Christian
 PATCH: [ 754001 ] 2.0 doc link points to 1.7 docs
  WinMerge: MainFrm.cpp

2003-06-26 Kimmo
 PATCH: [ 755918 ] Fix typo in CCrystalTextView extension list
  Added more filename extensions
  editlib: ccrystaltextview.cpp
 PATCH: [ 757180 ] Update Open OK on dropped file
  Submitted by Perry
  common: SuperComboBox.cpp

2003-06-25 Kimmo
 PATCH: [ 759755 ] Better rescan error reporting
  WinMerge: MainFrm.cpp MergeDoc.h MergeDoc.cpp
 PATCH: [ 760441 ] Fix recursive d&d compare
  WinMerge: MainFrm.cpp

2003-06-24 Perry
 PATCH: [ 759778 ] Minor cleanup of crystaltextbuffer.cpp
  editlib: ccrystaltextbuffer.cpp
 PATCH: [ 760077 ] Omit final ghostline if last line unterminated
  WinMerge: MergeDoc.cpp
 PATCH: [ 760129 ] Wipe Undo stack if Undo fails, and fix Undo check
  editlib: ccrystaltextbuffer.cpp ccrystaltextbuffer.h

2003-06-23 Perry
 PATCH: [ 759533 ] Fix InternalInsertText & InternalDeleteText
  editlib: ccrystaltextbuffer.cpp
 PATCH: [ 759569 ] Fix Undo/Redo & line mappings
  editlib: ccrystaltextbuffer.cpp ccrystaltextbuffer.h ccrystaleditview.cpp

2003-06-23 Kimmo
 PATCH: [ 759344 ] Add Copy, Cut & Paste to Edit menu
  WinMerge: Merge.rc
  Updated other languages too - translations from Perry and Christian

2003-06-22 Perry
 PATCH: [ 758870 ] Return filesize+1 rather than 0 for new lines at end
  editlib: crystaltextbuffer.cpp crystaltextbuffer.h
 PATCH: [ 758900 ] Fix bug in pasting contiguous empty lines.
  editlib: crystaltextbuffer.cpp
 PATCH: [ 756218 ] Fix copy & paste typo in comment in Merge.cpp
  WinMerge: Merge.cpp

2003-06-21 Perry
 PATCH: [ 716456 ] Update Undo/Redo info with rescan line mapping
  WinMerge: MergeDoc.cpp MergeEditView.h
  editlib: crystaltextbuffer.cpp crystaltextbuffer.h

2003-06-19 Perry
 PATCH: [ 728153 ] If opening first file fails, don't open second
  WinMerge: MainFrm.cpp

2003-06-17 Kimmo
 PATCH: [ 755918 ] Fix typo in CCrystalTextView extension list
  editlib: ccrystaltextview.cpp

2003-06-16 Christian
 PATCH [ 754015 ] WinMerge installer imrovements
  InnoSetup: Info.txt WinMerge.iss WinMergeIcon.bmp WinMergeLarge.bmp

2003-06-12 Kimmo
 PATCH: [ 753284 ] Fix cursor slowdown after rescan
  editlib: ccrystaltextview.cpp

2003-06-08 Kimmo
 PATCH: [ 750160 ] Char width fix
   editlib: ccrystaltextview.cpp

2003-06-07 Kimmo
 PATCH: [ 750302 ] Allow *.flt filter files
  WinMerge: Merge.cpp

2003-06-06 Kimmo
 Updated filter file from patch [ 742041 ] Improvements to file/directory filters
  Filters: Merge_VC_loose.flt
 BUG: [ 749007 ] "Highlighting" is misspelled as "Hilighting"
  WinMerge: Merge.rc

2003-06-05 WinMerge 2.0.0 Release Candidate 1 released

2003-06-05 Christian
 PATCH: [ 749566 ] Updated resource files
  Languages: all resource.h files and all Merge*.rc files

2003-06-04 Christian
 Updated Czech and Danish resources
  Languages: Czech\resource.h Czech\MergeCzech.rc Danish\resource.h Danish\MergeDanish.rc

2003-06-02 Kimmo
 PATCH: [ 747090 ] Fix scroll handling
  editlib: ccrystaltextview.cpp

2003-06-01 Kimmo
 PATCH: [ 746319 ] Adjust horiz scrolling width
  editlib: ccrystaltextview.cpp

2003-05-31 Kimmo
 PATCH: [ 745654 ] Draw long lines
  WinMerge: MergeEditView.cpp
  editlib: ccrystaltextview.cpp
 PATCH: [ 742041 ] Improvements to file/directory filters
  WinMerge: FileFilterMgr.h FileFilterMgr.cpp Merge.cpp
 PATCH: [ 738557 ] Allow "Open Left" for directories
  WinMerge: DirActions.cpp DirView.h DirView.cpp

2003-05-29 Christian
 PATCH: Applied [ 745223 ] Fix pasting different EOL types
  edilib: ccrystaltextbuffer.cpp
 BUG: [ 744990 ] Pasting to the right

2003-05-29 Kimmo
 Made new folder \Filters for filter files.  Added Perry's original filter files
 working with beta 1.9.2.2

2003-05-28 Kimmo
 PATCH: [ 744872 ] Use CopyFile()
  WinMerge: files.h files.cpp MergeDoc.cpp

2003-05-26 Kimmo
 PATCH: [ 743284 ] String fixes
  WinMerge: DirDoc.cpp DirView.cpp MainFrm.cpp MergeDoc.h paths.cpp
  editlib: ccrystaleditview.cpp ccrystaltextview.cpp registry.cpp

2003-05-25 Kimmo
 PATCH: [ 742735 ] Fix CCrystalTextBuffer::InsertLine()
  edilib: ccrystaltextbuffer.cpp

2003-05-23 Christian
 Made new folder \InnoSetup\ for files related to making WinMerge installer using InnoSetup

2003-05-22 Perry
 PATCH: [ 741265 ] Fix for bug#741263: 1.9.2.2 crash on refresh
  WinMerge: Diff.cpp DiffContext.cpp DiffContext.h DirDoc.cpp MainFrm.cpp

2003-05-19 WinMerge beta 1.9.2.2 released

2003-05-18 Perry
 PATCH: [ 739529 ] project file & directory filters
  WinMerge: Diff.cpp DiffContext.cpp DiffContext.h FileFilterMgr.cpp 
    FileFilterMgr.h MainFrm.cpp MainFrm.h Merge.cpp Merge.dsp Merge.h
	Merge.rc PropFilter.cpp PropFilter.h resource.h

2003-05-16 Kimmo
 PATCH: [ 737709 ] Remove unneeded UpdateAllViews()
  WinMerge: MergeDoc.cpp
 PATCH: [ 738467 ] Enable/disable save items
  WinMerge: MergeEditView.h MergeEditView.cpp

2003-05-14 Kimmo
 PATCH: [ 737692 ] Scrolling optimisation - significant speedup
  editlib: ccrystaltextview.cpp

2003-05-13 Kimmo
 PATCH: [ 736443 ] Prpoper Esc fix
  WinMerge: DirView.h DirView.cpp Merge.cpp MergeEditView.h MergeEditView.cpp

2003-05-09 Kimmo
 PATCH: [ 735127 ] German Translation
  Updated German language file submitted by J�rg Schneider
 PATCH: [ 734787 ] Disable ESC closing window
  NOTE: This is to fix BUG [ 734554 ] ESC closes dialog + document
  WinMerge: Merge.cpp
 BUG: [ 711961 ] shift-tab in editor moved block then deselects
  WinMerge: MergeEditView.cpp

2003-05-07 Kimmo
 PATCH: [ 733383 ] Enable Find- and Replace- dialogs
  WinMerge: Merge.rc resource.h
  editlib: ceditreplacedlg.h ceditreplacedlg.cpp cfindtextdlg.h
   cfindtextdlg.cpp editcmd.h editres.rc
  Removed: editlib/resource.h
  Updated other languages
 PATCH: [ 733926 ] Fix find dialog text selection
  editlib: ccrystaltextview.cpp

2003-05-06 Kimmo
 PATCH: [ 732775 ] Add support for MAC files
  WinMerge: MergeDoc.cpp MergeEditView.h

2003-05-03 Kimmo
 BUG: [ 731486 ] Crash after save
  WinMerge: MergeDoc.cpp
 PATCH: [ 731533 ] Temp path fix
  WinMerge: MergeDoc.h MergeDoc.cpp
  editlib: ccrystaltextbuffer.h ccrystaltextbuffer.cpp

2003-05-02 Kimmo
 BUG: [ 724726 ] Status bar says "Rescanning" the first time
  WinMerge: Merge.rc

2003-05-01 Kimmo
 PATCH: [ 730738 ] Update BCMenu (to v3.033)
  WinMerge: BCMenu.h BCMenu.cpp
   Updates BCMenu to v3.033, http://www.codeproject.com/menu/bcmenu.asp

2003-04-29 Kimmo
 PATCH: [ 728542 ] Automatic rescan enable/disable
  WinMerge: MainFrm.h MainFrm.cpp Merge.rc MergeEditView.h MergeEditView.cpp
   PropGeneral.h PropGeneral.cpp resource.h
   Updated other languages too

2003-04-28 Kimmo
 PATCH: [ 726330 ] Saving before refresh - backout saving - see bug [ 723412 ]
  WinMerge: MergeDoc.cpp MergeEditView.cpp
 PATCH: [ 728228 ] Rename properties to options and rearrange
  Renaming "Properties" to "Options"
  WinMerge: MainFrm.h MainFrm.cpp Merge.rc resource.h
  Updated .rc and resource.h for other languages
 BUG: [ 718133 ] Inconsistent user keyword list for C/C++ syntax hiligth
  editlib: cplusplus.cpp

2003-04-27 Kimmo
 BUG: [ 728173 ] Autoscroll first diff breaks syntax hilight
  WinMerge: MergeDoc.cpp

2003-04-26 Kimmo
 PATCH: [ 726330 ] Saving before refresh
  WinMerge: MergeDoc.h MergeDoc.cpp MergeEditView.cpp
 PATCH: [ 727455 ] Autoscroll first diff for dir compare
  WinMerge: DirView.h MainFrm.cpp

2003-04-25 Kimmo
 PATCH: [ 727104 ] Disable margins for unknown filetypes
  editlib: ccrystaltextview.h ccrystaltextview.cpp

2003-04-23 Kimmo
 PATCH: [ 721939 ] Wider horiz scroll + more space after last char
  editlib: ccrystaltextview.cpp
 PATCH: [ 726390 ] Last diff shown incorrectly sometimes
  WinMerge: MergeDoc.cpp

2003-04-20 Perry
 Changed dynamic array to be CArray based
  editlib: ccrystaltextview.cpp
 PATCH: [ 724309 ] Add WaitStatusCursors
  WinMerge: DirDoc.cpp DirView.cpp MainFrm.cpp Merge.dsp MergeDoc.cpp MergeEditView.cpp 
   WaitStatusCursor.cpp WaitStatusCursor.h
 PATCH: [ 724473 ] Move WaitStatusCursor strings into RC
  WinMerge: DirDoc.cpp DirView.cpp MainFrm.cpp MergeDoc.cpp MergeEditView.cpp resource.h StdAfx.cpp StdAfx.h
 PATCH: [ 724479 ] Fix Refresh crash in empty dir view
  WinMerge: DirDoc.cpp
 bugfix: Add new files that were part of patch#724309
  WinMerge: WaitStatusCursor.cpp WaitStatusCursor.h
 PATCH: [ 724493 ] WaitStatusCursor for MergeDoc rescans
  WinMerge: MergeDoc.cpp

2003-04-19 Perry
 [ 724280 ] Could use wait cursors at OpenSelection calls
  WinMerge: DirView.cpp

2003-04-15 Perry
 PATCH: [ 722083 ] refresh button (menu item, accelerator)
  WinMerge: DirView.cpp DirView.h Merge.rc MergeEditView.cpp MergeEditView.h resource.h res/Toolbar.bmp
 Also updated resource files (resource.h, *.rc, Toolbar.bmp) to all languages

2003-04-14 Kimmo
 PATCH: [ 720613 ] CDirView leaks imagelist
  WinMerge: DirView.cpp
 PATCH: [ 720701 ] Use memory-mapped files when saving files
  WinMerge: files.h files.cpp MergeDoc.h MergeDoc.cpp

2003-04-14 Perry
 PATCH: [ 716475 ] Reduce edtlib.h coupling
 WinMerge: MergeEditView.h StdAfx.h editlib/ccrystaleditview.inl

2003-04-13 Kimmo
 PATCH: [ 719872 ] Use memory-mapped files when loading files
  WinMerge: files.h files.cpp Merge.dsp MergeDoc.h MergeDoc.cpp

2003-04-10 Kimmo
 PATCH: [ 717688 ] CrystalEditor optimisations and memleaks
  fix tab bug.
  editlib: ccrystaltextview.cpp

2003-04-09 Kimmo
 PATCH: [ 717688 ] CrystalEditor optimisations and memleaks
  editlib: ccrystaltextview.cpp

2003-04-08 Kimmo
 PATCH: [ 716744 ] Fix CrystalEditor memory leaks
  editlib: ccrystaltextbuffer.cpp ccrystaltextview.cpp

2003-04-07 Kimmo
 PATCH: [ 716308 ] Disable italic text
  editlib: ccrystaltextview.cpp
 PATCH: [ 715553 ] Add commandline options to not add paths to MRU
  Usage: -ur to ignore rigth, -ul to ignore left and -ub to ignore both
  WinMerge: MainFrm.h MainFrm.cpp Merge.cpp 

2003-04-06 Kimmo
 PATCH: [ 715457 ] LoadFromFile() rewrote
  WinMerge: MergeDoc.h MergeDoc.cpp

2003-04-06 Perry
 PATCH: [ 716088 ] Fix bin same/diff bug in Diff.cpp & analyze.c
  WinMerge: analyze.c Diff.cpp

2003-04-04 Christian
 PATCH: [ 711843 ] Allow backspace at beginning of line
  WinMerge: MainFrm.cpp

2003-04-03 Kimmo
 BUG: [ 714570 ] Sensitive to EOL -setting not saved
  WinMerge: MainFrm.cpp

2003-04-02 Kimmo
 PATCH: [ 713322 ] Save whole buffer not line by line
  WinMerge: MergeDoc.cpp
 PATCH: [ 713416 ] MergeEditView optimisations
  WinMerge: MergeEditView.cpp

2003-03-31 Kimmo
 PATCH: [ 711437 ] Don't copy/cut removed lines
 WinMerge: MergeDoc.h MergeDoc.cpp MergeEditView.h MergeEditView.cpp

2003-03-31 Perry
 PATCH: [ 712429 ] Fix UpdateItemStatus bug 712093
 WinMerge: DirActions.cpp DirView.cpp paths.cpp paths.h

2003-03-30 Perry
 PATCH: [ 712417 ] Fix save of open doc before next merge
 WinMerge: ChildFrm.cpp ChildFrm.h DirDoc.cpp DirDoc.h DirFrame.cpp DirFrame.h
  DirView.cpp DirView.h MainFrm.cpp MainFrm.h MergeDoc.cpp MergeDoc.h

2003-03-29 Perry
 PATCH: [ 710347 ] Fix crash of winmerge c:\ d:\
 WinMerge: DiffContext.h DirDoc.cpp MainFrm.cpp Merge.cpp OpenDlg.cpp OpenDlg.h paths.cpp paths.h
 PATCH: [ 687177 ] Handle more errors during copy/creation
 WinMerge: MainFrm.cpp
 PATCH: [ 689884 ] Revise doc/view code (allow multiple docs)
 WinMerge: ChildFrm.cpp DirActions.cpp DirDoc.cpp DirDoc.h DirView.cpp MainFrm.cpp MainFrm.h
  Merge.cpp MergeDoc.cpp MergeDoc.h MergeEditView.cpp
 NB: This broke two things in MainFrm--search for TODO: 2003-03-29
 BUGFIX: [ 711975 ] CMainFrame::GetAllViews crashes
 WinMerge: MainFrm.cpp
 BUGFIX: [ 711983 ] ClearMergeDoc causing ASSERT
 WinMerge: DirDoc.cpp MergeDoc.cpp

2003-03-26 WinMerge beta 1.9.1.4 released

2003-03-25 Perry
 PATCH: [ 709575 ] Fix sanity check in CChildFrame::UpdateHeaderSizes()
 WinMerge: ChildFrm.cpp
 PATCH: [ 708542 ] Make splash & about show language-independent version
 common: version.cpp version.h
 WinMerge: Merge.cpp Splash.cpp

2003-03-25 Christian
 Replace COPYING with corrected version from http://www.gnu.org/licenses/gpl.txt

2003-03-25 Kimmo
 PATCH: [ 708893 ] Don't ask to save file compare
  WinMerge: MergeDoc.h MergeDoc.cpp
 PATCH: [ 703069 ] Fix undo crash
  editlib: crystaltextbuffer.cpp
 PATCH: [ 709502 ] Fix missing/existing EOL at end of file
  WinMerge: MergeDoc.h MergeDoc.cpp

2003-03-23 Perry
 PATCH: [ 706006 ] Suppress Rescan during Merge
  Merge.cpp Merge.h MergeDoc.cpp MergeDoc.h MergeEditView.cpp

2003-03-23 Kimmo
 PATCH: [ 707753 ] Delayed rescan - fix
  WinMerge: MergeEditView.cpp

2003-03-22 Kimmo
 PATCH: [ 707753 ] Delayed rescan
  WinMerge: MergeEditView.h MergeEditView.cpp

2003-03-21 Kimmo
 PATCH: [ 707152 ] Delay path check in opendialog
  WinMerge: OpenDlg.h OpenDlg.cpp

2003-03-14 Kimmo
 PATCH: [ 703234 ] Use stricmp
  WinMerge: DIFF.C diffmain.c SDIFF.C

2003-03-11 Perry
  Deleted unused res files: bitmap1.bmp, bmp00001.bmp, ScreenShot.gif, WinMerge.gif

2003-03-11 Christian
 PATCH: [ 700872 ] Esc to close crashes WinMerge
  WinMerge: Merge.cpp

2003-03-09 Kimmo
 PATCH: [ 699839 ] Add same binaries to dir compare status
  WinMerge: MainFrm.h MainFrm.cpp
 PATCH: [ 699893 ] Add drag&drop of files/paths to main window
  Note: Press down Ctrl when dropping dirs for recursive compare
  WinMerge: MainFrm.h MainFrm.cpp Merge.cpp paths.h paths.cpp

2003-03-07 WinMerge beta 1.9.1.3 released

2003-03-07 Perry
 PATCH: [ 699580 ] Fix paths.cpp
  WinMerge: paths.cpp

2003-03-07 Kimmo
 PATCH: [ 698754 ] Update filetimes after save
  WinMerge: DiffContext.h DiffContext.cpp DirDoc.h DirDoc.cpp MergeDoc.cpp

2003-03-06 Christian
 Update Danish language file with new strings in MergeDanish.rc
 Removed duplicate dialog in French language file in MergeFrench.rc
 Added Czech to BuildAll.bat
 PATCH: [ 698346 ] Use full path for directory context menu option
  WinMerge: PropRegistry.cpp

2003-03-06 Kimmo
 PATCH: [ 696695 ] DirDoc Redisplay optimisation
  WinMerge: DirDoc.h DirDoc.cpp

2003-03-05 Perry
 Added new subdir Languages/Danish/res
  PATCH: [ 695857 ] Fix directory handling, & UNC paths, & shortnames
 Fix Danish language files: Update resource.h and update language dialogbox to use IDC_LANGUAGE_LIST.

2003-03-04
 BUG: [ 684622 ] Escape key closes whole program
  Nikki Locke's patch slightly modified
  WinMerge: Merge.cpp
 PATCH: [ 696588 ] Multimonitor + virtual screens support
  WinMerge: MainFrm.cpp Merge.dsp multimon.h
 BUG: [ 697320 ] Saving enabled when comparing directories
  WinMerge: DirView.h DirView.cpp

2003-02-28 Kimmo
 BUG: [ 693932 ] Trailing \ on path crashes WinMerge
  WinMerge: OpenDlg.h OpenDlg.cpp

2003-02-27 Kimmo
 BUG: [ 693820 ] Typed path failed to compare
  WinMerge: OpenDlg.cpp
 BUG: [ 693004 ] Keyboard does not work for accessing menus
  Add missing WM_MENUCHAR handler from www.codeguru.com
  WinMerge: MainFrm.h MainFrm.cpp

2003-02-26 Kimmo
 BUG: [ 683753 ] Rescan is not prompting to save dirty
      current file
  WinMerge:
   MainFrm.cpp

2003-02-26 Christian
 [ 70970 ] Translate Merge.rc into danish
    BuildAll.bat and added directory Languages/Danish

2003-02-26 Kimmo
 [ 692922 ] Don't try to compare file & directory
  WinMerge:
   OpenDlg.cpp
 [ 686699 ] Check & return file saving success
  Fix2: Do not return save success of "Save As"
  WinMerge:
   MergeDoc.cpp

2003-02-24 Kimmo
 [ 687574 ] Navigate in directory view (fix bug in patch)
  WinMerge:
   DirView.cpp
 [ 686699 ] Check & return file saving success - fix
  WinMerge:  
   MergeDoc.h Mergedoc.cpp

2003-02-22 Perry
 [ 650356 ] suggest commenting out several functions
 [ 684416 ] is_all_whitespace is bad
  Removed bad unused functions in common/coretools.cpp (& .h)
  (Did not remove all unused functions, only ones with bugs)

2003-02-21 Perry
 Sync'ed all language resources to English version.
 [ 684300 ] Stringified Diffitems
   + combined & modularized code in DirDoc.cpp
  WinMerge:
   DiffContext.cpp DiffContext.h DirDoc.cpp DirView.cpp DirView.h

2003-02-20 Perry
 [ 689876 ] Patch to fix bug # 689869 (ASSERT on Rescan)
  WinMerge:
   MainFrm.cpp & new resource IDS_ERROR_FILE_NOT_FOUND
 [ 687574 ] Navigate in directory view
  WinMerge:
   DirView.cpp DirView.h
 [ 690176 ] Cmdline param for ESC closing
   (and updated all obsolete French & German accelerators as well)
  WinMerge:
   Merge.cpp Merge.h Merge.rc
  Languages:
   Czech\MergeCzech.rc Dutch\MergeDutch.rc French\MergeFrench.rc
   German\MergeGerman.rc Italian\MergeItalian.rc Spanish\MergeSpanish.rc
  [ 689896 ] Update to sync Czech resource
    Czech\MergeCzech.rc

2003-02-18 Christian
  Removed annoying SourceSafe path from Merge.dsp

2003-02-18 Kimmo
  [ 684737 ] Fix Language Box to handle dblclick
  Languages:
   LanguageSelect.h LanguageSelect.cpp
  WinMerge:
   Merge.rc resource.h
  [ 687039 ] View whitespaces on editor
  WinMerge:
   MainFrm.h MainFrm.cpp Merge.rc resource.h
  WinMerge/editlib:
   ccrystaltextview.cpp

2003-02-15 Kimmo
  [ 686699 ] Check & return file saving success
  Merge.rc MergeDoc.h Mergedoc.cpp resource.h

2003-02-14 Kimmo
 [ 685491 ] Saving fixes: SaveToFile() rewrite part
  + Missing NULL check for mf->m_pDirDoc in CMergeDoc
  MergeDoc.h MergeDoc.cpp

2003-02-13  Perry
 [ 685816 ] Recommend deleting unlocalized translations
 [ 684290 ] improved bin test:
  Diff.cpp readme.txt
 [ 686090 ] Show/hide binary files
 WinMerge:
  DirDoc.cpp MainFrm.cpp MainFrm.h Merge.rc readme.txt
  resource.h
 Languages:
  Spanish/MergeSpanish.rc Spanish/resource.h

2003-02-12  Perry
 Languages:
  Dutch/MergeDutch.rc
 Some updates to align Dutch rc towards English rc.
 [ 685637 ] CMainFrame::SyncFiles GPF bug

2003-02-12  Christian
 [ 630286 ] WinMerge crashes/fails in WinCVS
 [ 683752 ] Add option to be listed on Directory context menu

2003-02-11  Perry
 Fixing recent SplitFilename & directory concat problems.
 [ 683924 ] Same binaries for directory compare.

2003-02-10  Perry
 Kimmo's fix for crash due to NULL doc pointer in MergeDoc::SaveHelper.
 [ 470013 ] Spanish Orthography and Semantics

2003-02-09  Perry
 [ 683120 ] splitfilename changed to CStrings
 Move new SplitFilename from Chris Mumford into coretools, remove old
 versions, add test chassis, and fix some minor cases.

2003-02-08  Perry
 DirDoc.*: Changed from strftime to COleDateTime::Format (respects user prefs).
 First half of [ 679692 ] Update after editing (with code combination).
 [ 682505 ] API changes for CLog, CDirView and CDirDoc
 2nd half of [ 679692 ] Update after editing
 
2003-02-07  WinMerge beta 1.9.1.1 released

2003-02-07  Christian
 Added 24 missing languages
 Fixed duplicated strings in Italian rc.

2003-02-05  Perry
 Updated Brazilian, Italian, Spanish resources to sync with English/main resources.

2003-01-28  Christian
 Committed [ 424579 ] Escape to exit

2003-01-27  Christian
 [ 668654 ] Tabtype selection feature
 [ 672554 ] Update DirView after individual merge
 Fixed [ 225571 ] After individual merge, dirView not updated
 [ 672879 ] Propose clarifying change in ccrystaleditview.cpp
 [ 672885 ] Fix for ASSERT from CChildFrame::UpdateHeaderSizes
 [ 673063 ] Modification check before close
 [ 675526 ] Waitcursors and merging all

2003-01-21  Christian
 [ 671037 ] Changes for MSVC.NET
 [ 671810 ] Tempfiles to temp dir

2003-01-20  Christian
 [ 671034 ] operator precedence bug in SplitterWndEx.cpp

2003-01-15  Christian
 [ 650384 ] Fix for false equals for binary files
 [ 651689 ] Add OpenLeft, OpenLeftWith, ... context
 [ 650293 ] Implement context menus with multiselect

2003-01-14  Christian
 [ 665993 ] Little optimisation

2002-12-17  WinMerge beta 1.7.2.11 released

2002-11-26  Perry
 Implemented recursive directory delete
 Fix bug in EOL handling in crystaltextbuffer, and add optional EOL sensitivity.
 Added optional EOL insensitivity to gnu diff IO.c.
 Added status bar progress (file count) indicator during scanning
 Fixed tooltips of toolbar buttons to list correct hotkeys
 Split "Show Unique" option into "Show Left Unique" and "Show Right Unique"

2002-12-02  Perry
 Added left files, left dirs, right files, right dirs to status bar feedback

2002-11-18  WinMerge beta 1.7.2.1 released
