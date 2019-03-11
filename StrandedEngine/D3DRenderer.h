#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "RenderInterface.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//D3D ����ṹ
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
//��Ϸ��Ⱦ��
//class CD3DRenderer : public CRenderInterface {
class CD3DRenderer :public  CRenderInterface {
public:
	CD3DRenderer();
	~CD3DRenderer();

	bool Initialize(int w, int h, WinHWND mainWin, bool fullScreen);

	void CalculateProjMatrix(float fov, float n, float f);
	void CalculateOrthoMatrix(float n, float f);

	void SetClearCol(float r, float g, float b);

	//��ʼ��Ⱦ
	void StartRender(bool bColor, bool bDepth, bool bStencil);
	//������Ⱦ
	void EndRender();

	void ClearBuffers(bool bColor, bool bDepth, bool bStencil);

	//����һ����������
	int CreateStaticBuffer(VertexType, PrimType, int totalVerts, int totalIndeces, int stride, void **data, unsigned int *indices, int *staticId);

	void Shutdown();



private:
	void OneTimeInit();

private :
	D3DCOLOR m_Color;								//d3d��ɫ
	LPDIRECT3D9 m_Direct3D;							//d3d����
	LPDIRECT3DDEVICE9 m_Device;						//d3d�豸
	bool m_renderingScene;							//�Ƿ���Ⱦ

	stD3DStaticBuffer *m_staticBufferList;			//��̬��������
	int m_numStaticBuffers;							//��̬����Ĵ�С
	int m_activeStaticBuffer;						//��ǰ��̬��������

};



bool CreateD3DRenderer(CRenderInterface **pObj);
