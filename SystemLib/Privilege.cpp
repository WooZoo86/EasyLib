#include "stdafx.h"
#include "System.h"
#include "./Common/defines.h"
#include <tchar.h>


/********************************************************************************************************
*	Function:		IsUACEnabled
*	Input：			输入参数说明，包括每个参数的作
*	Output:			对输出参数的说明，包括每个参数的作
*	Return:			true 具有管理员权限，false 非管理员权限
*	Called:			调用的函数
*	Caller:			被调用的函数
*	Accessed:		被访问的表（此项仅对于牵扯到数据库操作的程序）
*	Updated:		被修改的表（此项仅对于牵扯到数据库操作的程序）
*	Description:	判断用户是否以管理员UAC方式运行程序
*	Others:			其它内容的说明
*	Modified:		修改记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
********************************************************************************************************/
bool __stdcall IsUACEnabled()
{
	DWORD dwType = 4, cbData = 4;
	HKEY phkResult = nullptr;
	BYTE Data[4] = { 0 };
	bool bRet = true;


	if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\"),
		0, 0x20019u, &phkResult))
	{
		*(DWORD *)Data = 0;

		if (!RegQueryValueExW(phkResult, _T("EnableLUA"), nullptr, &dwType, Data, &cbData))
			bRet = *(DWORD *)Data == 1;

		RegCloseKey(phkResult);
	}

	return bRet;
}
