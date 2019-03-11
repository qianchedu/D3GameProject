#include "D3DRenderer.h"

bool CreateD3DRenderer(CRenderInterface **pObj)
{
	if (!*pObj)*pObj = new CD3DRenderer();
	else
		return false;

	return true;
}

unsigned long CreateD3DFVF(int flags)
{
	unsigned long fvf = 0;
	return fvf;
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


//清屏颜色
void CD3DRenderer::SetClearCol(float r, float g, float b)
{
	m_Color = D3DCOLOR_COLORVALUE(r, g, b, 1.0f);

}


//开始渲染
void CD3DRenderer::StartRender(bool bColor, bool bDepth, bool bStencil)
{
	if (!m_Device)return;
	unsigned int buffers = 0;
	if (bColor)buffers |= D3DCLEAR_TARGET;
	if (bDepth)buffers |= D3DCLEAR_ZBUFFER;
	if (bStencil)buffers |= D3DCLEAR_STENCIL;

	if (FAILED(m_Device->Clear(0, NULL, buffers, m_Color, 1, 0)))
		return;

	if (FAILED(m_Device->BeginScene()))
		return;

	m_renderingScene = true;			//开始渲染






}
//结束渲染
void CD3DRenderer::EndRender()
{
	if (!m_Device)return;
	m_Device->EndScene();
	m_Device->Present(NULL, NULL, NULL, NULL);		//将渲染的图形显示出来
	m_renderingScene = false;		//渲染结束
}


void  CD3DRenderer::ClearBuffers(bool bColor, bool bDepth, bool bStencil)
{
	if (!m_Device)return;
	
	unsigned int buffers = 0;
	if (bColor)buffers |= D3DCLEAR_TARGET;
	if (bDepth)buffers |= D3DCLEAR_ZBUFFER;
	if (bStencil)buffers |= D3DCLEAR_STENCIL;

	if(m_renderingScene) m_Device->EndScene();
	if (FAILED(m_Device->Clear(0, NULL, buffers, m_Color, 1, 0)))
		return;

	if (m_renderingScene)
		if (FAILED(m_Device->BeginScene()))		//开始渲染
			return;
}

//创建静态缓存
int  CD3DRenderer::CreateStaticBuffer(VertexType vType, PrimType primType, 
	int totalVerts, int totalIndices, int stride, void **data, unsigned int *indices, int *staticId)
{

	void *ptr;
	int index = m_numStaticBuffers;
	if (!m_staticBufferList)
	{
		m_staticBufferList = new stD3DStaticBuffer[1];
		if (!m_staticBufferList)return UGP_FAIL;

	}
	else
	{
		stD3DStaticBuffer *temp;
		temp = new stD3DStaticBuffer[m_numStaticBuffers + 1];

		memcpy(temp, m_staticBufferList, sizeof(stD3DStaticBuffer)*m_numStaticBuffers);

		delete[] m_staticBufferList;
		m_staticBufferList = temp;


	}

	m_staticBufferList[index].numVerts = totalVerts;
	m_staticBufferList[index].numIndices = totalIndices;
	m_staticBufferList[index].primType = primType;
	m_staticBufferList[index].stride = stride;
	m_staticBufferList[index].fvf = CreateD3DFVF(vType);


	//如果有顶点索引 就创建顶点缓存
	if (totalIndices > 0)
	{
		if (FAILED(m_Device->CreateIndexBuffer(sizeof(unsigned int)*totalIndices, D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_staticBufferList[index].ibPtr, NULL)))
		{
			return UGP_FAIL;
		}

		if (FAILED(m_staticBufferList[index].ibPtr->Lock(0, 0, (void**)&ptr, 0)))
			return UGP_FAIL;

		memcpy(ptr, indices, sizeof(unsigned int)*totalIndices);
		m_staticBufferList[index].ibPtr->Unlock();
	}
	else 
	{
		m_staticBufferList[index].ibPtr = NULL;
	}

	if (FAILED(m_Device->CreateVertexBuffer(totalVerts*stride, D3DUSAGE_WRITEONLY, m_staticBufferList[index].fvf,
		D3DPOOL_DEFAULT, &m_staticBufferList[index].vbPtr, NULL)))
		return UGP_FAIL;

	if (FAILED(m_staticBufferList[index].vbPtr->Lock(0, 0, (void**)&ptr, 0)))
		return UGP_FAIL;
	memcpy(ptr, data, totalVerts*stride);

	m_staticBufferList[index].vbPtr->Unlock();
}