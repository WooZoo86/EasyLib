#ifndef EASY_LIB_DEFINES_HEADER_
#define	EASY_LIB_DEFINES_HEADER_

#pragma once

#include <typeinfo>

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


/************************base macro*********************************/
#define _STR(s)			#s
#define STR(s)			_STR(s)
#define _CAT(x,y)		x##y
#define CAT(x,y)		_CAT(x,y)
#define _WSTR(x)		L##x
#define WSTR(x)			_WSTR(x)

/************************debug setting*********************************/
#define	DEBUG_OUT_STR_MAX		0x400
#define DEBUG_TIME_INFO_A		"[" __DATE__ "," __TIME__ "]"
#define DEBUG_TIME_INFO_W		L"[" WSTR(__DATE__) L"," WSTR(__TIME__) L"]"
#define DEBUG_POS_INFO_A		"[" STR(__LINE__) "," __FUNCTION__ "," __FILE__ "] "
#define DEBUG_POS_INFO_W		L"[" WSTR(STR(__LINE__)) L"," WSTR(__FUNCTION__) L"," WSTR(__FILE__) L"] "
#define DEBUG_PREFIX_STR_A		DEBUG_TIME_INFO_A DEBUG_POS_INFO_A
#define DEBUG_PREFIX_STR_W		DEBUG_TIME_INFO_W DEBUG_POS_INFO_W

#define DBGOUT_ANYWAY			1
#define DBGOUT_NORMAL			1
#define DBGOUT_CONST			1
//#define USE_CONSOLE_OUTPUT	1
//#define USE_CUSTOM_DBGOUT		1
#if (!defined(DBGOUT_ANYWAY) && !defined(DBGOUT_NORMAL) && !defined(DBGOUT_CONST))
	#define DBGOUTA(info)	
	#define DBGOUTW(info)
#else
	#ifdef USE_CUSTOM_DBGOUT
		#define DBGOUTA			CustomDbgA
		#define DBGOUTW			CustomDbgW
	#else
		#ifdef USE_CONSOLE_OUTPUT
			#define DEBUG_CONSOLE_TITLE			_T("Debug Window")
			#define CONSOLE_FOREGROUND_MAGENTA	(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
			#define CONSOLE_BACKGROUND_WHITE	(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

			#define	DBGOUTA			printf
			#define	DBGOUTW			wprintf

			void OpenConsole();
			void CloseConsole();
		#else
			#define DBGOUTA			OutputDebugStringA
			#define DBGOUTW			OutputDebugStringW
		#endif
	#endif
#endif

#if (defined(DBGOUT_ANYWAY) || defined(DBGOUT_NORMAL))
	void VDebugOutA(const char* prefix, const char* format, ...);
	void VDebugOutW(const wchar_t* prefix, const wchar_t* format, ...);
#endif

#ifdef DBGOUT_CONST//only const string
	#define _C_DEBUGOUTA(info)		DBGOUTA("[CONST]" DEBUG_PREFIX_STR_A info "\r\n")
	#define C_DEBUGOUTA(info)		_C_DEBUGOUTA(info)
	#define _C_DEBUGOUTW(info)		DBGOUTW(L"[CONST]" DEBUG_PREFIX_STR_W info L"\r\n")
	#define C_DEBUGOUTW(info)		_C_DEBUGOUTW(info)
#else
	#define C_DEBUGOUTA(info)
	#define C_DEBUGOUTW(info)
#endif

#ifdef DBGOUT_NORMAL
	#define N_DEBUGOUTA(format,...)		VDebugOutA("[NORMAL]" DEBUG_PREFIX_STR_A,format,##__VA_ARGS__)
	#define N_DEBUGOUTW(format,...)		VDebugOutW(L"[NORMAL]" DEBUG_PREFIX_STR_W,format,##__VA_ARGS__)
#else
	#define N_DEBUGOUTA(format,...)		
	#define N_DEBUGOUTW(format,...)	
#endif

#ifdef DBGOUT_ANYWAY
	#define A_DEBUGOUTA(format,...)		VDebugOutA("[ANYWAY]" DEBUG_PREFIX_STR_A, format,##__VA_ARGS__)
	#define A_DEBUGOUTW(format,...)		VDebugOutW(L"[ANYWAY]" DEBUG_PREFIX_STR_W, format,##__VA_ARGS__)
#else
	#define A_DEBUGOUTA(format,...)		
	#define A_DEBUGOUTW(format,...)	
#endif

#ifdef _UNICODE
	#define DEBUG_TIME_INFO			DEBUG_TIME_INFO_W
	#define DEBUG_POS_INFO			DEBUG_POS_INFO_W
	#define DEBUG_PREFIX_STR		DEBUG_PREFIX_STR_W
	#define DBGOUT					DBGOUTW
	#define C_DEBUGOUT				C_DEBUGOUTW
	#define N_DEBUGOUT				N_DEBUGOUTW
	#define A_DEBUGOUT				A_DEBUGOUTW
#else
	#define DEBUG_TIME_INFO			DEBUG_TIME_INFO_A
	#define DEBUG_POS_INFO			DEBUG_POS_INFO_A
	#define DEBUG_PREFIX_STR		DEBUG_PREFIX_STR_A
	#define DBGOUT					DBGOUTA
	#define C_DEBUGOUT				C_DEBUGOUTA
	#define N_DEBUGOUT				N_DEBUGOUTA
	#define A_DEBUGOUT				A_DEBUGOUTA
#endif


/************************safe assert*********************************/
#define	SAFE_ASSERT(exp)			if(!(exp)){A_DEBUGOUTA("%s%s == 0",typeid((exp)).name(),STR((exp)));}
#define	SAFE_ASSERT_RETURN(exp,ret)	if(!(exp)){A_DEBUGOUTA("%s%s == 0",typeid((exp)).name(),STR((exp)));return ret;}

#define	SAFE_FAILED(exp)			if(FAILED((exp))){A_DEBUGOUTA("%s%s == 0",typeid((exp)).name(),STR((exp)))}
#define	SAFE_FAILED_RETURN(exp,ret)	if(FAILED((exp))){A_DEBUGOUTA("%s%s == 0",typeid((exp)).name(),STR((exp)));return ret;}

/************************safe release*********************************/
#define NO_USE_INLINE_CODE
#ifdef NO_USE_INLINE_CODE
	typedef enum _POINTER_TYPE
	{
		emPOINTER_TYPE_C,
		emPOINTER_TYPE_CPP,
		emPOINTER_TYPE_CPP_ARRAY,
		emPOINTER_TYPE_REG,
		emPOINTER_TYPE_FILE,
		emPOINTER_TYPE_HANDLE,
		emPOINTER_TYPE_MAPVIEW,
		emPOINTER_TYPE_CS,
		emPOINTER_TYPE_LIB,
		emPOINTER_TYPE_CLASS
	}POINTER_TYPE,*LPPOINTER_TYPE,*PPOINTER_TYPE;

	void SRelease(void* pointer, POINTER_TYPE ptype);

	#define	SAFE_FREE_POINTER(p)			SRelease(p,emPOINTER_TYPE_C)
	#define	SAFE_RELEASE_POINTER(p)			SRelease(p,emPOINTER_TYPE_CPP)
	#define	SAFE_RELEASE_POINTER_ARR(p)		SRelease(p,emPOINTER_TYPE_CPP_ARRAY)
	#define	SAFE_REALSE_REGKEY(p)			SRelease(p,emPOINTER_TYPE_REG)
	#define	SAFE_CLOSE_FILE(p)				SRelease(p,emPOINTER_TYPE_FILE)
	#define SAFE_RELEASE_HANDLE(p)			SRelease(p,emPOINTER_TYPE_HANDLE)
	#define SAFE_REALASE_MAPVIEW(p)			SRelease(p,emPOINTER_TYPE_MAPVIEW)
	#define SAFE_RELEASE_CRITICALSECTION(p)	SRelease(p,emPOINTER_TYPE_CS)
	#define SAFE_RELEASE_LIB(p)				SRelease(p,emPOINTER_TYPE_LIB)
#else
	#define	SAFE_FREE_POINTER(p)			if((p)){free((p));(p)=nullptr;}
	#define	SAFE_RELEASE_POINTER(p)			if((p)){delete (p);(p)=nullptr;}
	#define	SAFE_REALSE_REGKEY(p)			if((p)){RegCloseKey((p));(p)=nullptr;}
	#define	SAFE_CLOSE_FILE(p)				if((p)){fclose((p));(p)=nullptr;}
	#define SAFE_RELEASE_HANDLE(p)			if((p)){CloseHandle((p));(p)=nullptr;}
	#define SAFE_REALASE_MAPVIEW(p)			if((p)){UnmapViewOfFile((p));(p)=nullptr;}
	#define SAFE_RELEASE_CRITICALSECTION(p)	if((p)){LeaveCriticalSection((p));DeleteCriticalSection((p));}
	#define SAFE_RELEASE_LIB(p)				if((p)){FreeLibrary((p));(p)=nullptr;}
#endif

#define SAFE_RELEASE_CLASS(p)				if((p)){(p)->Release();(p)=nullptr;}
#ifdef __cplusplus
	template<class T> void TRelease(T p) { if (p) { p->Release(); p = nullptr; } }
	template<class T> void TDelete(T p) { if (p) {delete p; p = nullptr; } }
	template<class T> void TARRDelete(T p) { if (p) { delete[] p; p = nullptr; } }
	template<class T> int TLength(T& arr) { if (arr) { return sizeof(arr) / sizeof(arr[0]); } return 0;}
#endif
/************************safe check windows API return value************************/
void FormatWinMsg();
#define	SAFE_CHECK_API(value,ops,cmp)				if ((value) ops (cmp)) {FormatWinMsg();}
#define	SAFE_CHECK_API_RETURN(value,ops,cmp,ret)	if ((value) ops (cmp)) {FormatWinMsg();return ret;}


#endif // !EASY_LIB_DEFINES_HEADER_


