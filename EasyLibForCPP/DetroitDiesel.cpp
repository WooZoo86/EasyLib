#include "stdafx.h"
#include "defines.h"


void DDDL_TCM01_SecurityAccess(U8 in[2], U8 out[2])
{
	if (!in || !out)
	{
		printf("Mm..NoWay to be Zero!!");
		return;
	}

	USHORT Res = (((USHORT)in[0]) << 8) + (USHORT)in[1];
	Res *= (USHORT)0x4DD1;
	Res += (USHORT)0x26AD;

	out[0] = (U8)((Res) >> 8);
	out[1] = (U8)(Res);
}

void DDDL_MCM_SecurityAccess(U8 in[2], U8 out[2])
{
	if (!in || !out)
	{
		printf("Mm..NoWay to be Zero!!");
		return;
	}

	UINT Res = (((UINT)in[0]) << 8) + (UINT)in[1];
	Res &= 0x0000FFFF;
	Res <<= (USHORT)0x0002;
	Res += (USHORT)0x07AF;
	Res >>= (USHORT)0x0008;
	Res %= 0x00010000;

	out[0] = (U8)((Res) >> 8);
	out[1] = (U8)(Res);
}

void DDDL_SecurityAccess_Test()
{
	U8 in[2] = { 0xFF,0xFF };
	U8 out[2] = { 0 };

	for (int i = 0; i < 256; i++)
	{
		in[0] = i;
		for (int j = 0; j < 256; j++)
		{
			in[1] = j;

			DDDL_TCM01_SecurityAccess(in, out);
			DDDL_MCM_SecurityAccess(in, out);
			printf("%02X %02X --> %02X %02X\n", in[0], in[1], out[0], out[1]);
		}	

		system("pause");
	}
}