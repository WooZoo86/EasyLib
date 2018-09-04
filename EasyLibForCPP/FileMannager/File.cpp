#include "stdafx.h"
#include "defines.h"
#include "Interface.h"


ELIB_API bool __stdcall ProcessStorageFile(const wchar_t* file, std::function<bool(IStorage *)>callback)
{
	SAFE_ASSERT_RETURN(file, false);
	SAFE_ASSERT_RETURN(callback, false);

	HRESULT hr = S_FALSE;
	hr = ::StgIsStorageFile(file);
	SAFE_CHECK_API_RETURN((HRESULT)hr, < , 0, false);

	IStorage *pStgRoot = nullptr;
	hr = ::StgOpenStorage(file, nullptr, STGM_SHARE_EXCLUSIVE, nullptr, 0, &pStgRoot);
	SAFE_CHECK_API_RETURN((HRESULT)hr, < , 0, false);

	if (!callback(pStgRoot))
	{
		N_DEBUGOUTW(L"%s %s", file, " callback return false!");
		return false;
	}

	TRelease(pStgRoot);
	return true;
}