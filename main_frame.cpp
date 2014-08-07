/*
 * main_frame.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include "main_frame.h"
#include "frame_eventid.h"

#include "ctl/server_datacenter.h"
#include "def/def_message_id.h"
//#include "def/def_message_body.h"
#include "dal/server_message.h"
#include "roomserver_message_define.h"
#include "hallserver_message_define.h"
#include "def/dal_def.h"
#include "bll/bll_event_getroomserverid.h"
#include "bll/bll_event_updateinfonotice.h"
#include "bll/bll_event_getmediainfo.h"
#include "main_frame.h"
#include "bll/bll_event_updateviplevelnotice.h"
#include "bll/bll_event_updatedatanotice.h"
#include "bll/bll_event_createroomnotice.h"
#include "bll/bll_event_destroyroomnotice.h"
#include "bll/bll_event_enterroomnotice.h"
#include "bll/bll_event_exitroomnotice.h"
#include "bll/bll_event_online.h"
#include "bll/bll_event_offline.h"
#include "daemon_message_define.h"
#include "bll/bll_event_serverstartnotice.h"
#include "bll/bll_event_serverstopnotice.h"
#include "bll/bll_event_rebulidok_notice.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

CMainFrame::CMainFrame()
:CLightFrame(SERVER_NAME_STRING)
{

}

CMainFrame::~CMainFrame()
{
}

//框架初始话
int32_t CMainFrame::Initialize()
{
	int32_t ret=S_OK;
	//添加sever自己需要的数据中心
	ret=AddDataCenter(&GET_DALDATACENTER_INSTANCE());
	if(ret<0)
	{
		return ret;
	}

	ret = CLightFrame::Initialize();
	if(0 > ret)
	{
		return ret;
	}
	//注册所有的消息
	RegistMsg();

	g_FrameLogEngine.RegistLog("server_distributed");

	return S_OK;
}

//恢复框架
int32_t CMainFrame::Resume()
{
	return CLightFrame::Resume();

}
//销毁框架
int32_t CMainFrame::Terminate()
{
	g_eServerCommand = enmServerCommand_Terminate;
	return S_OK;

}
void CMainFrame::RegistMsg()
{
	//注册服务器启动事件通知
	RegistSysEvent(SYS_EVENT_REGIST_SERVER, &GET_SERVERSTARTNOTICEEVENT_INSTANCE());

	//注册断线通知事件
	RegistSysEvent(SYS_EVENT_DISCONNECT_SERVER, &GET_SERVERSTOPNOTICEEVENT_INSTANCE());

	INITIALIZE_MESSAGEMAP_DECL(public_message);
	INITIALIZE_MESSAGEMAP_DECL(roomdispatcher_from_server);

	RegistMsgHandler(MSGID_TSHS_GETROOMSERVERID_REQ, &CREATE_GETROOMSERVERIDEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSHS_UPDATEINFOTOHALL_NOTICE, &CREATE_UPDATEINFOTOHALLEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSHS_GETMEDIAINFO_REQ, &CREATE_GETMEDIAINFOEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSMS_ENTER_ROOM_NOTICE, &CREATE_ENTERROOMNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSMS_EXIT_ROOM_NOTICE, &CREATE_EXITROOMNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSRD_UPDATEDATA_NOTI, &CREATE_UPDATEDATANOTICEEVENT_INSTANCE());

	INITIALIZE_MESSAGEMAP_DECL(roomserver);

	//处理sysinfo发来的更新viplevel通知
	RegistMsgHandler(MSGID_SSRS_UPDATELEVEL_NOTI, &CREATE_UPDATEVIPNOTIEVENT_INSTANCE());

	RegistMsgHandler(MSGID_RSMS_CREATE_ROOM_NOTICE, &CREATE_CREATEROOMNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSMS_DESTORY_ROOM_NOTICE, &CREATE_DESTROYROOMNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSTA_REBULIDSUCCESS_NOTIFY, &CREATE_REBULIDOKNOTICEEVENT_INSTANCE());

	INITIALIZE_MESSAGEMAP_DECL(hallserver);
	RegistMsgHandler(MSGID_HSTA_ONLINE_NOTI, &CREATE_ONLINEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_HSTA_OFFLINE_NOTI, &CREATE_OFFLINEEVENT_INSTANCE());

	INITIALIZE_MESSAGEMAP_DECL(daemonserver);
	RegistMsgHandler(MSGID_DAEMON_START_NOTIFY, &CREATE_SERVERSTARTNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_DAEMON_STOP_NOTIFY, &CREATE_SERVERSTOPNOTICEEVENT_INSTANCE());
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

