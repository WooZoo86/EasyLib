#include "stdafx.h"
#include "defines.h"
#include "Interface.h"
#include <regex>
#include <io.h>
#include <iostream>



bool __stdcall ProcessDir(const TCHAR* dir, const TCHAR* filter, const TCHAR* regExpress, std::function<bool(const TCHAR*)> callback)
{
	if (!callback)
	{
		MYCOUT << _T("ProcessDir callback is null") << std::endl;
		return false;
	}

	TCHAR szRoot[_MAX_PATH + 1] = { 0 };
	TCHAR szFind[_MAX_PATH + 1] = { 0 };
	TCHAR szPath[_MAX_PATH + 1] = { 0 };

	_tcscpy_s(szRoot, _MAX_PATH, dir);
	if (dir[_tcsclen(dir)] != _T('\\'))
		_tcscat_s(szRoot, _MAX_PATH, _T("\\"));
	_tcscpy_s(szFind, _MAX_PATH, szRoot);

	bool bUseRegex = !filter && regExpress;
	if (bUseRegex)
	{
		_tcscat_s(szFind, _MAX_PATH, _T("*.*"));
	}
	else
	{
		_tcscat_s(szFind, _MAX_PATH, filter);
	}

	_tfinddata_t file = { 0 };
	long hFind = _tfindfirst(szFind, &file);
	if (-1 == hFind)
	{
		MYCOUT << _T("ProcessDir _findfirst() failed") << szFind << std::endl;
		return false;
	}

	do
	{
		if (!_tccmp(file.name, _T(".")) || !_tccmp(file.name, _T("..")))
			continue;
		
		memset(szPath, 0, _MAX_PATH + 1);
		_tcscpy_s(szPath, _MAX_PATH, szRoot);
		_tcscat_s(szPath, _MAX_PATH, file.name);

		if (file.attrib & _A_SUBDIR)
		{
			_tcscat_s(szPath, _MAX_PATH, _T("\\"));
			ProcessDir(szPath, filter, regExpress, callback);
		}
		else
		{
			if (bUseRegex)
			{
				try
				{
					tregex regExpress((tstring)regExpress, std::regex_constants::icase);
					tmatch ms;
					tstring txt = szPath;
					if (std::regex_match(txt, ms, regExpress))
					{
						if (ms.size() > 0)
						{
							if (!callback(szPath))
							{
								MYCOUT << _T("ProcessDir callback return false, path: ") << szPath << std::endl;
							}
						}
					}
				}
				catch (const std::regex_error& e)
				{
					MYCOUT << _T("ProcessDir regex_error: ") << e.code() << _T(",") << e.what() << std::endl;
					continue;
				}
			}
			else
			{
				if (!callback(szPath))
				{
					MYCOUT << _T("ProcessDir callback return false, path: ") << szPath << std::endl;
				}
			}
		}
	} while (!_tfindnext(hFind, &file));

	_findclose(hFind);

	return true;
}

ELIB_API bool __stdcall ProcessDirByRegex(const TCHAR* dir, const TCHAR* regExpress, std::function<bool(const TCHAR*)> callback)
{
	return ProcessDir(dir, nullptr, regExpress, callback);
}

ELIB_API bool __stdcall ProcessDir(const TCHAR* dir, const TCHAR* filter, std::function<bool(const TCHAR*)> callback)
{
	return ProcessDir(dir, filter, nullptr, callback);
}


bool __stdcall _ProcessDir(const CString dir, const CString filter, const CString regExpress, std::function<bool(const CString)> callback)
{
	if (!callback)
	{
		MYCOUT << _T("_ProcessDir callback is null") << std::endl;
		return false;
	}

	CString cstrFind = "";
	bool bUseRegex = filter.IsEmpty() && !regExpress.IsEmpty();
	if (bUseRegex)
	{
		if (_T("\\*.*") != dir.Right(4))
		{
			cstrFind = (dir.Right(1) == '\\' ? dir : (dir + "\\")) + _T("*.*");
		}
		else
		{
			cstrFind = dir;
		}
	}
	else
	{
		cstrFind = (dir.Right(1) == '\\' ? dir : (dir + "\\")) + filter;
	}

	CFileFind finder;
	BOOL bFind = finder.FindFile(cstrFind);
	if (!bFind)
	{
		MYCOUT << _T("_ProcessDir dir illegal or not existed: ") << cstrFind << std::endl;
		return false;
	}

	while (bFind)
	{
		bFind = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}

		CString cstrPath = finder.GetFilePath();
		if (finder.IsDirectory())
		{
			_ProcessDir(cstrPath + _T("\\*.*"), filter, regExpress, callback);
		}
		else
		{
			if (bUseRegex)
			{
				try
				{
					tregex regExpress((tstring)regExpress, std::regex_constants::icase);
					tmatch ms;
					tstring txt = cstrPath.GetBuffer(0);
					if (std::regex_match(txt, ms, regExpress))
					{
						if (ms.size() > 0)
						{
							if (!callback(cstrPath))
							{
								MYCOUT << _T("_ProcessDir callback failed path: ") << cstrPath << std::endl;
							}
						}
					}
				}
				catch (const std::regex_error& e)
				{
					MYCOUT << _T("_ProcessDir regex_error: ") << e.code() << _T(",") << e.what() << std::endl;
					continue;
				}
			}
			else
			{
				if (!callback(cstrPath))
				{
					MYCOUT << _T("_ProcessDir callback return fals, path: ") << cstrPath << std::endl;
				}
			}
		}
	}

	return true;
}

ELIB_API bool __stdcall ProcessDirByRegexEx(const CString dir, const CString regExpress, std::function<bool(const CString)> callback)
{
	return _ProcessDir(dir, "", regExpress, callback);
}

ELIB_API bool __stdcall ProcessDirEx(const CString dir, const CString filter, std::function<bool(const CString)> callback)
{
	return _ProcessDir(dir, filter, "", callback);
}


ELIB_API bool __stdcall ProcessStorageFile(CString file, std::function<bool(IStorage *)>callback)
{
	MYCOUT << _T(" ProcessBMF file: ") << file.GetBuffer(0) << std::endl;

	HRESULT hr = S_FALSE;

	USES_CONVERSION;
	LPCOLESTR path = T2COLE(file);
	hr = ::StgIsStorageFile(path);
	if (FAILED(hr))
	{
		MYCOUT << file << _T(" Is not a StorageFile!") << std::endl;
		return false;
	}

	IStorage *pStgRoot = nullptr;
	hr = ::StgOpenStorage(path, NULL, STGM_SHARE_EXCLUSIVE, 0, 0, &pStgRoot);
	if (FAILED(hr))
	{
		MYCOUT << file << _T(" Open failed as a storage file!") << std::endl;
		return false;
	}

	if (!callback || !callback(pStgRoot))
	{
		MYCOUT << file << _T(" callback is null or return false!") << std::endl;
		return false;
	}
	
	SAFE_RELEASE_EX(pStgRoot);
	return true;
}