/*
 * main_server.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */


#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "../lightframe/frame_typedef.h"
#include "common/common_errordef.h"
#include "common/common_singleton.h"
#include "def/dal_def.h"
#include "global.h"
#include "main_server.h"
#include "main_frame.h"


FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

//初始化服务器命令和状态
ServerCommand CMainServer::ms_eServerCommand = enmServerCommand_None;
ServerStatus CMainServer::ms_eServerStatus = enmServerStatus_None;

CMainServer::CMainServer()
{

}

CMainServer::~CMainServer()
{

}

//初始化服务器
int32_t CMainServer::Initialize()
{
#ifdef WIN32
	//初始化winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	//初始化随机种子
	srand((unsigned)time(NULL));

//	CSimpleLog& logger = GET_SIMPLELOG_INSTANCE();

	//初始化LibFrame
	CREATE_MAINFRAME_INSTANCE();
	int32_t ret = g_Frame.Initialize();
	if(ret < 0)
	{
		return ret;
	}
	ret = PreStart();

	return ret;
}

int32_t CMainServer::PreStart()
{
	static int32_t i = 0;
	i++;
	if (i>=2)
	{
		WRITE_ERROR_LOG("PreStart had been called more than two times!");
	}

	//每3秒检查一次
//	int32_t nTimerIndex = enmInvalidTimerIndex;
	int32_t ret = S_OK;
//	ret = g_Frame.SetTimer(&g_TestTimer, enmTestTimePeriod * US_PER_SECOND, true, nTimerIndex);
//	if (ret < 0)
//	{
//		return ret;
//	}
//
//	 g_Frame.OpenOnlinePlayerMgt(&g_OnlineHandler);

	 //todo : add your code to here

	return ret;
}

//恢复服务器
int32_t CMainServer::Resume()
{
#ifdef WIN32
	//初始化winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	//初始化随机种子
	srand((unsigned)time(NULL));

	//初始化LibFrame
	g_Frame.Resume();

	return S_OK;
}

//销毁服务器
int32_t CMainServer::Uninitialize()
{
	//销毁LibFrame

#ifdef WIN32
	WSACleanup();
#endif

	//g_Frame().Terminate();
	return S_OK;
}


//重新加载配置文件
int32_t CMainServer::ReloadXmls()
{
	//通知LibFrame重新加载配置文件
	g_Frame.Reload();
	return S_OK;
}


//启动服务器
int32_t CMainServer::Start()
{
	//启动LibFrame
	g_Frame.Start();
	return S_OK;
}

//服务器开始执行
void CMainServer::Run()
{
	ServerCommand eServerCommand = enmServerCommand_None;

	while (true)
	{
		eServerCommand = ms_eServerCommand;

		if (enmServerCommand_Reload == eServerCommand)
		{
			//WriteLog(LOGPREFIX, enmLogLevel_Notice, "Activity server received reload signal!\n");
			ms_eServerCommand = enmServerCommand_None;
			ReloadXmls() ;
		}
		else if (enmServerCommand_Terminate == eServerCommand)
		{
			//WriteLog(LOGPREFIX, enmLogLevel_Notice, "Activity server received terminate signal!\n");

			//通知LibFrame结束运行
			g_Frame.Terminate();
			break;
		}

		if (g_bTest)
		{
//			CTestMain::OnTestRequest();
		}
		//TODO:
		Delay(100000);
	}
}

//重新加载配置文件
void CMainServer::Reload()
{
	ms_eServerCommand = enmServerCommand_Reload;
}

//终止服务器
void CMainServer::Terminate()
{
	ms_eServerCommand = enmServerCommand_Terminate;
}

//获取服务器状态
ServerStatus CMainServer::GetServerStatus()
{
	return ms_eServerStatus;
}

//设置服务器状态
void CMainServer::SetServerStatus(ServerStatus eServerStatus)
{
	ms_eServerStatus = eServerStatus;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

