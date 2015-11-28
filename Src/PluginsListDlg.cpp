/**
 * @file  PluginsListDlg.cpp
 *
 * @brief Implementation file for PluginsList dialog
 */

#include "stdafx.h"
#include "PluginsListDlg.h"
#include "UnicodeString.h"
#include "Merge.h"
#include "Plugins.h"
#include "OptionsDef.h"
#include "OptionsMgr.h"

IMPLEMENT_DYNAMIC(PluginsListDlg, CDialog)

BEGIN_MESSAGE_MAP(PluginsListDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_PLUGIN_SETTINGS, OnBnClickedPluginSettings)
	ON_NOTIFY(NM_DBLCLK, IDC_PLUGINSLIST_LIST, OnNMDblclkList)
END_MESSAGE_MAP()

/**
 * @brief Constructor.
 */
PluginsListDlg::PluginsListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PluginsListDlg::IDD, pParent)
{
}

/**
 * @brief Destructor.
 */
PluginsListDlg::~PluginsListDlg()
{
}

void PluginsListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGINSLIST_LIST, m_list);
}

/**
 * @brief Initialize the dialog.
 */
BOOL PluginsListDlg::OnInitDialog()
{
	theApp.TranslateDialog(m_hWnd);
	CDialog::OnInitDialog();
	
	InitList();
	AddPlugins();
	m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	bool pluginsEnabled = GetOptionsMgr()->GetBool(OPT_PLUGINS_ENABLED);
	if (pluginsEnabled)
	{
		CButton *btn = (CButton *)GetDlgItem(IDC_PLUGINS_ENABLE);
		btn->SetCheck(BST_CHECKED);
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
 * @brief Set-up the list control.
 */
void PluginsListDlg::InitList()
{
	// Show selection across entire row.
	// Also enable infotips.
	m_list.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	String title = _("Name");
	m_list.InsertColumn(0, title.c_str(), LVCFMT_LEFT, 200);
	title = _("Type");
	m_list.InsertColumn(1, title.c_str(), LVCFMT_LEFT, 100);
	title = _("Description");
	m_list.InsertColumn(2, title.c_str(), LVCFMT_LEFT, 300);
}

/**
 * @brief Add found plugins to the list.
 */
void PluginsListDlg::AddPlugins()
{
	String type = _("Unpacker");
	AddPluginsToList(L"FILE_PACK_UNPACK", type);
	AddPluginsToList(L"BUFFER_PACK_UNPACK", type);
	AddPluginsToList(L"FILE_FOLDER_PACK_UNPACK", type);
	type = _("Prediffer");
	AddPluginsToList(L"FILE_PREDIFF", type);
	AddPluginsToList(L"BUFFER_PREDIFF", type);
	type = _("Editor script");
	AddPluginsToList(L"EDITOR_SCRIPT", type);
}

/**
 * @brief Add plugins of given event type to the list.
 * @param [in] pluginEvent Event type for plugins to add.
 * @param [in] pluginType String to use as type in the list.
 */
void PluginsListDlg::AddPluginsToList(const wchar_t *pluginEvent, const String& pluginType)
{
	PluginArray * piPluginArray = 
		CAllThreadsScripts::GetActiveSet()->GetAvailableScripts(pluginEvent);

	for (int iPlugin = 0 ; iPlugin < piPluginArray->size() ; iPlugin++)
	{
		const PluginInfoPtr& plugin = piPluginArray->at(iPlugin);
		int ind = m_list.InsertItem(m_list.GetItemCount(), plugin->m_name.c_str());
		m_list.SetItemText(ind, 1, pluginType.c_str());
		m_list.SetItemText(ind, 2, plugin->m_description.c_str());
		m_list.SetCheck(ind, !plugin->m_disabled);
	}
}

/**
 * @brief Save plugins enabled setting when closing the dialog.
 */
void PluginsListDlg::OnBnClickedOk()
{
	CButton *btn = (CButton *)GetDlgItem(IDC_PLUGINS_ENABLE);
	GetOptionsMgr()->SaveOption(OPT_PLUGINS_ENABLED, (btn->GetCheck() == BST_CHECKED));

	for (int i = 0; i < m_list.GetItemCount(); ++i)
	{
		PluginInfo * plugin = CAllThreadsScripts::GetActiveSet()->GetPluginByName(NULL, String(m_list.GetItemText(i, 0)));
		plugin->m_disabled = !m_list.GetCheck(i);
	}
	CAllThreadsScripts::GetActiveSet()->SaveSettings();
	OnOK();
}

void PluginsListDlg::OnBnClickedPluginSettings()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos)
	{
		String pluginName = static_cast<const TCHAR *>(m_list.GetItemText(m_list.GetNextSelectedItem(pos), 0));
		for (int i = 0; TransformationCategories[i]; ++i)
		{
			PluginInfo * plugin = CAllThreadsScripts::GetActiveSet()->GetPluginByName(TransformationCategories[i], pluginName);
			if (plugin)
			{
				EnableWindow(false);
				InvokeShowSettingsDialog(plugin->m_lpDispatch);
				EnableWindow(true);
				SetForegroundWindow();
				break;
			}
		}
	}
}

void PluginsListDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnBnClickedPluginSettings();
}
