#include "stdafx.h"
#include "System.h"
#include "./Common/defines.h"
#include <tchar.h>


/********************************************************************************************************
*	Function:		IsUACEnabled
*	Input��			�������˵��������ÿ����������
*	Output:			�����������˵��������ÿ����������
*	Return:			true ���й���ԱȨ�ޣ�false �ǹ���ԱȨ��
*	Called:			���õĺ���
*	Caller:			�����õĺ���
*	Accessed:		�����ʵı����������ǣ�������ݿ�����ĳ���
*	Updated:		���޸ĵı����������ǣ�������ݿ�����ĳ���
*	Description:	�ж��û��Ƿ��Թ���ԱUAC��ʽ���г���
*	Others:			�������ݵ�˵��
*	Modified:		�޸ļ�¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
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
