/*
 * bll_event_enterroomnotice.cpp
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#include "bll_event_enterroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/server_message.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CEnterRoomNoticeMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody == NULL || pMsgHead == NULL || pSession == NULL)
	{
		WRITE_ERROR_LOG("pMsgBody null or pMsgHead null or pSession NULL ret=0x%08x \n",E_NULLPOINTER);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
		default:
			WRITE_ERROR_LOG("EnterRoomNotice:get session event");
		break;
	}

	return S_OK;
}

int32_t CEnterRoomNoticeMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	WRITE_DEBUG_LOG("enter room notice session time out");
	if(pSession == NULL)
	{
		WRITE_ERROR_LOG("pSession NULL ret=0x%08x \n",E_NULLPOINTER);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
		default:
			WRITE_ERROR_LOG("EnterRoomNotice:session time out");
		break;
	}

	return S_OK;
}

int32_t CEnterRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CEnterRoomNotice *pEnterRoomNotice = dynamic_cast<CEnterRoomNotice *>(pMsgBody);
	if(NULL == pEnterRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:{pEnterRoomNotice=0x%08x, nRleID=%d, nRoomID=%d}\n",pEnterRoomNotice,pMsgHead->nRoleID,pMsgHead->nRoomID);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("recv EnterRoomNotice{RoomID=%d,RoleID=%d}\n", pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);

	CRoom *pRoom = NULL;
	int32_t ret = g_RoomMgt.GetRoom(pEnterRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		WRITE_ERROR_LOG("It's not found room object!{nRoomID=%d, nServerID=%d, nRoleID=%d}\n", pEnterRoomNotice->nRoomID, pEnterRoomNotice->nServerID, pEnterRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}

	CPlayer *pPlayer = NULL;
	ret = g_PlayerMgt.GetPlayer(pEnterRoomNotice->nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_NOTICE_LOG("It's not found player object!{nRoomID=%d,nRoleID=%d}\n", pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
		if(enmPlayerType_Rebot == pEnterRoomNotice->nPlayerType)
		{
			WRITE_NOTICE_LOG("but this is a robot, so we'll create it!{nRoleID=%d}\n", pEnterRoomNotice->nRoleID);
			ret = g_PlayerMgt.CreatePlayer(pEnterRoomNotice->nRoleID, pPlayer);
			if(ret < 0 || pPlayer == NULL)
			{
				WRITE_ERROR_LOG("create robot object failed!{nRoleID=%d, ret=0x%08x}\n", pEnterRoomNotice->nRoleID, ret);
				return E_NULLPOINTER;
			}
		}
	}

	if(NULL == pPlayer)
	{
		WRITE_ERROR_LOG("recv roleid enterRoomNotice, but it's not foud player object!{nRoleID=%d}\n", pEnterRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}

	pPlayer->SetRoleID(pEnterRoomNotice->nRoleID);
	pPlayer->SetPlayerType(pEnterRoomNotice->nPlayerType);
	pPlayer->SetRoleName(pEnterRoomNotice->strRoleName.GetString());
	pPlayer->SetAccountID(pEnterRoomNotice->nAccountID);

	g_RoomMgt.AddPlayer(pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END
