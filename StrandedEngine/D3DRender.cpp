#include "D3DRenderer.h"

bool CreateD3DRenderer(CRenderInterface **pObj)
{
	if (!*pObj)*pObj = new CD3DRenderer();
	else
		return false;

	return true;
}

CD3DRenderer::CD3DRenderer()
{
	m_Direct3D = NULL;
	m_Device = NULL;
	m_renderingScene = false;
	m_numStaticBuffers = 0;
	m_activeStaticBuffer = UGP_INVALID;
	m_staticBufferList = NULL;

}

CD3DRenderer::~CD3DRenderer()
{

}