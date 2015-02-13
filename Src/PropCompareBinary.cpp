/** 
 * @file  PropCompareBinary.cpp
 *
 * @brief Implementation of PropCompareBinary propertysheet
 */
// ID line follows -- this is updated by SVN
// $Id$

#include "stdafx.h"
#include "PropCompareBinary.h"
#include "Merge.h"
#include "OptionsDef.h"
#include "OptionsMgr.h"
#include "OptionsPanel.h"
#include "heksedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class Heksedit
{
public:
	Heksedit(CWnd *pwndParent)
	{
		pv = LoadLibrary(_T("Frhed\\hekseditU.dll"));
		if (!pv)
		{
			LangMessageBox(IDS_FRHED_NOTINSTALLED, MB_OK);
			return;
		}
		wnd.Create(_T("heksedit"), NULL, 0, CRect(), pwndParent, 1);
		get_interface()->read_ini_data();
		get_interface()->get_settings()->bSaveIni = TRUE;
	}

	IHexEditorWindow *get_interface()
	{
		return reinterpret_cast<IHexEditorWindow *>(::GetWindowLongPtr(wnd.m_hWnd, GWLP_USERDATA));
	}

private:
	HMODULE pv;
	CWnd wnd;
};

/** 
 * @brief Constructor.
 * @param [in] optionsMgr Pointer to COptionsMgr.
 */
PropCompareBinary::PropCompareBinary(COptionsMgr *optionsMgr) 
 : OptionsPanel(optionsMgr, PropCompareBinary::IDD)
{
}

void PropCompareBinary::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropCompareBinary)
	DDX_Text(pDX, IDC_COMPAREBINARY_PATTERNS, m_sFilePatterns);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropCompareBinary, CPropertyPage)
	//{{AFX_MSG_MAP(PropCompareBinary)
	ON_BN_CLICKED(IDC_COMPAREBINARY_VIEWSETTINGS, OnViewSettings)
	ON_BN_CLICKED(IDC_COMPAREBINARY_BINARYMODE, OnBinaryMode)
	ON_BN_CLICKED(IDC_COMPAREBINARY_CHARACTERSET, OnCharacterSet)
	ON_BN_CLICKED(IDC_COMPAREBINARY_DEFAULTS, OnDefaults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/** 
 * @brief Reads options values from storage to UI.
 * Property sheet calls this before displaying GUI to load values
 * into members.
 */
void PropCompareBinary::ReadOptions()
{
	m_sFilePatterns = GetOptionsMgr()->GetString(OPT_CMP_BIN_FILEPATTERNS).c_str();
}

/** 
 * @brief Writes options values from UI to storage.
 * Property sheet calls this after dialog is closed with OK button to
 * store values in member variables.
 */
void PropCompareBinary::WriteOptions()
{
	GetOptionsMgr()->SaveOption(OPT_CMP_BIN_FILEPATTERNS, m_sFilePatterns);
}

/** 
 * @brief Called before propertysheet is drawn.
 */
BOOL PropCompareBinary::OnInitDialog() 
{
	theApp.TranslateDialog(m_hWnd);
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/** 
 * @brief Show Frhed view settings dialog
 */
void PropCompareBinary::OnViewSettings()
{
	Heksedit(this).get_interface()->CMD_view_settings();
}

/** 
 * @brief Show Frhed binary mode dialog
 */
void PropCompareBinary::OnBinaryMode()
{
	Heksedit(this).get_interface()->CMD_binarymode();
}

/** 
 * @brief Show Frhed characterset dialog
 */
void PropCompareBinary::OnCharacterSet()
{
	Heksedit(this).get_interface()->CMD_character_set();
}

/** 
 * @brief Sets options to defaults
 */
void PropCompareBinary::OnDefaults()
{
	String tmp;
	GetOptionsMgr()->GetDefault(OPT_CMP_BIN_FILEPATTERNS, tmp);
	m_sFilePatterns = tmp.c_str();
	UpdateData(FALSE);
}
