#include "main.h"

HWND g_hwnd;
CRenderInterface *g_Render = NULL;

//消息处理函数
//参数1:窗口句柄
//参数2:消息
//参数3:消息参数
//参数4:消息参数
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	//处理你想要处理的消息
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
	//其他消息 可以转给window去处理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//HINSTANCE : 应用程序句柄
//HWND		: 窗口句柄


//WinMain函数时系统提供给用户Windows应用程序入口点
//参数1(:HINSTANCE) :	是当前应用长须实例的Handle
//参数2(HINSTANCE)  :	是应用程序上一个实例的Handle.(MSDN:如果你想要知道程序是否有另一个实例，
//					建议使用Mutex来实现，用Mutex可以实现只运行一个实例)
//参数3(LPTSTR)		:	字符串，是命令行参数
//参数4(int)		:	int型，指明windows应该怎么实现，windows定义了一系列的宏来帮助记忆,以SW开头,
//					如:SW_SHOW(返回值是一个int型)
int WINAPI WinMain(HINSTANCE h,
	HINSTANCE p,
	LPTSTR cmd,
	int show)
{
	//属于一个窗台类
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

	//注册窗口
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
		//该功能设置指定窗口的显示状态
		ShowWindow(g_hwnd, SW_SHOWDEFAULT);


		//立即刷新窗口
		UpdateWindow(g_hwnd);
	}
	

	//初始化游戏引擎
	if (InitializeEngine())
	{
		//初始化游戏
		if (GameInitialize())
		{

	

			//Windows程序中，消息是由MSG结构体来表示的
			MSG msg;
			//对结构体对象进行初始化或清零
			ZeroMemory(&msg, sizeof(msg));

			//设置鼠标的位置
			SetCursorPos(0,0);

			//循环访问消息并处理
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
	

	GameShutdown();			//关闭游戏
	ShutdownEngine();		//关闭游戏引擎

	//注销一个窗口类
	//参数1:它指定窗口类的名称。
	//参数2:句柄的模块的创建类的实例
	UnregisterClass(WINDOW_CLASS, h);
}


//初始化游戏引擎
bool InitializeEngine()
{
	//创建D3D的渲染器(如何创建失败直接返回)
	if(!CreateD3DRenderer(&g_Render))return false;

	if (!g_Render->Initialize(WIN_WIDTH, WIN_HEIGHT, g_hwnd, FULLSCREENN)) return false;
	return true;
}
//释放游戏引擎
void ShutdownEngine()
{
}

//初始化游戏
bool GameInitialize()
{
	return true;
}
//游戏循环处理
void GameLoop()
{
}

//释放游戏资源
void GameShutdown()
{
}
