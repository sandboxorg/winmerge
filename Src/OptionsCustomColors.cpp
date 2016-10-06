#include "OptionsCustomColors.h"
#include "OptionsMgr.h"

namespace Options { namespace CustomColors
{

/** @brief Setting name for user-defined custom colors. */
const TCHAR Section[] = _T("Custom Colors");

void Load(COptionsMgr *pOptionsMgr, COLORREF * colors)
{
	for (int i = 0; i < 16; i++)
	{
		String valuename = string_format(_T("%s/%d"), Section, i);
		pOptionsMgr->InitOption(valuename, RGB(255, 255, 255), true);
		colors[i] = pOptionsMgr->GetInt(valuename);
	}
}

void Save(COptionsMgr *pOptionsMgr, const COLORREF * colors)
{
	for (int i = 0; i < 16; i++)
	{
		String valuename = string_format(_T("%s/%d"), Section, i);
		if (colors[i] == RGB(255, 255, 255))
			pOptionsMgr->RemoveOption(valuename);
		else 
			pOptionsMgr->SaveOption(valuename, static_cast<int>(colors[i]));
	}
}

}}