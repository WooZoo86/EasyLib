#include "stdafx.h"
#include <regex>
#include <io.h>
#include <iostream>
#include "Interface.h"
#include "defines.h"



static bool __stdcall ProcessDir(const TCHAR* dir, const TCHAR* filter, const TCHAR* regExpress, std::function<bool(const TCHAR*)> callback)
{
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
	if (INVALID_HANDLE_VALUE == (HANDLE)hFind)
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
					tsmatch ms;
					tstring txt = szPath;
					if (std::regex_match(txt, ms, regExpress))
					{
						if (ms.size() > 0)
						{
							if (!callback(szPath))
							{
								N_DEBUGOUT(_T("callback failed, path: %s"), szPath);
							}
						}
					}
				}
				catch (const std::regex_error& e)
				{
					N_DEBUGOUTA("regex_error,code: %s,des: %s,path: %ls", e.code(), e.what(), szPath);
				}
			}
			else
			{
				if (!callback(szPath))
				{
					N_DEBUGOUT(_T("callback failed, path: %s"), szPath);
				}
			}
		}
	} while (!_tfindnext(hFind, &file));

	_findclose(hFind);

	return true;
}

ELIB_API bool __stdcall TraverseDirEx(const TCHAR* dir, const TCHAR* regExpress, std::function<bool(const TCHAR*)> callback)
{
	SAFE_ASSERT_POINTER(dir, false);
	SAFE_ASSERT_POINTER(regExpress, false);
	SAFE_ASSERT_POINTER(callback, false);

	return ProcessDir(dir, nullptr, regExpress, callback);
}

ELIB_API bool __stdcall TraverseDir(const TCHAR* dir, const TCHAR* filter, std::function<bool(const TCHAR*)> callback)
{
	SAFE_ASSERT_POINTER(dir, false);
	SAFE_ASSERT_POINTER(filter, false);
	SAFE_ASSERT_POINTER(callback, false);

	return ProcessDir(dir, filter, nullptr, callback);
}

ELIB_API bool __stdcall WatchDir(const TCHAR* dir, std::function<bool(const TCHAR*)> callback)
{
	HANDLE dwChangeHandles = FindFirstChangeNotification(dir, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE);
	SAFE_CHECK_API_RETURN(dwChangeHandles, ==, INVALID_HANDLE_VALUE, false);

	bool bLoop = true;
	while (bLoop)
	{
		if (!WaitForSingleObject(dwChangeHandles, INFINITE))
		{
			bLoop = callback(dir);
		}

		if (FALSE == FindNextChangeNotification(dwChangeHandles))
		{
			A_DEBUGOUT(_T("FindNextChangeNotification failed!!!"));
			FindCloseChangeNotification(dwChangeHandles);
			return false;
		}
	}

	return true;
}