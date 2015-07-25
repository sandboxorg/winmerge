/**
 *  @file DirScan.h
 *
 *  @brief Declaration of DirScan module (see DirScan function)
 */ 
#pragma once

#include "UnicodeString.h"
#include <cstdint>

class CDiffContext;
class DiffItemList;
class PathContext;
class IAbortable;
struct DIFFITEM;
struct DiffFuncStruct;

int DirScan_GetItems(const PathContext &paths, const String subdir[], DiffFuncStruct *myStruct,
		bool casesensitive, int depth, DIFFITEM *parent, bool bUniques);
int DirScan_UpdateMarkedItems(DiffFuncStruct *myStruct, uintptr_t parentdiffpos);

int DirScan_CompareItems(DiffFuncStruct *, uintptr_t parentdiffpos);
int DirScan_CompareRequestedItems(DiffFuncStruct *, uintptr_t parentdiffpos);
