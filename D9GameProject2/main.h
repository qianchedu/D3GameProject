#ifndef _UGP_MAIN_H_

#define	_UGP_MAIN_H_

#include "../StrandedEngine/engine.h"
#pragma comment(lib,"../Debug/StrandedEngine.lib")

#define WINDOW_CLASS "StrandedGame"
#define WINDOW_TITLE "Stranded"
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define FULLSCREENN 0

//初始化游戏引擎
bool InitializeEngine();
//释放游戏引擎
void ShutdownEngine();
//初始化游戏
bool GameInitialize();
//游戏循环处理
void GameLoop();
//释放游戏资源
void GameShutdown();

#endif //_UGP_MAIN_H_