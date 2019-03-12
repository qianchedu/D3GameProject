#pragma once

#include <Windows.h>

#define UGP_INVALID -1
#define UGP_OK 1
#define UGP_FAIL 0

#define LIGHT_POINT 1
#define LIGHT_DIRECTIONAL 2
#define LIGHT_SPOT 3


#define WinHWND HWND

typedef long VertexType;

enum  PrimType
{
	NULL_TYPE,
	POINT_LIST,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	LINE_LIST,
	LINE_STRIP

};

