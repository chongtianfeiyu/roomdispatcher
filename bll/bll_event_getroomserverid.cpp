/*
 * bll_event_getroomserverid.cpp
 *
 *  Created on: 2011-12-6
 *      Author: jimm
 */

#include "bll_event_getroomserverid.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"
#include "dal/server_message.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CGetRoomServerIDMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}
int32_t CGetRoomServerIDMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}
int32_t CGetRoomServerIDMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CGetRoomServerIDReq *pGetRSIDMsg = dynamic_cast<CGetRoomServerIDReq *>(pMsgBody);
	if(NULL == pGetRSIDMsg)
	{
		WRITE_ERROR_LOG("null pointer:{pGetRSIDMsg=0x%08x, nRoomID=%d}\n",pGetRSIDMsg,pMsgHead->nRoomID);
		return E_NULLPOINTER;
	}
	WRITE_DEBUG_LOG("oh,shit!recv tunnelserver get roomserver id request{nRoleID=%d,nRoomID=%d}\n",pGetRSIDMsg->nRoleID, pGetRSIDMsg->nRoomID);

	CGetRoomServerIDResp stResp;
	stResp.nRoleID = pGetRSIDMsg->nRoleID;
	stResp.nRoomID = pGetRSIDMsg->nRoomID;

	CRoomCache *pRoomCache = NULL;
	ServerID nRoomServerID = enmInvalidServerID;
	int32_t ret = g_RoomCacheMgt.GetRoomCache(pGetRSIDMsg->nRoomID, pRoomCache);
	if(ret < 0 || pRoomCache == NULL) //表示没有创建房间
	{
		nRoomServerID = g_DataCenter.GetIdleRoomServerID();
		if(nRoomServerID != enmInvalidServerID)
		{
			//分配了一个roomserver
			ret = g_RoomCacheMgt.CreateRoomCache(pGetRSIDMsg->nRoomID, pRoomCache);
			if(ret < 0 || pRoomCache == NULL)
			{
				//todo :
				WRITE_ERROR_LOG("getroomserverid:createroomCache error{nRoleID=%d, nRoomID=%d}",pGetRSIDMsg->nRoleID, pGetRSIDMsg->nRoomID);
				nRoomServerID = enmInvalidServerID;
			}
			else
			{
				pRoomCache->SetRoomID(pMsgHead->nRoomID);
				pRoomCache->SetServerID(nRoomServerID);
			}
		}
	}
	else
	{
		//如果正在恢复，则等恢复好之后的通知
		if(pRoomCache->IsRebulid())
		{
			CReqDate stCReqDate;
			memcpy(&stCReqDate.stHead, pMsgHead, sizeof(stCReqDate.stHead));
			memcpy(&stCReqDate.stGetRSIDReq, pGetRSIDMsg, sizeof(stCReqDate.stGetRSIDReq));
			pRoomCache->m_arrReqDate.push_back(stCReqDate);
			//保存太多的时候删除
			if(pRoomCache->m_arrReqDate.size()>MaxUserCountPerRoom)
			{
				pRoomCache->m_arrReqDate.clear();
			}
			WRITE_NOTICE_LOG("room is rebulid on other roomserver {nRoomID=%d, nServerID=%d}!",pGetRSIDMsg->nRoomID,pRoomCache->GetServerID());
			return S_OK;
		}
		nRoomServerID = pRoomCache->GetServerID();
	}

	stResp.nResult = ret;
	stResp.nServerID = nRoomServerID;
	stResp.nConnIndex = pGetRSIDMsg->nConnIndex;

	SendMessageResponse(MSGID_HSTS_GETROOMSERVERID_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);
	WRITE_DISTRIBUTED_NOTICE_LOG("Room:%d  RoomServerID:%d",pGetRSIDMsg->nRoomID,nRoomServerID);

	return S_OK;
}


FRAME_ROOMDISPATCHER_NAMESPACE_END


