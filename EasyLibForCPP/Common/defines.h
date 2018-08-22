#ifndef EASY_LIB_FOR_CPP_DEFINES_HEADER_
#define	EASY_LIB_FOR_CPP_DEFINES_HEADER_

#pragma once

//#include <typeinfo>


/************************regex*********************************/
#ifdef _UNICODE
	#define tregex	std::wregex
	#define	tsmatch	std::wsmatch
	#define	tstring std::wstring
#else
	#define tregex	std::regex
	#define	tmatch	std::smatch
	#define	tstring std::string
#endif//_UNICODE


#define MYCOUT	std::wcout

/************************base macro*********************************/
#define _STR(s)			#s
#define STR(s)			_STR(s)
#define _CAT(x,y)		x##y
#define CAT(x,y)		_CAT(x,y)
#define _WSTR(x)		L ## x
#define WSTR(x)			_WSTR(x)

/************************debug setting*********************************/
#define	DEBUG_OUT_STR_MAX		0x400
#define DEBUG_TIME_INFO_A		"[" __DATE__ "," __TIME__ "]"
#define DEBUG_TIME_INFO_W		L"[" WSTR(__DATE__) L"," WSTR(__TIME__) L"]"
#define DEBUG_POS_INFO_A		"[" STR(__LINE__) "," __FUNCTION__ "," __FILE__ "] "
#define DEBUG_POS_INFO_W		L"[" WSTR(STR(__LINE__)) L"," WSTR(__FUNCTION__) L"," WSTR(__FILE__) L"] "
#define DEBUG_PREFIX_STR_A		DEBUG_TIME_INFO_A DEBUG_POS_INFO_A
#define DEBUG_PREFIX_STR_W		DEBUG_TIME_INFO_W DEBUG_POS_INFO_W

//#define USE_CONSOLE_OUTPUT
#define DEBUG_CONSOLE_TITLE			_T("Debug Console")
#define CONSOLE_FOREGROUND_MAGENTA	(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CONSOLE_BACKGROUND_WHITE	(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

#ifdef _DEBUG
	#ifdef USE_CONSOLE_OUTPUT
		#define	DBGOUTA(info)	printf(info)
		#define	DBGOUTW(info)	wprintf(info)
	#else
		#define DBGOUTA(info)	OutputDebugStringA(info)
		#define DBGOUTW(info)	OutputDebugStringW(info)
	#endif
#else
	#define DBGOUTA(info)	
	#define DBGOUTW(info)	
#endif // _DEBUG

/*const debug*/
#define _C_DEBUGOUTA(info)		DBGOUTA(DEBUG_PREFIX_STR_A info "\r\n")
#define C_DEBUGOUTA(info)		_C_DEBUGOUTA(info)
#define _C_DEBUGOUTW(info)		DBGOUTW(DEBUG_PREFIX_STR_W info L"\r\n")
#define C_DEBUGOUTW(info)		_C_DEBUGOUTW(info)

/*var debug*/
void VDebugOutA(const char* format, ...);
void VDebugOutW(const wchar_t* format, ...);
#define V_DEBUGOUTA(format,...)		VDebugOutA(format,__VA_ARGS__)
#define V_DEBUGOUTW(format,...)		VDebugOutW(format,__VA_ARGS__)

#ifdef _UNICODE
	#define DBGOUT					DBGOUTW
	#define DEBUG_PREFIX_STR		DEBUG_PREFIX_STR_W
	#define C_DEBUGOUT				C_DEBUGOUTW
	#define V_DEBUGOUT				V_DEBUGOUTW
#else
	#define DBGOUT					DBGOUTA
	#define DEBUG_PREFIX_STR		DEBUG_PREFIX_STR_A
	#define C_DEBUGOUT				C_DEBUGOUTA
	#define V_DEBUGOUT				V_DEBUGOUTA
#endif


/************************safe assert*********************************/
#define	SAFE_ASSERT_POINTER(p,v)	if(!(p)){V_DEBUGOUTA("%s%s == nullptr",typeid((p)).name(),STR((p)));return v;}


/************************safe release*********************************/
#define	SAFE_FREE_POINTER(p)			if((p)){free((p));(p)=nullptr;}
#define	SAFE_RELEASE_POINTER(p)			if((p)){delete (p);(p)=nullptr;}
#define	SAFE_REALSE_REGKEY(p)			if((p)){RegCloseKey((p));(p)=nullptr;}
#define	SAFE_CLOSE_FILE(p)				if((p)){fclose((p));(p)=nullptr;}
#define SAFE_RELEASE_LUASTATE(p)		if((p)){lua_close((p));(p)=nullptr;}
#define SAFE_RELEASE_HANDLE(p)			if((p)){CloseHandle((p));(p)=nullptr;}
#define SAFE_REALASE_MAPVIEW(p)			if((p)){UnmapViewOfFile((p));(p)=nullptr;}
#define SAFE_RELEASE_CRITICALSECTION(p)	if((p)){LeaveCriticalSection((p));DeleteCriticalSection((p));}
#define SAFE_RELEASE_LIB(p)				if((p)){FreeLibrary((p));(p)=nullptr;}
#define SAFE_RELEASE_CLASS(p)			if((p)){(p)->Release();(p)=nullptr;}



/*********************************safe check value*********************************/
#define	SAFE_CHECK_RETURN(value,ops,cmp)	if ((value)ops(cmp)) {FormatWinMsg();}



#endif // !EASY_LIB_FOR_CPP_DEFINES_HEADER_


