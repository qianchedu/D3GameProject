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


bool CD3DRenderer::Initialize(int w, int h, WinHWND mainWin, bool fullScreen)
{

	
	m_mainHandle = mainWin;

	if (!m_mainHandle)return false;
	m_fullScreen = fullScreen;

	D3DDISPLAYMODE mode;
	D3DCAPS9 caps;
	D3DPRESENT_PARAMETERS Params;

	ZeroMemory(&Params, sizeof(Params));

	m_Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_Direct3D) return false;

	if (FAILED(m_Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))return false;

	if (FAILED(m_Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))return false;

	DWORD processing = 0;
	if (caps.VertexProcessingCaps != 0)
		processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if (m_fullScreen)
	{
		Params.FullScreen_RefreshRateInHz = mode.RefreshRate;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	}
	else
	{
		Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	Params.Windowed = !m_fullScreen;
	Params.BackBufferWidth = w;
	Params.BackBufferHeight = h;
	Params.hDeviceWindow = m_mainHandle;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.BackBufferFormat = mode.Format;
	Params.BackBufferCount = 1;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;

	m_screenWidth = w;
	m_screenHeight = h;


	if (FAILED(m_Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_mainHandle, processing, &Params, &m_Device))) return false;

	if (m_Device == NULL) return false;

	OneTimeInit();

	return true;

}


void CD3DRenderer::OneTimeInit()
{
	if (m_Device)return;

	m_Device->SetRenderState(D3DRS_LIGHTING,FALSE);
	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CalculateProjMatrix(D3DX_PI/4,0.1f,1000);
}

void CD3DRenderer::CalculateProjMatrix(float fov, float n, float f)
{
	if (!m_Device)return;
	D3DXMATRIX projection;

	D3DXMatrixPerspectiveFovLH(&projection, fov, (float)m_screenWidth / (float)m_screenHeight, n, f);

	m_Device->SetTransform(D3DTS_PROJECTION, &projection);
}

void CD3DRenderer::CalculateOrthoMatrix(float n, float f)
{

	if (!m_Device)return;
	D3DXMATRIX ortho;
	D3DXMatrixOrthoLH(&ortho, (float)m_screenWidth, (float)m_screenHeight, n, f);
	m_Device->SetTransform(D3DTS_PROJECTION, &ortho);

}