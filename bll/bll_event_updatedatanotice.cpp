/*
 * bll_event_updatedatanotice.cpp
 *
 *  Created on: 2012-4-23
 *      Author: jimm
 */

#include "bll_event_updatedatanotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "roomdispatcher_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CUpdateDataNoticeMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}

int32_t CUpdateDataNoticeMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}

int32_t CUpdateDataNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CUpdateDataNoti *pUpdateDataNoti = dynamic_cast<CUpdateDataNoti *>(pMsgBody);
	if(NULL == pUpdateDataNoti)
	{
		WRITE_ERROR_LOG("null pointer:{pUpdateDataNoti=0x%08x}\n",pUpdateDataNoti);
		return E_NULLPOINTER;
	}

	switch(pUpdateDataNoti->nType)
	{
	case enmUpdateDataType_Room:
		UpdateRoomCache(pMsgHead, pUpdateDataNoti);
		break;
	case enmUpdateDataType_Player:
		UpdatePlayerCache(pMsgHead, pUpdateDataNoti);
		break;
	default:
		break;
	}

	return S_OK;
}

void CUpdateDataNoticeMessageEvent::UpdateRoomCache(MessageHeadSS * pMsgHead, CUpdateDataNoti* pUpdateDataNoti)
{
	if(pMsgHead->nSourceType != enmEntityType_Room)
	{
		return;
	}

	CRoomCache *pRoomCache = NULL;
	int32_t ret = g_RoomCacheMgt.GetRoomCache(pMsgHead->nRoomID, pRoomCache);
	if(ret < 0 || pRoomCache == NULL)
	{
		return;
	}

	ret = pRoomCache->UpdateRoomCache(pUpdateDataNoti->nStartPos, pUpdateDataNoti->nEndPos,
			pUpdateDataNoti->nDataOffset, pUpdateDataNoti->nDataSize, pUpdateDataNoti->arrData);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("update room data has error{nRoomID=%d, ret=0x%08x}",pMsgHead->nRoomID,ret);
	}
}

void CUpdateDataNoticeMessageEvent::UpdatePlayerCache(MessageHeadSS * pMsgHead, CUpdateDataNoti* pUpdateDataNoti)
{
	if(pMsgHead->nSourceType != enmEntityType_Room)
	{
		//todo: error log
		return;
	}

	CPlayerCache *pPlayerCache = NULL;
	int32_t ret = g_PlayerCacheMgt(pMsgHead->nSourceID)->GetPlayerCache(pUpdateDataNoti->nValue, pPlayerCache);
	if(ret < 0 || pPlayerCache == NULL)
	{
		ret = g_PlayerCacheMgt(pMsgHead->nSourceID)->CreatePlayerCache(pUpdateDataNoti->nValue, pPlayerCache);
		if(ret < 0 || pPlayerCache == NULL)
		{
			WRITE_ERROR_LOG("create palyerCache filed{nRoleID=%d, ret=0x%08x, playercache_count=%d}",pUpdateDataNoti->nValue,ret,g_PlayerCacheMgt(pMsgHead->nSourceID)->GetPlayerCacheCount());
			return;
		}

		pPlayerCache->SetRoleID(pMsgHead->nRoleID);
		pPlayerCache->SetServerID(pMsgHead->nSourceID);
	}

	ret = pPlayerCache->UpdatePlayerCache(pUpdateDataNoti->nStartPos, pUpdateDataNoti->nEndPos,
			pUpdateDataNoti->nDataOffset, pUpdateDataNoti->nDataSize, pUpdateDataNoti->arrData);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("update player data has error{nRoleID=%d, ret=0x%08x}",pUpdateDataNoti->nValue,ret);
	}

}

FRAME_ROOMDISPATCHER_NAMESPACE_END
