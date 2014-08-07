/*
 * bll_event_createroomnotice.cpp
 *
 *  Created on: 2012-2-12
 *      Author: jimm
 */

#include "bll_event_createroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/server_message.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CCreateRoomNoticeMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}

int32_t CCreateRoomNoticeMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}

int32_t CCreateRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CCreateRoomNotice *pCreateRoomNotice = dynamic_cast<CCreateRoomNotice *>(pMsgBody);
	if(NULL == pCreateRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:{pCreateRoomNotice=0x%08x, nRoomID=%d}\n",pCreateRoomNotice,pMsgHead->nRoomID);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("recv CreateRoomNotice{RoomID=%d,ServerID=%d}\n", pCreateRoomNotice->nRoomID, pCreateRoomNotice->nServerID);

	CRoom *pRoom = NULL;
	int32_t ret = g_RoomMgt.GetRoom(pCreateRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		WRITE_DEBUG_LOG("It's not found room object by roomid, so we'll create it{nRoomID=%d, nServerID=%d}\n", pCreateRoomNotice->nRoomID, pCreateRoomNotice->nServerID);
		ret = g_RoomMgt.CreateRoom(pCreateRoomNotice->nRoomID, pRoom);
		if(ret < 0 || pRoom == NULL)
		{
			WRITE_ERROR_LOG("create room object failed by roomid!{nRoomID=%d, nServerID=%d, ret=0x%08x}\n", pCreateRoomNotice->nRoomID, pCreateRoomNotice->nServerID, ret);
			return E_NULLPOINTER;
		}
	}
	pRoom->SetRegionID(pCreateRoomNotice->nRegionID);
	pRoom->SetChannelID(pCreateRoomNotice->nChannelID);
	pRoom->SetRoomID(pCreateRoomNotice->nRoomID);
	pRoom->SetRoomName(pCreateRoomNotice->strRoomName.GetString());
	pRoom->SetRoomServerID(pCreateRoomNotice->nServerID);
	pRoom->SetHostID(pCreateRoomNotice->nHostID);

	CRoomCache *pRoomCache = NULL;
	ret = g_RoomCacheMgt.GetRoomCache(pCreateRoomNotice->nRoomID, pRoomCache);
	if(ret < 0 || pRoomCache == NULL) //表示没有创建房间
	{
		ret = g_RoomCacheMgt.CreateRoomCache(pCreateRoomNotice->nRoomID, pRoomCache);
		if(ret < 0 || pRoomCache == NULL)
		{
			WRITE_ERROR_LOG("create roomCache filed{nRoomID=%d, ret=0x%08x}",pMsgHead->nRoomID,pCreateRoomNotice->nRoomID);
			return E_NULLPOINTER;
		}
	}
	pRoomCache->SetRoomID(pMsgHead->nRoomID);
	pRoomCache->SetServerID(pMsgHead->nSourceID);

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

