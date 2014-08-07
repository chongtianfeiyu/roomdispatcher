/*
 * bll_event_online.cpp
 *
 *  Created on: 2012-2-13
 *      Author: jimm
 */

#include "bll_event_online.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "public_typedef.h"
#include "hallserver_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t COnlineMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}
int32_t COnlineMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}
int32_t COnlineMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	COnlineNoti *pOnlineNoti = dynamic_cast<COnlineNoti *>(pMsgBody);
	if(NULL == pOnlineNoti)
	{
		WRITE_ERROR_LOG("null pointer:{pOnlineNoti=0x%08x, nRoleID=%d}\n",pOnlineNoti,pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_NOTICE_LOG("oh,shit!recv roleid online noti{nRoleID=%d}",pMsgHead->nRoleID);

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_DEBUG_LOG("It's not found player object, so we'll create it!{nRoomID=%d, nRoleID=%d}\n", pMsgHead->nRoomID, pMsgHead->nRoleID);

		ret = g_PlayerMgt.CreatePlayer(pMsgHead->nRoleID, pPlayer);
		if(ret < 0 || pPlayer == NULL)
		{
			WRITE_ERROR_LOG("create player object failed!{nRoomID=%d, nRoleID=%d, ret=0x%08x}\n", pMsgHead->nRoomID, pMsgHead->nRoleID, ret);
			return E_NULLPOINTER;
		}
	}

	pPlayer->SetRoleName(pOnlineNoti->strRoleName.GetString());
	pPlayer->SetAccountID(pOnlineNoti->nAccountID);
	pPlayer->SetVipLevel(pOnlineNoti->nVipLevel);
//	pPlayer->SetPlayerMoney(pOnlineNoti->nLeftMoney);
	pPlayer->SetLoginTime(pOnlineNoti->nLoginTime);
	pPlayer->SetConnInfo(pOnlineNoti->nConnUin);

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

