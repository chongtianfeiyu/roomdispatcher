/*
 * bll_event_offline.cpp
 *
 *  Created on: 2012-2-13
 *      Author: jimm
 */

#include "bll_event_offline.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t COfflineMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}
int32_t COfflineMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}
int32_t COfflineMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	COfflineNoti *pOfflineNoti = dynamic_cast<COfflineNoti *>(pMsgBody);
	if(NULL == pOfflineNoti)
	{
		WRITE_ERROR_LOG("null pointer:{pOfflineNoti=0x%08x, nRoleID=%d}\n",pOfflineNoti,pMsgHead->nRoleID,pMsgHead->nRoomID);
		return E_NULLPOINTER;
	}

	WRITE_NOTICE_LOG("oh,shit!recv offline noti{nRoleID=%d}",pOfflineNoti->nRoleID);

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pOfflineNoti->nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("get player object failed{RoleID=%d, ret=0x%08x}\n",pOfflineNoti->nRoleID, ret);
	}
	else
	{
		RoomID arrRoomID[MaxEnterRoomCount];
		int32_t nRoomCount = 0;
		pPlayer->GetAllEnterRoom(arrRoomID, MaxEnterRoomCount, nRoomCount);
		for(int32_t i = 0; i < nRoomCount; ++i)
		{
			CRoom *pRoom = NULL;
			ret = g_RoomMgt.GetRoom(arrRoomID[i], pRoom);
			if(ret < 0 || pRoom == NULL)
			{
				continue;
			}

			g_RoomMgt.DeletePlayer(arrRoomID[i], pOfflineNoti->nRoleID);
		}

		g_PlayerMgt.DestroyPlayer(pOfflineNoti->nRoleID);
	}

	//销毁相对应的玩家cache数据
	for(int32_t i = 0; i < MaxRoomServerCountPerRoomDispatcher; ++i)
	{
		CPlayerCache *pPlayerCache = NULL;
		int32_t ret = g_PlayerCacheMgt(i)->GetPlayerCache(pOfflineNoti->nRoleID, pPlayerCache);
		if(ret < 0 || pPlayerCache == NULL)
		{
			continue;
		}

		g_PlayerCacheMgt(i)->DestoryPlayerCache(pOfflineNoti->nRoleID);
	}

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END
