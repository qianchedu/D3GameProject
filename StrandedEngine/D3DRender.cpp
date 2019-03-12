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
	Shutdown();
}


bool CD3DRenderer::Initialize(int w, int h, WinHWND mainWin, bool fullScreen)
{

	Shutdown();
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

	*staticId = m_numStaticBuffers;
	m_numStaticBuffers++;

	return UGP_OK;
}


void CD3DRenderer::Shutdown()
{
	for (int s = 0; s < m_numStaticBuffers; s++)
	{
		if (m_staticBufferList[s].vbPtr)
		{
			m_staticBufferList[s].vbPtr->Release();
			m_staticBufferList[s].vbPtr = NULL;
		}

		if (m_staticBufferList[s].ibPtr)
		{
			m_staticBufferList[s].ibPtr->Release();
			m_staticBufferList[s].ibPtr = NULL;
		}
	}

	m_numStaticBuffers = 0;
	if (m_staticBufferList)delete[] m_staticBufferList;
	m_staticBufferList = NULL;

	if (m_Device)m_Device->Release();

	if (m_Direct3D)m_Direct3D->Release();

	m_Device = NULL;
	m_Direct3D = NULL;

}



int CD3DRenderer::Render(int staticId)
{

	if (staticId >= m_numStaticBuffers) return UGP_FAIL;

	if (m_activeStaticBuffer != staticId)
	{
		if(m_staticBufferList[staticId].ibPtr != NULL)
			m_Device->SetIndices(m_staticBufferList[staticId].ibPtr);

		m_Device->SetStreamSource(0,m_staticBufferList[staticId].vbPtr,0,m_staticBufferList[staticId].stride);
		m_Device->SetFVF(m_staticBufferList[staticId].fvf);
		m_activeStaticBuffer = staticId;

	}


	if (m_staticBufferList[staticId].ibPtr != NULL)
	{
		//索引缓存画法
		switch (m_staticBufferList[staticId].primType)
		{
		case POINT_LIST:
			if(FAILED(m_Device->DrawPrimitive(D3DPT_POINTLIST,0,m_staticBufferList[staticId].numVerts)))
				return UGP_FAIL;
			break;

		case TRIANGLE_LIST:
			if (FAILED(m_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_staticBufferList[staticId].numVerts / 3, 0, m_staticBufferList[staticId].numIndices)))
				return UGP_FAIL;
			break;

		case TRIANGLE_STRIP:
			if (FAILED(m_Device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_staticBufferList[staticId].numVerts / 2, 0, m_staticBufferList[staticId].numIndices)))
				return UGP_FAIL;
			break;

		case TRIANGLE_FAN:
			if (FAILED(m_Device->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, m_staticBufferList[staticId].numVerts / 3, 0, m_staticBufferList[staticId].numIndices)))
				return UGP_FAIL;
			break;

		case LINE_LIST:
			if (FAILED(m_Device->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_staticBufferList[staticId].numVerts / 3, 0, m_staticBufferList[staticId].numIndices)))
				return UGP_FAIL;
			break;

		case LINE_STRIP:
			if (FAILED(m_Device->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, m_staticBufferList[staticId].numVerts / 3, 0, m_staticBufferList[staticId].numIndices)))
				return UGP_FAIL;
			break;
		default:
			return UGP_FAIL;
		}
		//m_Device->DrawIndexedPrimitive();
	}
	else
	{
	

		//顶点缓存画法
		switch (m_staticBufferList[staticId].primType)
		{
		case POINT_LIST:
			if (FAILED(m_Device->DrawPrimitive(D3DPT_POINTLIST, 0, m_staticBufferList[staticId].numVerts)))
				return UGP_FAIL;
			break;

		case TRIANGLE_LIST:
			if (FAILED(m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, (int)(m_staticBufferList[staticId].numVerts / 3))))
				return UGP_FAIL;

			break;

		case TRIANGLE_STRIP:
			if (FAILED(m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, (int)(m_staticBufferList[staticId].numVerts / 2))))
				return UGP_FAIL;
			break;

		case TRIANGLE_FAN:
			if (FAILED(m_Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, (int)(m_staticBufferList[staticId].numVerts / 2))))
				return UGP_FAIL;
			break;

		case LINE_LIST:
			if (FAILED(m_Device->DrawPrimitive(D3DPT_LINELIST, 0, (int)(m_staticBufferList[staticId].numVerts / 2))))
				return UGP_FAIL;
			break;

		case LINE_STRIP:
			if (FAILED(m_Device->DrawPrimitive(D3DPT_LINESTRIP, 0, (int)(m_staticBufferList[staticId].numVerts / 2))))
				return UGP_FAIL;
			break;
		default:
			return UGP_FAIL;
		}
	}


	return UGP_OK;


}


//设置材质
void CD3DRenderer::SetMaterial(stMaterial *mat)
{
	if (!mat || !m_Device) return;

	D3DMATERIAL9  m =
	{
		mat->diffuseR,
		mat->diffuseG,
		mat->diffuseB,
		mat->diffuseA,

		mat->ambientR,
		mat->ambientG,
		mat->ambientB,
		mat->ambientA,

		mat->specularR,
		mat->specularG,
		mat->specularB,
		mat->specularA,

		mat->emissiveR,
		mat->emissiveG,
		mat->emissiveB,
		mat->emissiveA,

		mat->power
	};

	m_Device->SetMaterial(&m);

}
//设置光照
void CD3DRenderer::SetLight(stLight *light, int index)
{
	if (!light || !m_Device || index < 0)return;

	D3DLIGHT9 l;
	l.Ambient.a = light->ambientA;
	l.Ambient.r = light->ambientR;
	l.Ambient.g = light->ambientG;
	l.Ambient.b = light->ambientB;


	l.Attenuation0 = light->attenuation0;
	l.Attenuation1 = light->attenuation1;
	l.Attenuation2 = light->attenuation2;

	l.Diffuse.a = light->diffuseA;
	l.Diffuse.r = light->diffuseR;
	l.Diffuse.g = light->diffuseG;
	l.Diffuse.b = light->diffuseB;

	l.Direction.x = light->dirX;
	l.Direction.y = light->dirY;
	l.Direction.z = light->dirZ;

	l.Falloff = light->falloff;
	l.Phi = light->phi;

	l.Position.x = light->posX;
	l.Position.y = light->posY;
	l.Position.z = light->posZ;

	l.Range = light->range;
	l.Specular.a = light->specularA;
	l.Specular.r = light->specularR;
	l.Specular.g = light->specularG;
	l.Specular.b = light->specularB;

	l.Theta = light->theta;

	if (light->type == LIGHT_POINT) l.Type = D3DLIGHT_POINT;
	else if (light->type == LIGHT_SPOT)l.Type = D3DLIGHT_SPOT;
	else l.Type = D3DLIGHT_DIRECTIONAL;
	

	m_Device->SetLight(index, &l);
	m_Device->LightEnable(index,TRUE);



}
//把某个光关掉
void CD3DRenderer::DisabledLight(int index) 
{
	m_Device->LightEnable(index, FALSE);
}

//纹理处理//

//设置透明的
void CD3DRenderer::SetTranspency(RenderState state, TransState src, TransState dst)
{
	if (!m_Device)return;
	if (state == TRANSPARENCY_NONE)
	{
		m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		return;
	}

	if (state == TRANSPARENCY_ENABLE)
	{
		m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		switch (src)
		{
		case TRANS_ZERO:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			break;

		case TRANS_ONE:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			break;
		case TRANS_SRCCOLOR:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			break;
		case TRANS_INVSRCOLOR:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		case TRANS_SRCALPHA:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			break;
		case TRANS_INVSRCALPHA:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case TRANS_DSTALPHA:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
			break;
		case TRANS_INVDSTALPHA:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
			break;
		case TRANS_DSTCOLOR:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			break;
		case TRANS_INVDSTCOLOR:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
			break;
		case TRANS_SRCALPHASAT:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHASAT);
			break;
		case TRANS_BOTHSRCALPHA:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHSRCALPHA);
			break;
		case TRANS_INVBOTHSRCALPHA:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHINVSRCALPHA);
			break;
		case TRANS_BLENDFACTOR:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
			break;
		case TRANS_INVBLENDFACTOR:
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVBLENDFACTOR);
			break;
		default:
			m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			return;
			break;
		}


		switch (dst)
		{
		case TRANS_ZERO:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			break;

		case TRANS_ONE:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;
		case TRANS_SRCCOLOR:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			break;
		case TRANS_INVSRCOLOR:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		case TRANS_SRCALPHA:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
			break;
		case TRANS_INVSRCALPHA:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case TRANS_DSTALPHA:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
			break;
		case TRANS_INVDSTALPHA:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
			break;
		case TRANS_DSTCOLOR:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
			break;
		case TRANS_INVDSTCOLOR:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
			break;
		case TRANS_SRCALPHASAT:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
			break;
		case TRANS_BOTHSRCALPHA:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHSRCALPHA);
			break;
		case TRANS_INVBOTHSRCALPHA:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);
			break;
		case TRANS_BLENDFACTOR:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BLENDFACTOR);
			break;
		case TRANS_INVBLENDFACTOR:
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
			break;
		default:
			m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			return;
			break;
		}
	}
}
//添加纹理
int CD3DRenderer::AddTexture2D(char *file, int *texId)
{
	return 1;
}

void CD3DRenderer::SetTextureFilter(int index, int filter, int val)
{
}

//设置多重纹理
void CD3DRenderer::SetMultiTexture()
{
}
//应用纹理
void CD3DRenderer::ApplyTexture(int index, int texId)
{
}
//保存屏幕截图
void CD3DRenderer::SaveScreenShot(char *file)
{
}
void CD3DRenderer::EnablePointerSprites(float size, float min, float a, float b, float c) 
{
}
void CD3DRenderer::DisablePointSprites()
{
}