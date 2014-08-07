/*
 *  bll_event_rebulidok_notice.cpp
 *
 *  To do：
 *  Created on: 	2012-5-16
 *  Author: 		luocj
 */

#include "bll_event_rebulidok_notice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/server_message.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CRebulidOKNoticeMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
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
			WRITE_ERROR_LOG("RebulidOKNotice:get exit room session");
		break;
	}

	return S_OK;
}

int32_t CRebulidOKNoticeMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	WRITE_DEBUG_LOG("exit room notice session time out");
	if(pSession == NULL)
	{
		WRITE_ERROR_LOG("pSession NULL ret=0x%08x \n",E_NULLPOINTER);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
		default:
			WRITE_ERROR_LOG("RebulidOKNotice:session time out");
		break;
	}

	return S_OK;
}

int32_t CRebulidOKNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CRebulidSuccessNoti *pRebulidSuccessNoti = dynamic_cast<CRebulidSuccessNoti *>(pMsgBody);
	if(NULL == pRebulidSuccessNoti)
	{
		WRITE_ERROR_LOG("null pointer:{pRebulidSuccessNoti=0x%08x, nRoomID=%d}\n",pRebulidSuccessNoti,pMsgHead->nRoomID);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("recv RebulidOKNotice{RoomID=%d, nServerID=%d}\n", pRebulidSuccessNoti->nRoomID,pMsgHead->nSourceID);

	CRoomCache *pRoomCache = NULL;
	ServerID nRoomServerID = enmInvalidServerID;
	int32_t ret = g_RoomCacheMgt.GetRoomCache(pRebulidSuccessNoti->nRoomID, pRoomCache);
	if(ret < 0 || pRoomCache == NULL) //表示没有创建房间
	{
		WRITE_ERROR_LOG("get roomcache failed{roomid=%d}\n",pRebulidSuccessNoti->nRoomID);
		return E_NULLPOINTER;
	}
	//设置房间恢复完成
	pRoomCache->SetRebulidSuccess();
	//回复在等待的获取roomserverid的请求
	uint32_t nWaitCount = pRoomCache->m_arrReqDate.size();
	for(uint32_t i = 0;i<nWaitCount;i++)
	{
		MessageHeadSS stHead = pRoomCache->m_arrReqDate[i].stHead;
		CGetRoomServerIDReq  stGetRSIDReq = pRoomCache->m_arrReqDate[i].stGetRSIDReq;
		CGetRoomServerIDResp stResp;
		stResp.nRoleID = stGetRSIDReq.nRoleID;
		stResp.nRoomID = stGetRSIDReq.nRoomID;
		stResp.nResult = S_OK;
		stResp.nServerID = pRoomCache->GetServerID();
		stResp.nConnIndex = stGetRSIDReq.nConnIndex;

		SendMessageResponse(MSGID_HSTS_GETROOMSERVERID_RESP, &stHead, &stResp);
	}
	pRoomCache->m_arrReqDate.clear();
	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END
