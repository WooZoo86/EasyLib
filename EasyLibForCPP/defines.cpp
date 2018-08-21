#include "stdafx.h"
#include "defines.h"
#include <cstdarg>
#include <tchar.h>


void DegbugOut(const TCHAR* format, ...)
{
	va_list va = nullptr;
	TCHAR buf[DEBUG_OUT_STR_MAX + 1] = { 0 };

	va_start(va, format);
	_vsntprintf(buf, DEBUG_OUT_STR_MAX, format, va);
	MYCOUT(buf);
}
