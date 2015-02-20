/** 
 * @file  ExConverter.cpp
 *
 * @brief Codepage converter using external libraries.
 */

#include "ExConverter.h"
#include <windows.h>
#include <mlang.h>
#include <memory>
#include "unicoder.h"
#include "codepage.h"

#if !defined(__IMultiLanguage2_INTERFACE_DEFINED__) && !defined(__GNUC__)
#error "IMultiLanguage2 is not defined in mlang.h. Please install latest Platform SDK."
#endif

#if defined(__GNUC__)
const IID IID_IMultiLanguage2 = {0xDCCFC164, 0x2B38, 0x11d2, {0xB7, 0xEC, 0x00, 0xC0, 0x4F, 0x8F, 0x5D, 0x9A}};
#endif 

extern "C" typedef HRESULT (__stdcall *DllGetClassObjectFunc)(const CLSID &, const IID &, void **); 

class CExconverterMLang: public IExconverter {
private:
	IMultiLanguage2 *m_pmlang;
	HMODULE m_hLibMLang;
	DWORD m_mlangcookie;
#ifdef USEUNIVERSALCHARDET
	HMODULE m_hLibCharGuess;
	int (*m_pfnCharGuessInit)(void);
	int (*m_pfnCharGuessDone)(void);
	const char* (*m_pfnGuessChardet)(const char *str);
#endif

public:
	CExconverterMLang()
	: m_pmlang(NULL)
	, m_hLibMLang(NULL)
	, m_mlangcookie(0)
	{
	}

	~CExconverterMLang()
	{
		if (m_pmlang)
			m_pmlang->Release();
		if (m_hLibMLang)
			FreeLibrary(m_hLibMLang);
	}

	bool initialize()
	{
		DllGetClassObjectFunc pfnDllGetClassObject = NULL;
		IClassFactory *pClassFactory = NULL;

		m_hLibMLang = LoadLibrary(_T("mlang.dll"));
		if (m_hLibMLang)
		{
			pfnDllGetClassObject = (DllGetClassObjectFunc)GetProcAddress(m_hLibMLang, "DllGetClassObject");
			if (pfnDllGetClassObject)
			{
				HRESULT hr = pfnDllGetClassObject(CLSID_CMultiLanguage, IID_IClassFactory, (void**)&pClassFactory);
				if (pClassFactory)
				{
					hr = pClassFactory->CreateInstance(NULL, IID_IMultiLanguage2, (void**)&m_pmlang);
					if (SUCCEEDED(hr))
					{
						pClassFactory->Release();
						return true;
					}
				}
			}
		}
		if (pClassFactory)
			pClassFactory->Release();
		if (m_hLibMLang)
		{
			FreeLibrary(m_hLibMLang);
			m_hLibMLang = NULL;
		}
		return false;
	}

	bool convert(int srcCodepage, int dstCodepage, const unsigned char * src, size_t * srcbytes, unsigned char * dest, size_t * destbytes)
	{
		bool bsucceeded;
#ifdef POCO_ARCH_BIG_ENDIAN
		if (srcCodepage == CP_UCS2BE)
#else
		if (srcCodepage == CP_UCS2LE)
#endif
		{
			size_t srcwchars = *srcbytes / sizeof(wchar_t);
			bsucceeded = convertFromUnicode(dstCodepage, (const wchar_t *)src, &srcwchars, (char *)dest, destbytes);
			*srcbytes = srcwchars * sizeof(wchar_t);
		}
		else
		{
			size_t wsize = *srcbytes * 2 + 6;
			std::unique_ptr<wchar_t[]> pbuf(new wchar_t[wsize]);
			bsucceeded = convertToUnicode(srcCodepage, (const char *)src, srcbytes, pbuf.get(), &wsize);
			if (!bsucceeded)
			{
				*destbytes = 0;
				return false;
			}
			bsucceeded = convertFromUnicode(dstCodepage, pbuf.get(), &wsize, (char *)dest, destbytes);
		}
		return bsucceeded;
	}

	bool convertFromUnicode(int dstCodepage, const wchar_t * src, size_t * srcchars, char * dest, size_t *destbytes)
	{
		UINT uisrcchars = static_cast<UINT>(*srcchars), uidestbytes = static_cast<UINT>(*destbytes);
		HRESULT hr = m_pmlang->ConvertStringFromUnicode(&m_mlangcookie, dstCodepage, (wchar_t *)src, &uisrcchars, (char *)dest, &uidestbytes);
		*srcchars = uisrcchars;
		*destbytes = uidestbytes;
		return SUCCEEDED(hr) ? true : false;
	}

	bool convertToUnicode(int srcCodepage, const char * src, size_t * srcbytes, wchar_t * dest, size_t *destchars)
	{
		UINT uisrcbytes = static_cast<UINT>(*srcbytes), uidestchars = static_cast<UINT>(*destchars);
		HRESULT hr = m_pmlang->ConvertStringToUnicode(&m_mlangcookie, srcCodepage, (char *)src, &uisrcbytes, dest, &uidestchars);
		*srcbytes = uisrcbytes;
		*destchars = uidestchars;
		return SUCCEEDED(hr) ? true : false;
	}

	void clearCookie()
	{
		m_mlangcookie = 0;
	}

	int detectInputCodepage(int autodetectType, int defcodepage, const char *data, size_t size)
	{
		int codepage;
		IMLangConvertCharset *pcc;
		UINT dstsize;
		UINT srcsize;
		HRESULT hr;

		hr = m_pmlang->CreateConvertCharset(autodetectType, CP_UCS2LE, MLCONVCHARF_AUTODETECT, &pcc);
		if (FAILED(hr))
			return defcodepage;
		srcsize = static_cast<UINT>(size);
		dstsize = static_cast<UINT>(size * sizeof(wchar_t));
		std::unique_ptr<unsigned char[]> pdst(new unsigned char[size * sizeof(wchar_t)]);
		SetLastError(0);
		hr = pcc->DoConversion((unsigned char *)data, &srcsize, pdst.get(), &dstsize);
		pcc->GetSourceCodePage((unsigned *)&codepage);
		if (FAILED(hr) || GetLastError() == ERROR_NO_UNICODE_TRANSLATION || codepage == autodetectType)
		{
			int codepagestotry[3] = {0};
			if (codepage == autodetectType)
			{
				if (size < 2 || (data[0] != 0 && data[1] != 0))
				{
					codepagestotry[0] = defcodepage;
					codepagestotry[1] = CP_UTF8;
				}
			}
			else
			{
				if (size < 2 || (data[0] != 0 && data[1] != 0))
					codepagestotry[0] = CP_UTF8;
			}
			codepage = defcodepage;
			int i;
			for (i = 0; i < sizeof(codepagestotry)/sizeof(codepagestotry[0]) - 1; i++)
			{
				if (codepagestotry[i] == 0) break;
				pcc->Initialize(codepagestotry[i], CP_UCS2LE, 0);
				srcsize = static_cast<UINT>(size);
				dstsize = static_cast<UINT>(size * sizeof(wchar_t));
				SetLastError(0);
				hr = pcc->DoConversion((unsigned char *)data, &srcsize, pdst.get(), &dstsize);
				if (FAILED(hr) || GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
					continue;
				codepage = codepagestotry[i];
				break;
			}
			if (codepagestotry[i] == 0 && (size % 2) == 0)
			{
				// UCS-2
				int lezerocount = 0;
				int lecrorlf = 0;
				int bezerocount = 0;
				int becrorlf = 0;
				for (i = 0; i < size; i += 2)
				{
					if (data[i] == 0)
					{
						bezerocount++;
						if (data[i + 1] == 0x0a || data[i + 1] == 0x0d)
							lecrorlf++;
					}
					else if (data[i + 1] == 0)
					{
						lezerocount++;
						if (data[i] == 0x0a || data[i] == 0x0d)
							lecrorlf++;
					}
				}
				if (lezerocount > 0 || bezerocount > 0)
				{
					if ((lecrorlf == 0 && size < 512 || (lecrorlf > 0 && (size / lecrorlf > 1024))) && lezerocount > bezerocount)
						codepage = CP_UCS2LE;
					else if ((becrorlf == 0 && size < 512 || (becrorlf > 0 && (size / becrorlf > 1024))) && lezerocount < bezerocount)
						codepage = CP_UCS2BE;
				}
			}
		}
		if (codepage == 20127)
			return defcodepage;
		return codepage;
	}

	int enumCodePages(CodePageInfo *cpinfo, int count)
	{
		IEnumCodePage *pEnumCodePage = NULL;
		PMIMECPINFO pcpInfo;
		ULONG ccpInfo;
		HRESULT hr = m_pmlang->EnumCodePages(MIMECONTF_SAVABLE_BROWSER | MIMECONTF_VALID | MIMECONTF_VALID_NLS, 0, &pEnumCodePage);
		if (FAILED(hr))
			return 0;
		pcpInfo = (PMIMECPINFO)CoTaskMemAlloc(sizeof(MIMECPINFO) * count);
		pEnumCodePage->Next(count, pcpInfo, &ccpInfo);
		CoTaskMemRealloc((void*)pcpInfo, sizeof(MIMECPINFO)*ccpInfo);

		for (int i = 0; i < (int)ccpInfo; i++)
		{
			cpinfo[i].codepage = pcpInfo[i].uiCodePage;
			lstrcpyW(cpinfo[i].desc, pcpInfo[i].wszDescription);
		}

		return ccpInfo;
	}

	bool getCodepageFromCharsetName(const String& sCharsetName, int& codepage)
	{
		MIMECSETINFO charsetInfo;
		BSTR bstrCharsetName = SysAllocString(ucr::toUTF16(sCharsetName).c_str());
		HRESULT hr = m_pmlang->GetCharsetInfo(bstrCharsetName, &charsetInfo);
		SysFreeString(bstrCharsetName);
		if (FAILED(hr))
			return false;
		codepage = charsetInfo.uiInternetEncoding;
		return true;
	}

	bool getCodepageDescription(int codepage, String& sDescription)
	{
		wchar_t szDescription[256];
		HRESULT hr = m_pmlang->GetCodePageDescription(codepage, GetSystemDefaultLangID(), szDescription, sizeof(szDescription)/sizeof(wchar_t));
		if (FAILED(hr))
			return false;

		sDescription = ucr::toTString(szDescription);
		return true;
	}

	bool isValidCodepage(int codepage)
	{
		String sDesc;
		return getCodepageDescription(codepage, sDesc);
	}

	bool getCodePageInfo(int codepage, CodePageInfo *pCodePageInfo)
	{
		MIMECPINFO mcpi = {0};
		HRESULT hr = m_pmlang->GetCodePageInfo(codepage, GetSystemDefaultLangID(), &mcpi);
		if (FAILED(hr))
			return false;
		wcscpy(pCodePageInfo->fixedWidthFont, mcpi.wszFixedWidthFont);
		pCodePageInfo->bGDICharset = mcpi.bGDICharset;
		return true;
	}

};

#ifdef _MSC_VER
__declspec(thread) static IExconverter *m_pexconv = NULL;
#else
static __thread IExconverter *m_pexconv = NULL;
#endif

IExconverter *Exconverter::getInstance()
{
	if (m_pexconv)
	{
		m_pexconv->clearCookie();
		return m_pexconv;
	}
	CExconverterMLang *pexconv = new CExconverterMLang();
	if (!pexconv->initialize())
	{
		delete pexconv;
		return NULL;
	}
	m_pexconv = pexconv;
	return m_pexconv;
}

