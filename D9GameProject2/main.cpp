#include "main.h"

HWND g_hwnd;
CRenderInterface *g_Render = NULL;

//��Ϣ������
//����1:���ھ��
//����2:��Ϣ
//����3:��Ϣ����
//����4:��Ϣ����
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	//��������Ҫ�������Ϣ
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return  0;
		break;


	case WM_KEYUP:
		if (wParam = VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	default:
		break;
	}
	//������Ϣ ����ת��windowȥ����
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//HINSTANCE : Ӧ�ó�����
//HWND		: ���ھ��


//WinMain����ʱϵͳ�ṩ���û�WindowsӦ�ó�����ڵ�
//����1(:HINSTANCE) :	�ǵ�ǰӦ�ó���ʵ����Handle
//����2(HINSTANCE)  :	��Ӧ�ó�����һ��ʵ����Handle.(MSDN:�������Ҫ֪�������Ƿ�����һ��ʵ����
//					����ʹ��Mutex��ʵ�֣���Mutex����ʵ��ֻ����һ��ʵ��)
//����3(LPTSTR)		:	�ַ������������в���
//����4(int)		:	int�ͣ�ָ��windowsӦ����ôʵ�֣�windows������һϵ�еĺ�����������,��SW��ͷ,
//					��:SW_SHOW(����ֵ��һ��int��)
int WINAPI WinMain(HINSTANCE h,
	HINSTANCE p,
	LPTSTR cmd,
	int show)
{
	//����һ����̨��
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgProc,
		0,
		0,
		h,
		NULL,
		NULL,
		NULL,
		NULL,
		WINDOW_CLASS,
		NULL
	};

	//ע�ᴰ��
	RegisterClassEx(&wc);
	
	if (FULLSCREENN)
	{
		g_hwnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_TITLE,
			WS_POPUP | WS_SYSMENU | WS_VISIBLE,
			NULL, NULL, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, h, NULL);

	}
	else 
	{
		g_hwnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW|WS_VISIBLE,
			NULL, NULL, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, h, NULL);
	}


	if (g_hwnd)
	{
		//�ù�������ָ�����ڵ���ʾ״̬
		ShowWindow(g_hwnd, SW_SHOWDEFAULT);


		//����ˢ�´���
		UpdateWindow(g_hwnd);
	}
	

	//��ʼ����Ϸ����
	if (InitializeEngine())
	{
		//��ʼ����Ϸ
		if (GameInitialize())
		{

	

			//Windows�����У���Ϣ����MSG�ṹ������ʾ��
			MSG msg;
			//�Խṹ�������г�ʼ��������
			ZeroMemory(&msg, sizeof(msg));

			//��������λ��
			SetCursorPos(0,0);

			//ѭ��������Ϣ������
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);

				}
				else
				{
					GameLoop();
				}
			}

		}

	}
	

	GameShutdown();			//�ر���Ϸ
	ShutdownEngine();		//�ر���Ϸ����

	//ע��һ��������
	//����1:��ָ������������ơ�
	//����2:�����ģ��Ĵ������ʵ��
	UnregisterClass(WINDOW_CLASS, h);
}


//��ʼ����Ϸ����
bool InitializeEngine()
{
	//����D3D����Ⱦ��(��δ���ʧ��ֱ�ӷ���)
	if(!CreateD3DRenderer(&g_Render))return false;

	if (!g_Render->Initialize(WIN_WIDTH, WIN_HEIGHT, g_hwnd, FULLSCREENN)) return false;
	return true;
}
//�ͷ���Ϸ����
void ShutdownEngine()
{
}

//��ʼ����Ϸ
bool GameInitialize()
{
	return true;
}
//��Ϸѭ������
void GameLoop()
{
}

//�ͷ���Ϸ��Դ
void GameShutdown()
{
}
