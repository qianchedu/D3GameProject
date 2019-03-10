#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "RenderInterface.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//D3D 缓存结构
struct stD3DStaticBuffer
{
	LPDIRECT3DVERTEXBUFFER9 vbPtr;
	LPDIRECT3DINDEXBUFFER9 idPtr;
	int numVerts;
	int numIndices;
	int stride;
	unsigned long fvf;
	PrimType primType;

};
//游戏渲染器
//class CD3DRenderer : public CRenderInterface {
class CD3DRenderer :public  CRenderInterface {
public:
	CD3DRenderer();
	~CD3DRenderer();

	bool Initialize(int w, int h, WinHWND mainWin, bool fullScreen);

	void CalculateProjMatrix(float fov, float n, float f);
	void CalculateOrthoMatrix(float n, float f);


private:
	void OneTimeInit();

private :
	D3DCOLOR m_Color;								//d3d颜色
	LPDIRECT3D9 m_Direct3D;							//d3d对象
	LPDIRECT3DDEVICE9 m_Device;						//d3d设备
	bool m_renderingScene;							//是否渲染

	stD3DStaticBuffer *m_staticBufferList;			//静态缓存数组
	int m_numStaticBuffers;							//静态缓存的大小
	int m_activeStaticBuffer;						//当前静态缓存数量

};



bool CreateD3DRenderer(CRenderInterface **pObj);
