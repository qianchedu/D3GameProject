#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "RenderInterface.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//D3D ����ṹ
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
