/*
 * bll_base.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include "bll_base.h"
#include "public_typedef.h"
#include "ctl/server_datacenter.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

static ConnUin g_arrConnUin[MaxUserCountPerRoom];

/*
	发送请求消息
*/
void CBllBase::SendMessageRequest(uint32_t nMsgID, IMsgBody* pMsgBody, const RoomID nRoomID, TransType nTransType,
		RoleID nRoleID, EntityType nDestType, const int32_t nDestID, const SessionIndex nSessionIndex,
		const uint16_t nOptionLen, const char *pOptionData, const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Request;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = nDestType;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = nDestID;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = nSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nZoneID = 0;
	stSendMessage.nRouterIndex = 0;

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent, &stSendMessage, pMsgBody, nOptionLen, pOptionData);
}
/*
	发送相应消息
*/
void CBllBase::SendMessageResponse(uint32_t nMsgID,MessageHeadSS * pMsgHead,IMsgBody* pMsgBody,
		TransType nTransType, const uint16_t nOptionLen,
		const char *pOptionData,const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
	memset(&stSendMessage, 0, sizeof(stSendMessage));

	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Response;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = pMsgHead->nSourceType;
	stSendMessage.nSourceID =  g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = pMsgHead->nSourceID;
	stSendMessage.nRoleID = pMsgHead->nRoleID;
	stSendMessage.nSequence = pMsgHead->nSequence;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = pMsgHead->nSessionIndex;
	stSendMessage.nRoomID = pMsgHead->nRoomID;
	stSendMessage.nZoneID = pMsgHead->nZoneID;
	if(stSendMessage.nDestType == enmEntityType_Tunnel)
	{
		if(nOptionLen > 0)
		{
			ConnUin stInfo;
			uint32_t offset = 0;
			int32_t ret = stInfo.MessageDecode((uint8_t *)pOptionData, (uint32_t)nOptionLen, offset);
			if(ret < 0)
			{
				WRITE_ERROR_LOG("send message response:decode connuin failed!{ret=0x%08x, nRoleID=%d}\n", ret, pMsgHead->nRoleID);
				return;
			}
			stSendMessage.nRouterIndex = g_Frame.GetRouterIndex(stInfo.nRouterAddress, stInfo.nRouterPort);
		}
		else
		{
			CPlayer *pPlayer = NULL;
			int32_t ret = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID, pPlayer);
			if(ret < 0)
			{
				WRITE_ERROR_LOG("get expect player object error!{RoomID=%d, ret=0x%08x}\n", pMsgHead->nRoomID, ret);
				return;
			}
			stSendMessage.nRouterIndex = g_Frame.GetRouterIndex(pPlayer->GetConnInfo().nRouterAddress, pPlayer->GetConnInfo().nRouterPort);
		}
	}
	else
	{
		stSendMessage.nRouterIndex = pMsgHead->nRouterIndex;
	}

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage, pMsgBody, nOptionLen, pOptionData);
}

void CBllBase::SendMessageNotifyToServer(uint32_t nMsgID,IMsgBody* pMsgBody,const RoomID nRoomID,const TransType nTransType,
		const RoleID nRoleID,const EntityType nDestType,const int32_t nDestID,
		const uint16_t nOptionLen, const char *pOptionData,const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = nDestType;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = nDestID;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nZoneID = g_Frame.GetZoneID(enmRouterType_Sync);
	stSendMessage.nRouterIndex = 0;

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
}

/*
	发送通知消息
*/
void CBllBase::SendMessageNotify(const RoleID nRoleID,  IMsgBody* pMsgBody,uint32_t nMsgID,
		const char *szDumpContent, const RoomID nRoomID, const uint16_t nOptionLen, const char *pOptionData)
{
	MessageHeadSS stSendMessage;
	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = enmEntityType_Hall;
	stSendMessage.nSourceID =  g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = 0;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = enmTransType_ByKey;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
}

//发送广播消息给房间内所有玩家
void CBllBase::SendMessageNotifyToClient(uint32_t nMsgID, IMsgBody* pMsgBody, RoomID nRoomID, BroadcastType nType,
		RoleID nRoleID, const uint16_t nOptionLen,const char *pOptionData, const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = enmEntityType_Tunnel;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	//	stSendMessage.nDestID = 0;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = enmTransType_P2P;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nRoleID = enmInvalidRoleID;

	uint32_t nPlayerOptionLen = 0;
	char arrPlayerOptionData[MaxOptionDataSize];

	int32_t ret = S_OK;
	if(enmBroadcastType_ExpectPlayr == nType)
	{
		CPlayer *pPlayer = NULL;
		ret = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
		if(ret < 0)
		{
			WRITE_ERROR_LOG("get expect player object error!{nRoleID=%d, ret=0x%08x}\n", nRoleID, ret);
			return;
		}
		stSendMessage.nRoleID = nRoleID;
		stSendMessage.nDestID = pPlayer->GetConnInfo().nServerID;
		stSendMessage.nRouterIndex = g_Frame.GetRouterIndex(pPlayer->GetConnInfo().nRouterAddress, pPlayer->GetConnInfo().nRouterPort);

		g_Frame.PostMessage(&stSendMessage, pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
		DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
	}
	else if(enmBroadcastType_ExceptPlayr == nType)
	{
		stSendMessage.nTransType = enmTransType_Broadcast;
		stSendMessage.nDestID = enmInvalidServerID;

		stSendMessage.nRoleID = nRoleID;
		stSendMessage.nRouterIndex = 0;

		g_Frame.PostMessage(&stSendMessage, pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
		DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
	}
	else if(enmBroadcastType_All == nType)
	{
		stSendMessage.nTransType = enmTransType_Broadcast;
		stSendMessage.nDestID = enmInvalidServerID;
		stSendMessage.nRouterIndex = 0;

		g_Frame.PostMessage(&stSendMessage, pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
		DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
	}
	else if(enmBroadcastType_ExceptRoom == nType)
	{
		int32_t nRoomCount = 0;
		CRoom *arrRoomObject[MaxRoomCount];

		g_RoomMgt.GetAllRoom(arrRoomObject, MaxRoomCount, nRoomCount);

		for(int32_t i = 0; i < nRoomCount; ++i)
		{
			if(arrRoomObject[i] == NULL || nRoomID != arrRoomObject[i]->GetRoomID())
			{
				continue;
			}

			stSendMessage.nTransType = enmTransType_Broadcast;
			stSendMessage.nDestID = enmInvalidServerID;

			stSendMessage.nRoomID = arrRoomObject[i]->GetRoomID();

			g_Frame.PostMessage(&stSendMessage, pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
			DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nPlayerOptionLen, arrPlayerOptionData);
		}
	}
}

void CBllBase::SendMessageNotify(const RoleID nRoleID, const EntityType nDestType ,
		const TransType nTransType, IMsgBody* pMsgBody,uint32_t nMsgID,const char *szDumpContent,
		const RoomID nRoomID, const uint16_t nOptionLen, const char *pOptionData)
{
	MessageHeadSS stSendMessage;
	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = nDestType;
	stSendMessage.nSourceID =  g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = 0;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
}

/*
	打印接受，发送的消息
*/
void CBllBase::DumpMessage(const char* szContent, MessageHeadSS *pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const char *pOptionData)
{
	uint32_t offset = 0;
	char szLog[enmMaxLogInfoLength];
	szLog[0] = 0;
	sprintf(szLog + offset, szContent);
	offset = (uint32_t)strlen(szLog);

	sprintf(szLog + offset, " MessageHead=");
	offset = (uint32_t)strlen(szLog);

	pMsgHead->Dump(szLog, enmMaxLogInfoLength, offset);

	sprintf(szLog + offset, " MessageBody=");
	offset = (uint32_t)strlen(szLog);
	pMsgBody->Dump(szLog, enmMaxLogInfoLength, offset);

	WRITE_NOTICE_LOG("%s\n", szLog);
}

int32_t CBllBase::SendEvent(const RoleID roleID, uint32_t nMsgID, IMsgBody* pMsgBody)
{
	return S_OK;

}


FRAME_ROOMDISPATCHER_NAMESPACE_END

