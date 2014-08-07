/*
 * bll_event_updateviplevelnotice.cpp
 *
 *  Created on: 2012-3-15
 *      Author: zhurw
 */

#include "bll_event_updateviplevelnotice.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN
int32_t CUpdateVipNotiEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}
	CUpdateVipLevelNotify* pUpdateVipLevelNoti = dynamic_cast<CUpdateVipLevelNotify*>(pMsgBody);
	switch(pMsgHead->nMessageID)
	{
		case MSGID_SSRS_UPDATELEVEL_NOTI:
		{
			WRITE_DEBUG_LOG("recieve update vip notify{roleid=%d,viplevel=%d}",pUpdateVipLevelNoti->nRoleID, pUpdateVipLevelNoti->nVipLevel);
			nRet = UpdateMemVipLevel(pUpdateVipLevelNoti->nRoleID, pUpdateVipLevelNoti->nVipLevel);
			if(0 > nRet)
			{
				WRITE_ERROR_LOG("UpdateVipLevel error");
				return E_UNKNOWN;
			}
		}
		break;
		default:
		break;
	}
	return S_OK;
}
int32_t CUpdateVipNotiEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody, CFrameSession* pSession,
		const uint16_t nOptionLen, const void *pOptionData)
{

	return S_OK;
}
int32_t CUpdateVipNotiEvent::OnSessionTimeOut(CFrameSession *pSession)
{

	return S_OK;
}

int32_t CUpdateVipNotiEvent::UpdateMemVipLevel(RoleID nRoleID, VipLevel nVipLevel)
{
	int32_t nRet = S_OK;
	CPlayer* pPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
	if(0 > nRet || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("update vip level:get player error{nRet=%d}", nRet);
		return E_NULLPOINTER;
	}
	if(nVipLevel > enmVipLevel_TIP)
	{
		return S_OK;
	}
	pPlayer->SetVipLevel(nVipLevel);
	WRITE_NOTICE_LOG("update vip level:set player as viplevel{nRoleID=%d, nVipLevel=%d}", nRoleID, nVipLevel);
	return S_OK;
}
FRAME_ROOMDISPATCHER_NAMESPACE_END
