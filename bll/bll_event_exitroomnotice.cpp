/*
 * bll_event_exitroomnotice.cpp
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#include "bll_event_exitroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/server_message.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CExitRoomNoticeMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}

int32_t CExitRoomNoticeMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}

int32_t CExitRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CExitRoomNotice *pExitRoomNotice = dynamic_cast<CExitRoomNotice *>(pMsgBody);
	if(NULL == pExitRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:{pExitRoomNotice=0x%08x, nRoleID=%d, nRoomID=%d}\n",pExitRoomNotice,pMsgHead->nRoleID,pMsgHead->nRoomID);
		return E_NULLPOINTER;
	}

	CRoom *pRoom = NULL;
	int32_t ret = g_RoomMgt.GetRoom(pExitRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		WRITE_ERROR_LOG("It's not found room object!{nRoomID=%d, nRoleID=%d}\n", pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}

	CPlayer *pPlayer = NULL;
	ret = g_PlayerMgt.GetPlayer(pExitRoomNotice->nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("It's not found player object!{nRoomID=%d, nRoleID=%d}\n", pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}

	g_RoomMgt.DeletePlayer(pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);
	WRITE_NOTICE_LOG("player exit room{nRoomID=%d,nRoleID=%d}\n", pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);

	//如果玩家是机器人(没有下线通知)
	if(pPlayer->GetPlayerType() == enmPlayerType_Rebot && pPlayer->GetCurEnterRoomCount() <= 0)
	{
		//销毁玩家
		g_PlayerMgt.DestroyPlayer(pExitRoomNotice->nRoleID);
		//销毁playercache
		g_PlayerCacheMgt(pMsgHead->nSourceID)->DestoryPlayerCache(pExitRoomNotice->nRoleID);
	}

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

