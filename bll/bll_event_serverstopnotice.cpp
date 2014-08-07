/*
 * bll_event_serverstopnotice.cpp
 *
 *  Created on: 2012-4-23
 *      Author: jimm
 */

#include "bll_event_serverstopnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "daemon_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN


int32_t CServerStopNoticeMessageEvent::OnSystemEvent(uint16_t nEventID, void *pParam)
{
	MessageHeadSS *pMsgHead = (MessageHeadSS *)pParam;
	if(NULL == pMsgHead)
	{
		WRITE_ERROR_LOG("null pointer:{nEventID=0x%08x}\n",nEventID);
		return E_NULLPOINTER;
	}

	WRITE_NOTICE_LOG("recv stop noti!{nServerType=%d, nServerID=%d}\n", pMsgHead->nSourceType, pMsgHead->nSourceID);

	switch(pMsgHead->nSourceType)
	{
		case enmEntityType_Room:
		{
			int32_t ret = RoomServerStop(pMsgHead->nSourceID,pMsgHead);
			if(ret<0)
			{
				WRITE_ERROR_LOG("rebulid roomserver error!{nServerID=%d}",pMsgHead->nSourceID);
				return ret;
			}
		}
		break;
		case enmEntityType_Media:
		{
			int32_t ret = MediaServerStop(pMsgHead->nSourceID,pMsgHead);
			if(ret<0)
			{
				WRITE_ERROR_LOG("rebulid MediaServer error!{nServerID=%d}",pMsgHead->nSourceID);
				return ret;
			}
		}
		break;
		default:
		break;
	}
	return S_OK;
}
//roomserver�������ʱ��Ļָ�
int32_t CServerStopNoticeMessageEvent::RoomServerStop(ServerID nStopServerID,MessageHeadSS * pMsgHead)
{
	if(pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x}\n",pMsgHead);
		return E_NULLPOINTER;
	}
	EntityType nStopServerType = enmEntityType_Room;

	ServerLoadManagerMap &stManagerMap = g_ServerLoadManager;

	if(stManagerMap.count(nStopServerType) == 0)
	{
		WRITE_WARNING_LOG("It's not found server load info!{nServerType=%d,nServerID=%d}",nStopServerType, nStopServerID);
		return S_OK;
	}

	stManagerMap[nStopServerType].DelServerLoad(nStopServerID);

	CRoomCache *arrRoomCache[MaxRoomCountPerServer];
	int32_t nRoomCacheCount = 0;
	g_DataCenter.GetRoomCahce(nStopServerID, arrRoomCache, MaxRoomCountPerServer, nRoomCacheCount);

	CBuildRoomDataNoti stRoomNoti;
	CBuildPlayerDataNoti stPlayerNoti;
	for(int32_t i = 0; i < nRoomCacheCount; ++i)
	{
		RoomID nRoomID = arrRoomCache[i]->GetRoomID();
		if(arrRoomCache[i]==NULL)
		{
			continue;
		}
		if(arrRoomCache[i]->GetRoomID()==enmInvalidRoomID)
		{
			continue;
		}
		CRoom *pRoom = NULL;
		int32_t ret = g_RoomMgt.GetRoom(nRoomID, pRoom);
		if(ret < 0 || pRoom == NULL)
		{
			WRITE_ERROR_LOG("get room error(nRoomID=%d, ret=%d)",nRoomID,ret);
			continue;
		}
		RoleID arrRoleID[MaxUserCountPerRoom];
		int32_t nPlayerCount = 0;
		pRoom->GetAllPlayers(arrRoleID, MaxUserCountPerRoom, nPlayerCount);
		if(nPlayerCount == 0)
		{
			continue;
		}
		//����һ�����еķ�����
		ServerID nIdleServerID = enmInvalidServerID;
		uint32_t nGetCount = 0;
		//ѡȡserveridʧ�ܵ�ʱ���Զ��ѡȡ
		while(nIdleServerID <= 0 && nGetCount < MaxRoomServerCountPerRoomDispatcher)
		{
			nIdleServerID = g_DataCenter.GetIdleRoomServerID();
			if(nIdleServerID <= 0)
			{
				WRITE_ERROR_LOG("shit!we get idle roomserverid is invalid!{StopRoomServerID=%d}\n", nStopServerID);
			}
			else if(nIdleServerID == nStopServerID)
			{
				WRITE_ERROR_LOG("shit!idle server id equal stop server id!{nServerId=%d}\n", nIdleServerID);
				nIdleServerID = enmInvalidServerID;
			}
			nGetCount++;
		}
		WRITE_NOTICE_LOG("room will dispatch to other server{nRoomID=%d, nServerID=%d}\n", arrRoomCache[i]->GetRoomID(), nIdleServerID);
		//û�п������ָ���roomserver
		if(nIdleServerID == enmInvalidServerID)
		{
			continue;
		}
		//�л����µ�server��
		arrRoomCache[i]->SetServerID(nIdleServerID);
		//��ʶΪ���ڻָ�
		arrRoomCache[i]->IsRebulid();

		stRoomNoti.nRoomDataSize = arrRoomCache[i]->GetRoomCacheSize();
		memcpy(stRoomNoti.arrRoomData, arrRoomCache[i]->GetRoomCache(), stRoomNoti.nRoomDataSize);

		//���ͷ��������

		SendMessageNotifyToServer(MSGID_RDRS_BUILDROOMDATA_NOTI, &stRoomNoti, nRoomID, enmTransType_P2P, enmInvalidRoleID, enmEntityType_Room, nIdleServerID);


		WRITE_DISTRIBUTED_NOTICE_LOG("reDispatch Room:%d to Server:%d PlayerCount=%d", nRoomID,nIdleServerID,nPlayerCount);

		//���������������ҵ�����
		int32_t nIndex = 0;
		stPlayerNoti.nRoomID = nRoomID;
		for(int32_t j = 0; j < nPlayerCount; ++j)
		{
			CPlayerCache *pStopPlayerCache = NULL;
			ret = g_PlayerCacheMgt(nStopServerID)->GetPlayerCache(arrRoleID[j], pStopPlayerCache);
			if(ret < 0 || pStopPlayerCache == NULL)
			{
				continue;
			}

			CPlayerCache *pIdlePlayerCache = NULL;
			ret = g_PlayerCacheMgt(nIdleServerID)->GetPlayerCache(arrRoleID[j], pIdlePlayerCache);
			//���·���ķ�������û�д���ҵ�����ʱ�������·������ϴ��������cache�������ֱ��ɾ��ʧЧ�������ϵ�
			//������ݣ���Ϊ���е�������ݻ�����server�Ϻϲ����ݺ����ͬ������
			if(ret < 0 || pIdlePlayerCache == NULL)
			{
				ret = g_PlayerCacheMgt(nIdleServerID)->CreatePlayerCache(arrRoleID[j], pIdlePlayerCache);
				if(ret < 0 || pIdlePlayerCache == NULL)
				{
					continue;
				}
				//�����ݸ��Ƶ��·���ķ�����
				memcpy(pIdlePlayerCache, pStopPlayerCache, sizeof(CPlayerCache));
				pIdlePlayerCache->SetServerID(nIdleServerID);
			}

			stPlayerNoti.arrRoleID[nIndex] = arrRoleID[j];
			stPlayerNoti.arrPlayerDataSize[nIndex] = pStopPlayerCache->GetPlayerCacheSize();
			memcpy(stPlayerNoti.arrPlayerData[nIndex], pStopPlayerCache->GetPlayerCache(), stPlayerNoti.arrPlayerDataSize[nIndex]);

			++nIndex;
			if(nIndex % MaxBuildPlayerDataCount == 0)
			{
				stPlayerNoti.nPlayerCount = nIndex;
				SendMessageNotifyToServer(MSGID_RDRS_BUILDPLAYERDATA_NOTI, &stPlayerNoti, nRoomID, enmTransType_P2P, enmInvalidRoleID, enmEntityType_Room, nIdleServerID);

				nIndex = 0;
			}
		}
		//����ʣ����������
		if(nIndex > 0)
		{
			stPlayerNoti.nPlayerCount = nIndex;
			SendMessageNotifyToServer(MSGID_RDRS_BUILDPLAYERDATA_NOTI, &stPlayerNoti, pMsgHead->nRoomID, enmTransType_P2P, enmInvalidRoleID, enmEntityType_Room, nIdleServerID);
		}
	}

	//�����ʧЧserver�����е����cache����
	g_PlayerCacheMgt(nStopServerID)->Clear();
	return S_OK;
}
//mediaserver�������ʱ��Ļָ�
int32_t CServerStopNoticeMessageEvent::MediaServerStop(ServerID nStopServerID,MessageHeadSS * pMsgHead)
{
	int32_t ret = S_OK;
	EntityType nStopServerType = enmEntityType_Media;
	if(pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x}\n",pMsgHead);
		return E_NULLPOINTER;
	}
	//ͣ���������̨mediaserver�ķ���
	CMedia *pMediaServer = NULL;
	g_MediaMgt.GetMedia(nStopServerID,pMediaServer);
	if(pMediaServer==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMediaServer=0x%08x}\n",pMediaServer);
		return E_NULLPOINTER;
	}
	ServerID nPartServerID = pMediaServer->GetPartServerID();
	if(nPartServerID==enmInvalidServerID)
	{
		WRITE_ERROR_LOG("get MediaServer partServerID filed{nServerid=%d, nPartserverid=%d] \n",nStopServerID,enmInvalidServerID);
		return ret;
	}
	ServerLoadManagerMap &stManagerMap = g_ServerLoadManager;
	if(stManagerMap.count(nStopServerType) == 0)
	{
		WRITE_WARNING_LOG("It's not found server load info!{nServerType=%d, nServerID=%d,nPartServerid=%d]", nStopServerID, nStopServerType,nPartServerID);
		return S_OK;
	}
	stManagerMap[nStopServerType].DelServerLoad(nStopServerID);
	stManagerMap[nStopServerType].DelServerLoad(nPartServerID);
	//��ȡ��Щ������Ҫ���·���mediaserver
	RoomID arrRoomID[MaxRoomCountPerMedia];
	int32_t nRoomCount = 0;
	pMediaServer->GetAllRooms(arrRoomID, MaxRoomCountPerMedia, nRoomCount);
	//���޷��䷿���media
	for(int32_t i = 0;i<nRoomCount;i++)
	{
		CRoom *pRoom = NULL;
		int32_t ret = g_RoomMgt.GetRoom(arrRoomID[i], pRoom);
		if(ret < 0 || pRoom == NULL)
		{
			WRITE_DEBUG_LOG("It's not found room object by roomid{nRoomID=%d}\n", arrRoomID[i]);
			continue;
		}
		CMedia *arrMediaServer[enmNetType_Count] = {NULL};
		ret = g_DataCenter.GetIdleMediaServer(arrMediaServer, enmNetType_Count);
		//֪ͨroomserver�޸���
		CSetMediaInfoNoti stSetMediaInfoNoti;
		stSetMediaInfoNoti.nRoomID = arrRoomID[i];
		char strTelIP[16]={'\0'};
		char strCncIP[16]={'\0'};
		if(arrMediaServer[enmNetType_Tel] != NULL && arrMediaServer[enmNetType_Cnc] != NULL)
		{
			//����
			stSetMediaInfoNoti.nTelMediaIP = arrMediaServer[enmNetType_Tel]->GetServerAddress();
			stSetMediaInfoNoti.nTelMediaPort = arrMediaServer[enmNetType_Tel]->GetServerPort();
			stSetMediaInfoNoti.nTelServerID = arrMediaServer[enmNetType_Tel]->GetServerID();
			//��ͨ
			stSetMediaInfoNoti.nCncMediaIP = arrMediaServer[enmNetType_Cnc]->GetServerAddress();
			stSetMediaInfoNoti.nCncMediaPort = arrMediaServer[enmNetType_Cnc]->GetServerPort();
			stSetMediaInfoNoti.nCncServerID = arrMediaServer[enmNetType_Cnc]->GetServerID();
		}
		else
		{
			WRITE_ERROR_LOG("reDispatch MediaServer filed[roomid=%d] \n",arrRoomID[i]);
			//����
			stSetMediaInfoNoti.nTelMediaIP = 0;
			stSetMediaInfoNoti.nTelMediaPort = 0;
			stSetMediaInfoNoti.nTelServerID = enmInvalidServerID;
			//��ͨ
			stSetMediaInfoNoti.nCncMediaIP = 0;
			stSetMediaInfoNoti.nCncMediaPort = 0;
			stSetMediaInfoNoti.nCncServerID = enmInvalidServerID;
		}
		sprintf(strTelIP,"%s",inet_ntoa_f(stSetMediaInfoNoti.nTelMediaIP));
		sprintf(strCncIP,"%s",inet_ntoa_f(stSetMediaInfoNoti.nCncMediaIP));
		SendMessageNotifyToServer(MSGID_RDRS_SETMEIDAINFO_NOTI, &stSetMediaInfoNoti, arrRoomID[i], enmTransType_P2P, enmInvalidRoleID, enmEntityType_Room,pRoom->GetRoomServerID());
		WRITE_DISTRIBUTED_NOTICE_LOG("reDispatch media to Room:%d  MediaServerID_Tel:%s:%d:%d MediaServerID_Cnc:%s:%d:%d", arrRoomID[i],strTelIP,stSetMediaInfoNoti.nTelMediaPort,stSetMediaInfoNoti.nTelServerID,strCncIP,stSetMediaInfoNoti.nCncMediaPort,stSetMediaInfoNoti.nCncServerID);
	}
	//������һ��media
	g_MediaMgt.DestoryMedia(nStopServerID);
	g_MediaMgt.DestoryMedia(nPartServerID);

	return S_OK;
}
FRAME_ROOMDISPATCHER_NAMESPACE_END

