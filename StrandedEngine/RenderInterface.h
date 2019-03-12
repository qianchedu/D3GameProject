#pragma once

#include "defines.h"
#include "material.h"
#include "light.h"

//��Ϸ��Ⱦ�ӿ�
class CRenderInterface
{
public:
	CRenderInterface() :m_screenWidth(0),
		m_screenHeight(0),
		m_near(0),
		m_far(0) {}
	virtual ~CRenderInterface() {}
	virtual bool Initialize(int w, int h,WinHWND mainWin, bool fullScreen) = 0;

	virtual void OneTimeInit() = 0;
	virtual void CalculateProjMatrix(float fov, float n, float f) = 0;
	virtual void CalculateOrthoMatrix(float n, float f) = 0;

	//������ɫ
	virtual void SetClearCol(float r, float g, float b) = 0;		

	//��ʼ��Ⱦ
	virtual void StartRender(bool bColor, bool bDepth, bool bStencil) = 0;	
	//������Ⱦ
	virtual void EndRender() = 0;
	//�����Ⱦ����-��������һ���ֲ�������
	virtual void ClearBuffers(bool bColor,bool bDepth,bool bStencil) = 0;

	//����һ����������
	virtual int CreateStaticBuffer(VertexType , PrimType, int totalVerts,int totalIndeces,int stride,void **data ,unsigned int *indices,int *staticId) = 0;

	//�ͷ���Դ
	virtual void Shutdown() = 0;

	//��Ⱦ����
	virtual int Render(int staticId) = 0;

	//���ò���
	virtual void SetMaterial(stMaterial *mat) = 0;
	//���ù���
	virtual void SetLight(stLight *light, int index) = 0;
	//��ĳ����ص�
	virtual void DisabledLight(int index) = 0;			

	//����//
	//����͸����
	virtual void SetTranspency(RenderState state, TransState src, TransState dst) =0;
	//�������
	virtual int AddTexture2D(char *file, int *texId) = 0;

	virtual void SetTextureFilter(int index, int filter, int val) = 0;

	//���ö�������
	virtual void SetMultiTexture() = 0;
	//Ӧ������
	virtual void ApplyTexture(int index,int texId) = 0;
	//������Ļ��ͼ
	virtual void SaveScreenShot(char *file) = 0;
	virtual void EnablePointerSprites(float size, float min, float a, float b, float c) = 0;
	virtual void DisablePointSprites() = 0;
		
protected:
	int m_screenWidth;				//��Ļ���ڵĿ��
	int m_screenHeight;				//��Ļ���ڵĸ߶�
	bool m_fullScreen;				//�Ƿ�ȫ��

	WinHWND m_mainHandle;

	float m_near;
	float m_far;
};

