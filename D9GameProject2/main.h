#ifndef _UGP_MAIN_H_

#define	_UGP_MAIN_H_

#include "../StrandedEngine/engine.h"
#pragma comment(lib,"../Debug/StrandedEngine.lib")

#define WINDOW_CLASS "StrandedGame"
#define WINDOW_TITLE "Stranded"
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define FULLSCREENN 0

//��ʼ����Ϸ����
bool InitializeEngine();
//�ͷ���Ϸ����
void ShutdownEngine();
//��ʼ����Ϸ
bool GameInitialize();
//��Ϸѭ������
void GameLoop();
//�ͷ���Ϸ��Դ
void GameShutdown();

#endif //_UGP_MAIN_H_