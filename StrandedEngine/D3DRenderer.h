#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "RenderInterface.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//D3D 缓存结构
struct stD3DStaticBuffer
{
	stD3DStaticBuffer() :vbPtr(0), ibPtr(0), numVerts(0),
		numIndices(0), stride(0), fvf(0), primType(NULL_TYPE) {}

	LPDIRECT3DVERTEXBUFFER9 vbPtr;
	LPDIRECT3DINDEXBUFFER9 ibPtr;
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

	void SetClearCol(float r, float g, float b);

	//开始渲染
	void StartRender(bool bColor, bool bDepth, bool bStencil);
	//结束渲染
	void EndRender();

	void ClearBuffers(bool bColor, bool bDepth, bool bStencil);

	//参数一：顶点类型
	int CreateStaticBuffer(VertexType, PrimType, int totalVerts, int totalIndeces, int stride, void **data, unsigned int *indices, int *staticId);

	void Shutdown();



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
