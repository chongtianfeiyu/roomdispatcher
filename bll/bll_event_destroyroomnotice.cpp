/*
 * bll_event_destroyroomnotice.cpp
 *
 *  Created on: 2012-4-24
 *      Author: jimm
 */

#include "bll_event_destroyroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CDestroyRoomNoticeMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}

int32_t CDestroyRoomNoticeMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}

int32_t CDestroyRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CDestroyRoomNotice *pDestroyRoomNotice = dynamic_cast<CDestroyRoomNotice *>(pMsgBody);
	if(NULL == pDestroyRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:{pDestroyRoomNotice=0x%08x, nRoomID=%d}\n",pDestroyRoomNotice,pMsgHead->nRoomID);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("recv DestroyRoomNotice{RoomID=%d,ServerID=%d}\n", pDestroyRoomNotice->nRoomID, pDestroyRoomNotice->nServerID);

	CRoom *pRoom = NULL;
	int32_t ret = g_RoomMgt.GetRoom(pDestroyRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		WRITE_DEBUG_LOG("It's not found room object by roomid, so we'll do nothing!{nRoomID=%d, nServerID=%d}\n", pDestroyRoomNotice->nRoomID, pDestroyRoomNotice->nServerID);
	}

	WRITE_DEBUG_LOG("It's found room object by roomid, so we'll destory it!{nRoomID=%d, nServerID=%d}\n", pDestroyRoomNotice->nRoomID, pDestroyRoomNotice->nServerID);

	ret = g_RoomMgt.DestroyRoom(pDestroyRoomNotice->nRoomID);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("destroy room object failed by roomid!{nRoomID=%d, nServerID=%d, ret=0x%08x}\n", pDestroyRoomNotice->nRoomID, pDestroyRoomNotice->nServerID, ret);
	}

	//销毁相对应的房间cache数据
	ret = g_RoomCacheMgt.DestoryRoomCache(pDestroyRoomNotice->nRoomID);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("destroy room object failed by roomid!{nRoomID=%d, nServerID=%d,ret=0x%08x}\n", pDestroyRoomNotice->nRoomID, pDestroyRoomNotice->nServerID, ret);
	}

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END
