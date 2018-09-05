#include "stdafx.h"
#include <regex>
#include <io.h>
#include "Interface.h"
#include "defines.h"



static bool __stdcall ProcessDirW(const wchar_t* dir, const wchar_t* filter, const wchar_t* regExpress, std::function<bool(const wchar_t*)> callback)
{
	wchar_t szRoot[MAX_PATH + 1] = { 0 };
	wchar_t szFind[MAX_PATH + 1] = { 0 };
	wchar_t szPath[MAX_PATH + 1] = { 0 };

	wcsncpy(szRoot, dir, MAX_PATH);
	if (dir[wcslen(dir) - 1] != L'\\')
		wcsncat(szRoot, L"\\", MAX_PATH);
	wcsncpy(szFind, szRoot, MAX_PATH);

	bool bUseRegex = !filter && regExpress;
	if (bUseRegex)
	{
		wcsncat(szFind, L"*.*", MAX_PATH);
	}
	else
	{
		wcsncat(szFind, filter, MAX_PATH);
	}

	WIN32_FIND_DATAW file = { 0 };
	HANDLE hFind = FindFirstFileW(szFind, &file);
	SAFE_CHECK_API_RETURN(hFind, == , INVALID_HANDLE_VALUE, false);

	do
	{
		if (!_wcsicmp(file.cFileName, L".") || !_wcsicmp(file.cFileName, L".."))
			continue;
		
		memset(szPath, 0, MAX_PATH + 1);
		wcsncpy(szPath, szRoot, MAX_PATH);
		wcsncat(szPath, file.cFileName, MAX_PATH);

		if (file.dwFileAttributes & _A_SUBDIR)
		{
			wcsncat(szPath, L"\\", MAX_PATH);
			ProcessDirW(szPath, filter, regExpress, callback);
		}
		else
		{
			if (bUseRegex)
			{
				try
				{
					std::wregex regExpress(regExpress, std::regex_constants::icase);
					std::wsmatch ms;
					std::wstring txt = szPath;
					if (std::regex_match(txt, ms, regExpress))
					{
						if (ms.size() > 0)
						{
							if (!callback(szPath))
							{
								N_DEBUGOUTW(L"callback failed, path: %s", szPath);
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
					N_DEBUGOUTW(L"callback failed, path: %s", szPath);
				}
			}
		}
	} while (!FindNextFileW(hFind, &file));

	FindClose(hFind);

	return true;
}

static bool __stdcall ProcessDirA(const char* dir, const char* filter, const char* regExpress, std::function<bool(const char*)> callback)
{
	char szRoot[MAX_PATH + 1] = { 0 };
	char szFind[MAX_PATH + 1] = { 0 };
	char szPath[MAX_PATH + 1] = { 0 };

	strncpy(szRoot, dir, MAX_PATH);
	if (dir[strlen(dir) - 1] != '\\')
		strncat(szRoot, "\\", MAX_PATH);
	strncpy(szFind, szRoot, MAX_PATH);

	bool bUseRegex = !filter && regExpress;
	if (bUseRegex)
	{
		strncat(szFind, "*.*", MAX_PATH);
	}
	else
	{
		strncat(szFind, filter, MAX_PATH);
	}

	WIN32_FIND_DATAA file = { 0 };
	HANDLE hFind = FindFirstFileA(szFind, &file);
	SAFE_CHECK_API_RETURN(hFind, == , INVALID_HANDLE_VALUE, false);

	do
	{
		if (!_strcmpi(file.cFileName, ".") || !_strcmpi(file.cFileName, ".."))
			continue;

		memset(szPath, 0, MAX_PATH + 1);
		strncpy(szPath, szRoot, MAX_PATH);
		strncat(szPath, file.cFileName, MAX_PATH);

		if (file.dwFileAttributes & _A_SUBDIR)
		{
			strncat(szPath, "\\", MAX_PATH);
			ProcessDirA(szPath, filter, regExpress, callback);
		}
		else
		{
			if (bUseRegex)
			{
				try
				{
					std::regex regExpress(regExpress, std::regex_constants::icase);
					std::smatch ms;
					std::string txt = szPath;
					if (std::regex_match(txt, ms, regExpress))
					{
						if (ms.size() > 0)
						{
							if (!callback(szPath))
							{
								N_DEBUGOUTA("callback failed, path: %s", szPath);
							}
						}
					}
				}
				catch (const std::regex_error& e)
				{
					N_DEBUGOUTA("regex_error,code: %s,des: %s,path: %s", e.code(), e.what(), szPath);
				}
			}
			else
			{
				if (!callback(szPath))
				{
					N_DEBUGOUTA("callback failed, path: %s", szPath);
				}
			}
		}
	} while (!FindNextFileA(hFind, &file));

	FindClose(hFind);

	return true;
}


ELIB_API bool __stdcall TraverseDirExW(const wchar_t* dir, const wchar_t* regExpress, std::function<bool(const wchar_t*)> callback)
{
	SAFE_ASSERT_RETURN(dir, false);
	SAFE_ASSERT_RETURN(regExpress, false);
	SAFE_ASSERT_RETURN(callback, false);

	return ProcessDirW(dir, nullptr, regExpress, callback);
}

ELIB_API bool __stdcall TraverseDirW(const wchar_t* dir, const wchar_t* filter, std::function<bool(const wchar_t*)> callback)
{
	SAFE_ASSERT_RETURN(dir, false);
	SAFE_ASSERT_RETURN(filter, false);
	SAFE_ASSERT_RETURN(callback, false);

	return ProcessDirW(dir, filter, nullptr, callback);
}

ELIB_API bool __stdcall TraverseDirExA(const char* dir, const char* regExpress, std::function<bool(const char*)> callback)
{
	SAFE_ASSERT_RETURN(dir, false);
	SAFE_ASSERT_RETURN(regExpress, false);
	SAFE_ASSERT_RETURN(callback, false);

	return ProcessDirA(dir, nullptr, regExpress, callback);
}

ELIB_API bool __stdcall TraverseDirA(const char* dir, const char* filter, std::function<bool(const char*)> callback)
{
	SAFE_ASSERT_RETURN(dir, false);
	SAFE_ASSERT_RETURN(filter, false);
	SAFE_ASSERT_RETURN(callback, false);

	return ProcessDirA(dir, filter, nullptr, callback);
}

ELIB_API bool __stdcall WatchDirW(const wchar_t* dir, std::function<bool(const wchar_t*)> callback)
{
	HANDLE dwChangeHandles = FindFirstChangeNotificationW(dir, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE);
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

ELIB_API bool __stdcall WatchDirA(const char* dir, std::function<bool(const char*)> callback)
{
	HANDLE dwChangeHandles = FindFirstChangeNotificationA(dir, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE);
	SAFE_CHECK_API_RETURN(dwChangeHandles, == , INVALID_HANDLE_VALUE, false);

	bool bLoop = true;
	BOOL result = FALSE;
	while (bLoop)
	{
		if (!WaitForSingleObject(dwChangeHandles, INFINITE))
		{
			bLoop = callback(dir);
		}

		result = FindNextChangeNotification(dwChangeHandles);
		SAFE_CHECK_API(result, == , FALSE);
		if (FALSE == result)
		{
			FindCloseChangeNotification(dwChangeHandles);
			return false;
		}
	}

	return true;
}