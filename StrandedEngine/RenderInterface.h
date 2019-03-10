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

		
protected:
	int m_screenWidth;				//屏幕窗口的宽度
	int m_screenHeight;				//屏幕窗口的高度
	bool m_fullScreen;				//是否全屏

	WinHWND m_mainHandle;

	float m_near;
	float m_far;
};

