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


//��Ⱦ״̬
enum RenderState
{
	CULL_NONE,
	CULL_CW,
	CULL_CCW,
	DEPTH_NONE,
	DEPTH_READONLY,
	DEPTH_READWRITE,
	SHADE_POINTS,
	SHADE_SOLIDTRI,
	SHADE_WIRETR,
	SHADE_WIREPOLY,
	TRANSPARENCY_NONE,
	TRANSPARENCY_ENABLE

};

//͸����
enum TransState
{
	TRANS_ZERO = 1,
	TRANS_ONE,
	TRANS_SRCCOLOR,
	TRANS_INVSRCOLOR,
	TRANS_SRCALPHA,
	TRANS_INVSRCALPHA,
	TRANS_DSTALPHA,
	TRANS_INVDSTALPHA,
	TRANS_DSTCOLOR,
	TRANS_INVDSTCOLOR,
	TRANS_SRCALPHASAT,
	TRANS_BOTHSRCALPHA,
	TRANS_INVBOTHSRCALPHA,
	TRANS_BLENDFACTOR,
	TRANS_INVBLENDFACTOR,
};

//����
enum TextureState
{
	MIN_FILTER,
	MAG_FILTER,
	MIP_FILTER,

};

//�����������Ĺ���״̬
enum FilterType
{
	POINT_TYPE,
	LINEAR_TYPE,
	ANISOTROPIC_TYPE
};
