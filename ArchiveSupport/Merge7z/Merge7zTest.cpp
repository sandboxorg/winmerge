#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "Merge7z.h"

int main()
{
	TCHAR tmppath[260];
	TCHAR *zip = _T("D:/dev/WinMerge/TEST/ArchiveTest/7z458.tbz2");
	Merge7z *pMerge7z;
#ifdef _UNICODE
	HMODULE hLibrary = LoadLibrary(_T("Merge7zXXXU.dll"));
#else
	HMODULE hLibrary = LoadLibrary(_T("Merge7zXXX.dll"));
#endif
	pMerge7z = (Merge7z *)GetProcAddress(hLibrary, "Merge7z");
	pMerge7z->Initialize();
	Merge7z::Format *pFormat = pMerge7z->GuessFormat(zip);
	GetTempPath(sizeof(tmppath)/sizeof(tmppath[0]), tmppath);
	HRESULT hr = pFormat->DeCompressArchive(NULL, zip, tmppath);

	// test case
	// zip�t�@�C�������݂��Ȃ�
	zip = _T("D:/dev/WinMerge/TEST/ArchiveTest/DesktopXXXXX.zip");
	pFormat = pMerge7z->GuessFormat(zip);
	hr =  pFormat->DeCompressArchive(NULL, zip, tmppath);

	// 7z.exe�����݂��Ȃ�
	// �W�J��̃t�H���_�����Ȃ�
	// �p�X���[�h�t���Ő���p�X���[�h
	zip = _T("D:/dev/WinMerge/TEST/ArchiveTest/passwordtest.zip");
	pFormat = pMerge7z->GuessFormat(zip);
	hr =  pFormat->DeCompressArchive(NULL, zip, tmppath);

	// �p�X���[�h�t���ŕs���p�X���[�h
	// RPM�t�@�C���̓W�J
	// ��ꂽzip�t�@�C��
	// �W�J���ɃL�����Z��
	// Unicode/ANSI�r���h
	return 0;
}