#ifndef EASY_LIB_FOR_CPP_DEFINES_HEADER
#define	EASY_LIB_FOR_CPP_DEFINES_HEADER

#pragma once


#ifdef _UNICODE
#define MYCOUT	std::wcout

#define tregex	std::wregex
#define	tmatch	std::wsmatch
#define	tstring std::wstring
#else
#define MYCOUT	std::cout

#define tregex	std::regex
#define	tmatch	std::smatch
#define	tstring std::string
#endif//_UNICODE

#define SAFE_RELEASE_EX(p)	{if(p) p->Release();}


typedef unsigned char U8;





#endif // !EASY_LIB_FOR_CPP_DEFINES_HEADER


