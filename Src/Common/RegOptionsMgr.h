/** 
 * @file OptionsMgr.h
 *
 * @brief Declaration for Registry options class.
 *
 */
// ID line follows -- this is updated by SVN
// $Id$


#ifndef _REG_OPTIONSMGR_H_
#define _REG_OPTIONSMGR_H_

#include <Windows.h>
#include "OptionsMgr.h"

class COptionsMgr;

/**
 * @brief Registry-based implementation of OptionsMgr interface (q.v.).
 */
class CRegOptionsMgr: public COptionsMgr
{
public:
	CRegOptionsMgr() : m_serializing(true) { }

	int LoadOption(const String& name);
	int SetRegRootKey(const String& path);

	virtual int InitOption(const String& name, varprop::VariantValue defaultValue);
	virtual int InitOption(const String& name, const String& defaultValue);
	virtual int InitOption(const String& name, const TCHAR *defaultValue);
	virtual int InitOption(const String& name, int defaultValue, bool serializable=true);
	virtual int InitOption(const String& name, bool defaultValue);

	virtual int SaveOption(const String& name);
	virtual int SaveOption(const String& name, varprop::VariantValue value);
	virtual int SaveOption(const String& name, const String& value);
	virtual int SaveOption(const String& name, const TCHAR *value);
	virtual int SaveOption(const String& name, int value);
	virtual int SaveOption(const String& name, bool value);

	virtual int RemoveOption(const String& name);

	virtual void SetSerializing(bool serializing=true) { m_serializing = serializing; }

	virtual int ExportOptions(const String& filename) const;
	virtual int ImportOptions(const String& filename);

protected:
	void SplitName(const String &strName, String &strPath, String &strValue) const;
	int LoadValueFromReg(HKEY hKey, const String& strName,
		varprop::VariantValue &value);
	int SaveValueToReg(HKEY hKey, const String& strValueName,
		varprop::VariantValue value);

private:
	String m_registryRoot; /**< Registry path where to store options. */
	bool m_serializing;
};

#endif // _REG_OPTIONSMGR_H_
