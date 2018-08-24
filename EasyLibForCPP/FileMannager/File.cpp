#include "stdafx.h"
#include "defines.h"
#include "Interface.h"


ELIB_API bool __stdcall ProcessStorageFile(const TCHAR* file, std::function<bool(IStorage *)>callback)
{
	SAFE_ASSERT_POINTER(file, false);
	SAFE_ASSERT_POINTER(callback, false);

	HRESULT hr = S_FALSE;
	hr = ::StgIsStorageFile(file);
	if (FAILED(hr))
	{
		N_DEBUGOUT(_T("%s %s"), file, _T(" Is not a StorageFile!"));
		return false;
	}

	IStorage *pStgRoot = nullptr;
	hr = ::StgOpenStorage(file, nullptr, STGM_SHARE_EXCLUSIVE, nullptr, 0, &pStgRoot);
	if (FAILED(hr))
	{
		N_DEBUGOUT(_T("%s %s"), file, _T(" Open failed as a storage file!"));
		return false;
	}

	if (!callback(pStgRoot))
	{
		N_DEBUGOUT(_T("%s %s"), file, _T(" callback return false!"));
		return false;
	}

	SAFE_RELEASE_CLASS(pStgRoot);
	return true;
}