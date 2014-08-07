/*
 * hallserver_datacenter.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include "common/common_datetime.h"
#include "../../lightframe/frame_errordef.h"
#include "../../lightframe/lightframe_impl.h"
#include "server_datacenter.h"
#include "def/dal_def.h"
#include "main_frame.h"

#define MaxCanProcessPackageCount	1000
#define MaxFlowMediaServer          (500*1024)
#define MaxPlayerOnRoomServer		5000

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

CServerDataCenter::CServerDataCenter()
{
	m_pPlayerMgt = NULL;
	m_pRegionMgt = NULL;
	m_pRoomMgt = NULL;
	m_pMediaMgt = NULL;
}

CServerDataCenter::~CServerDataCenter()
{

}

size_t CServerDataCenter::GetSize()
{
	return sizeof(CPlayerMgt) + sizeof(CRegionMgt) + sizeof(CRoomMgt) +
	sizeof(CMediaMgt) + sizeof(CRoomCacheMgt) +
	sizeof(CPlayerCacheMgt) * MaxRoomServerCountPerRoomDispatcher;

}
const char* CServerDataCenter::GetName()
{
	return SERVER_NAME_STRING"DataCenter";
}
//初始化数据中心
int32_t CServerDataCenter::Initialize()
{
	m_pPlayerMgt = new CPlayerMgt();
	if(NULL == m_pPlayerMgt)
	{
		return E_HS_CREATEPLAYERMGTERROR;
	}

	int32_t nRet = m_pPlayerMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRegionMgt = new CRegionMgt();
	if(NULL == m_pRegionMgt)
	{
		return E_HS_CREATEREGIONMGTERROR;
	}

	nRet = m_pRegionMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRoomMgt = new CRoomMgt();
	if(NULL == m_pRoomMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pRoomMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pMediaMgt = new CMediaMgt();
	if(NULL == m_pMediaMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pMediaMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRoomCacheMgt = new CRoomCacheMgt();
	if(NULL == m_pRoomCacheMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pRoomCacheMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	for(int32_t i = 0; i < MaxRoomServerCountPerRoomDispatcher; ++i)
	{
		m_arrPlayerCacheMgt[i] = new CPlayerCacheMgt();
		if(NULL == m_arrPlayerCacheMgt[i])
		{
			return E_HS_CREATEROOMMGTERROR;
		}

		nRet = m_arrPlayerCacheMgt[i]->Initialize();
		if(0 > nRet)
		{
			return nRet;
		}
	}

	return S_OK;
}

//恢复数据中心
int32_t CServerDataCenter::Resume()
{
	m_pPlayerMgt = new CPlayerMgt();
	if(NULL == m_pPlayerMgt)
	{
		return E_HS_CREATEPLAYERMGTERROR;
	}

	int32_t nRet = m_pPlayerMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRegionMgt = new CRegionMgt();
	if(NULL == m_pRegionMgt)
	{
		return E_HS_CREATEREGIONMGTERROR;
	}

	nRet = m_pRegionMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRoomMgt = new CRoomMgt();
	if(NULL == m_pRoomMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pRoomMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pMediaMgt = new CMediaMgt();
	if(NULL == m_pMediaMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pMediaMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRoomCacheMgt = new CRoomCacheMgt();
	if(NULL == m_pRoomCacheMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pRoomCacheMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	for(int32_t i = 0; i < MaxRoomServerCountPerRoomDispatcher; ++i)
	{
		m_arrPlayerCacheMgt[i] = new CPlayerCacheMgt();
		if(NULL == m_arrPlayerCacheMgt[i])
		{
			return E_HS_CREATEROOMMGTERROR;
		}

		nRet = m_arrPlayerCacheMgt[i]->Resume();
		if(0 > nRet)
		{
			return nRet;
		}
	}

	return S_OK;
}

//注销数据中心
int32_t CServerDataCenter::Uninitialize()
{
	if(m_pPlayerMgt != NULL)
	{
		delete m_pPlayerMgt;
		m_pPlayerMgt = NULL;
	}

	if(m_pRegionMgt != NULL)
	{
		delete m_pRegionMgt;
		m_pRegionMgt = NULL;
	}

	if(m_pRoomMgt != NULL)
	{
		delete m_pRoomMgt;
		m_pRoomMgt = NULL;
	}

	if(m_pMediaMgt != NULL)
	{
		delete m_pMediaMgt;
		m_pMediaMgt = NULL;
	}

	if(m_pRoomCacheMgt != NULL)
	{
		delete m_pRoomCacheMgt;
		m_pRoomCacheMgt = NULL;
	}

	for(int32_t i = 0; i < MaxRoomServerCountPerRoomDispatcher; ++i)
	{
		if(m_arrPlayerCacheMgt[i] != NULL)
		{
			delete m_arrPlayerCacheMgt[i];
			m_arrPlayerCacheMgt[i] = NULL;
		}
	}

	return S_OK;
}

//取得相对空闲的roomserverid
ServerID CServerDataCenter::GetIdleRoomServerID()
{
	return m_sServerLoadMap[enmEntityType_Room].SelectServer();
}

//取得一组空闲的mediaserver
int32_t CServerDataCenter::GetIdleMediaServer(CMedia *arrMediaServer[], int32_t arrSize)
{
	if(arrMediaServer == NULL || arrSize < enmNetType_Count)
	{
		return E_NULLPOINTER;
	}

	ServerID nServerID = m_sServerLoadMap[enmEntityType_Media].SelectMediaServer(enmNetType_Tel);
	WRITE_DISTRIBUTED_DEBUG_LOG("select media serverid=%d ",nServerID);
	m_pMediaMgt->GetMedia(nServerID,arrMediaServer[enmNetType_Tel]);

	if(arrMediaServer[enmNetType_Tel]!=NULL)
	{
		ServerID nPartServerID = arrMediaServer[enmNetType_Tel]->GetPartServerID();
		WRITE_DISTRIBUTED_DEBUG_LOG("select media partserverid=%d ",nPartServerID);
		m_pMediaMgt->GetMedia(nPartServerID,arrMediaServer[enmNetType_Cnc]);
	}
	return S_OK;
}

//更新server信息
void CServerDataCenter::UpdateServerInfo(EntityType nEntityType, ServerID nServerID, int32_t nRoomCount, int32_t nPlayerCount, int32_t nPacketCount,NetType nNetType)
{
	//更新server info
	ServerLoadInfo stInfo;
	stInfo.serverID = nServerID;
	//todo 这个值还要研究一下（打算根据压力测试的数据在做一个算法，media，room可能不同）
	if(nEntityType == enmEntityType_Media)
	{
		stInfo.loadValue = ((float)nPacketCount/MaxFlowMediaServer) * 100;
	}
	else
	{
		stInfo.loadValue = ((float)nPlayerCount/MaxPlayerOnRoomServer) * 100;
	}
	stInfo.nLastUpdateTime = CDateTime::CurrentDateTime().Seconds();
	stInfo.nNetType = nNetType;
	WRITE_DISTRIBUTED_NOTICE_LOG("ServerType:%d  ServerID:%d RoomCount:%d PlayerCount:%d ServerLoadValue:%d",nEntityType,nServerID,nRoomCount,nPlayerCount,stInfo.loadValue);
	m_sServerLoadMap[nEntityType].UpdateServerLoad(stInfo);
}

//server信息是否过期
bool CServerDataCenter::IsServerInfoExpire(EntityType nEntityType, ServerID nServerID)
{
	for(int32_t i = 0; i < m_nServerCount; ++i)
	{
		if(m_arrServerInfo[i].nEntityType != nEntityType)
		{
			continue;
		}

		if(m_arrServerInfo[i].nServerID != nServerID)
		{
			continue;
		}

		//当前时间减去最后更新时间是否大于过期间隔时间
		return CDateTime::CurrentDateTime().Seconds() - m_arrServerInfo[i].nLastUpdateTime > ServerInfoExpireTime;
	}

	return true;
}

int32_t CServerDataCenter::GetAllPlayers(const RoomID nRoomID, CPlayer *arrPlayer[],
		const int32_t arrSize, int32_t &nPlayerCount)
{
	int32_t ret = 0;
	CRoom *pRoom = NULL;
	ret = g_RoomMgt.GetRoom(nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		return ret;
	}

	return GetAllPlayers(pRoom, arrPlayer, arrSize, nPlayerCount);
}

int32_t CServerDataCenter::GetAllPlayers(CRoom *pRoom, CPlayer *arrPlayer[],
		const int32_t arrSize, int32_t &nPlayerCount)
{
	if(pRoom == NULL)
	{
		return S_OK;
	}

	RoleID arrRoleID[MaxUserCountPerRoom] = {enmInvalidRoleID};
	nPlayerCount = 0;
	pRoom->GetAllPlayers(arrRoleID, MaxUserCountPerRoom, nPlayerCount);

	for(int32_t i = 0; i < nPlayerCount; ++i)
	{
		CPlayer *pPlayer = NULL;
		g_PlayerMgt.GetPlayer(arrRoleID[i], pPlayer);
		if(pPlayer == NULL)
		{
			if(nPlayerCount > 0)
			{
				--nPlayerCount;
			}
			continue;
		}

		arrPlayer[i] = pPlayer;
	}

	return S_OK;
}

int32_t CServerDataCenter::GetRoomCahce(ServerID nServerID, CRoomCache *arrRoomCahce[], int32_t nRoomCacheSize,
		int32_t &nRoomCacheCount)
{
	nRoomCacheCount = 0;

	int32_t nCount = 0;
	CRoomCache *arrCache[MaxRoomServerCountPerRoomDispatcher*MaxRoomCountPerServer];

	int32_t ret = m_pRoomCacheMgt->GetAllRoomCache(arrCache, MaxRoomServerCountPerRoomDispatcher*MaxRoomCountPerServer, nCount);
	if(ret < 0)
	{
		return S_OK;
	}

	for(int32_t i = 0; i < nCount; ++i)
	{
		if(nServerID == arrCache[i]->GetServerID()&&nRoomCacheCount<nRoomCacheSize)
		{
			arrRoomCahce[nRoomCacheCount] = arrCache[i];
			++nRoomCacheCount;
		}
	}

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

