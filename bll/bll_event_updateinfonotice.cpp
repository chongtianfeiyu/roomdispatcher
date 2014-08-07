/*
 * bll_event_updateinfonotice.cpp
 *
 *  Created on: 2012-1-10
 *      Author: jimm
 */

#include "bll_event_updateinfonotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CUpdateInfoToHallMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}

int32_t CUpdateInfoToHallMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}

int32_t CUpdateInfoToHallMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CUpdateInfoToHallNotice *pUpdateInfo = dynamic_cast<CUpdateInfoToHallNotice *>(pMsgBody);
	if(NULL == pUpdateInfo)
	{
		WRITE_ERROR_LOG("null pointer:{pUpdateInfo=0x%08x}\n",pUpdateInfo);
		return E_NULLPOINTER;
	}

	switch(pMsgHead->nSourceType)
	{
		case enmEntityType_Room:
		OnRoomUpdateInfo(pMsgHead, pUpdateInfo);
		break;
		case enmEntityType_Media:
		OnMediaUpdateInfo(pMsgHead, pUpdateInfo);
		break;
		default:
		WRITE_ERROR_LOG("unknown update info!{nEntityType=%d, nServerID=%d}\n", pMsgHead->nSourceType, pMsgHead->nSourceID);
		return S_OK;
	}
	int32_t nPlayerCountOnServer = 0;
	for(int32_t i = 0;i < pUpdateInfo->nRoomCount;i++)
	{
		nPlayerCountOnServer += pUpdateInfo->arrPlayerCount[i];
	}
	g_DataCenter.UpdateServerInfo(pMsgHead->nSourceType, pMsgHead->nSourceID, pUpdateInfo->nRoomCount, nPlayerCountOnServer,pUpdateInfo->nRecvPacketsPerTenMins,pUpdateInfo->nServerNetType);

	return S_OK;
}

int32_t CUpdateInfoToHallMessageEvent::OnRoomUpdateInfo(MessageHeadSS *pMsgHead, CUpdateInfoToHallNotice *pUpdateInfo)
{
	int32_t ret = S_OK;
	for(int32_t i = 0; i < pUpdateInfo->nRoomCount; ++i)
	{
		if(pUpdateInfo->arrRoomID[i]==enmInvalidRoomID)
		{
			continue;
		}
		//检查roomcache
		CRoomCache *pRoomCache = NULL;
		int32_t ret = g_RoomCacheMgt.GetRoomCache(pUpdateInfo->arrRoomID[i], pRoomCache);
		if(ret < 0 || pRoomCache == NULL)
		{
			ret = g_RoomCacheMgt.CreateRoomCache(pUpdateInfo->arrRoomID[i], pRoomCache);
			if(ret < 0 || pRoomCache == NULL)
			{
				WRITE_ERROR_LOG("update room info:createroomCache error{nRoomID=%d, ret=0x%08x}", pUpdateInfo->arrRoomID[i],ret);
				continue;
			}
		}
		pRoomCache->SetRoomID(pUpdateInfo->arrRoomID[i]);
		pRoomCache->SetServerID(pMsgHead->nSourceID);

		CRoom *pRoom = NULL;
		ret = g_RoomMgt.GetRoom(pUpdateInfo->arrRoomID[i], pRoom);
		if(ret < 0 || pRoom == NULL)
		{
			WRITE_DEBUG_LOG("Updateinfo:room %d not exist", pUpdateInfo->arrRoomID[i]);
			ret = g_RoomMgt.CreateRoom(pUpdateInfo->arrRoomID[i], pRoom);
			if(ret < 0 || pRoom == NULL)
			{
				WRITE_ERROR_LOG("Updateinfo:create room error{nRoomID=%d, ret=0x%08x}", pUpdateInfo->arrRoomID[i],ret);
				continue;
			}
		}

		pRoom->SetRoomServerID(pMsgHead->nSourceID);

		//检测非法数据
		for(int32_t j = 0; j < pUpdateInfo->arrPlayerCount[i]; ++j)
		{
			if(pRoom->IsPlayerInRoom(pUpdateInfo->arrRoleID[i][j]))
			{
				continue;
			}
			else
			{
				CPlayer *pPlayer = NULL;
				ret = g_PlayerMgt.GetPlayer(pUpdateInfo->arrRoleID[i][j], pPlayer);
				if(0 > ret || pPlayer == NULL)
				{
					WRITE_NOTICE_LOG("Updateinfo:player object not exist creat it{nRoleID=%d}", pUpdateInfo->arrRoleID[i][j]);
					ret = g_PlayerMgt.CreatePlayer(pUpdateInfo->arrRoleID[i][j], pPlayer);
					if(pPlayer==NULL)
					{
						WRITE_ERROR_LOG("create player error{nRoleID=%d, ret=0x%08x}", pUpdateInfo->arrRoleID[i][j],ret);
						continue;
					}
				}

				g_RoomMgt.AddPlayer(pUpdateInfo->arrRoomID[i], pUpdateInfo->arrRoleID[i][j]);
			}
		}
	}

	return S_OK;
}

int32_t CUpdateInfoToHallMessageEvent::OnMediaUpdateInfo(MessageHeadSS *pMsgHead, CUpdateInfoToHallNotice *pUpdateInfo)
{
	int32_t ret = S_OK;
	//检查自己和part是否在serverloadmanger中
	ServerLoadManagerMap &stManagerMap = g_ServerLoadManager;
	if(!stManagerMap[enmEntityType_Media].IsFind(pMsgHead->nSourceID)||!stManagerMap[enmEntityType_Media].IsFind(pUpdateInfo->nPartServerID))
	{
		WRITE_ERROR_LOG("mediaserver or partserver is not start or stop!{nServerID=%d}",pMsgHead->nSourceID);
		return ret;
	}
	CMedia *pMedia = NULL;

	ret = g_MediaMgt.GetMedia(pMsgHead->nSourceID, pMedia);
	if(ret < 0 || pMedia == NULL)
	{
		ret = g_MediaMgt.CreateMedia(pMsgHead->nSourceID, pMedia);
		if(ret < 0 || pMedia == NULL)
		{
			WRITE_ERROR_LOG("creat media filed{nServerID=%d, ret=0x%08x}!!",pMsgHead->nSourceID);
			return ret;
		}
	}

	pMedia->SetServerID(pMsgHead->nSourceID);
	pMedia->SetServerAddress(pUpdateInfo->nServerAddress);
	pMedia->SetServerPort(pUpdateInfo->nServerPort);
	pMedia->SetNetType(pUpdateInfo->nServerNetType);
	pMedia->SetPartServerID(pUpdateInfo->nPartServerID);

	WRITE_DEBUG_LOG("mediaserverupdate ok ! {nServerID=%d, ip=%s, nPort=%d, nServerNetType=%d, nPartServerID=%d}\n", pMsgHead->nSourceID,
			inet_ntoa_f(pUpdateInfo->nServerAddress), pUpdateInfo->nServerPort, pUpdateInfo->nServerNetType,pUpdateInfo->nPartServerID);

	//如果房间数不一致
	if(pMedia->GetRoomCount() != pUpdateInfo->nRoomCount)
	{
		pMedia->DeleteAllRooms();
		for(int32_t i = 0; i < pUpdateInfo->nRoomCount; ++i)
		{
			pMedia->AddRoom(pUpdateInfo->arrRoomID[i]);
		}
	}

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

