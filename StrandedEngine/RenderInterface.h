#pragma once

#include "defines.h"
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

		
protected:
	int m_screenWidth;				//��Ļ���ڵĿ��
	int m_screenHeight;				//��Ļ���ڵĸ߶�
	bool m_fullScreen;				//�Ƿ�ȫ��

	WinHWND m_mainHandle;

	float m_near;
	float m_far;
};

