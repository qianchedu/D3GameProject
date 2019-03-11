#pragma once

#include "defines.h"
//游戏渲染接口
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

	//清屏颜色
	virtual void SetClearCol(float r, float g, float b) = 0;		

	//开始渲染
	virtual void StartRender(bool bColor, bool bDepth, bool bStencil) = 0;	
	//结束渲染
	virtual void EndRender() = 0;
	//清除渲染缓存-》工作到一半又不想做了
	virtual void ClearBuffers(bool bColor,bool bDepth,bool bStencil) = 0;

	//参数一：顶点类型
	virtual int CreateStaticBuffer(VertexType , PrimType, int totalVerts,int totalIndeces,int stride,void **data ,unsigned int *indices,int *staticId) = 0;

	//释放资源
	virtual void Shutdown() = 0;
		
protected:
	int m_screenWidth;				//屏幕窗口的宽度
	int m_screenHeight;				//屏幕窗口的高度
	bool m_fullScreen;				//是否全屏

	WinHWND m_mainHandle;

	float m_near;
	float m_far;
};

