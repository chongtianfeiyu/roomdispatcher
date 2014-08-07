/*
 * server_datacenter.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef SERVER_DATACENTER_H_
#define SERVER_DATACENTER_H_

#include "common/common_object.h"
#include "common/common_memory.h"
#include "common/common_singleton.h"
#include "../../lightframe/lightframe_impl.h"
#include "def/server_namespace.h"
#include "dal/dal_region.h"
#include "public_typedef.h"
#include "def/dal_def.h"

#include "player_mgt.h"
#include "region_mgt.h"
#include "room_mgt.h"
#include "media_mgt.h"
#include "serverinfo_mgt.h"
#include "room_cache_mgt.h"
#include "player_cache_mgt.h"

#include <map>

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

using namespace std;

typedef map<EntityType, CServerLoadManager>		ServerLoadManagerMap;

//��hall�ύ��ʱ������Ϣ��server����
#define MaxServerInfoCount		256

class ServerInfo
{
public:
	EntityType			nEntityType;
	ServerID			nServerID;
	int64_t				nLastUpdateTime;

	void Reset()
	{
		nEntityType = enmInvalidEntityType;
		nServerID = enmInvalidServerID;
		nLastUpdateTime = 0;
	}

	ServerInfo()
	{
		Reset();
	}
};

class CServerDataCenter : public IDataCenter
{
public:
	CServerDataCenter();
	virtual ~CServerDataCenter();

public:
	virtual const char* GetName();

	//��Frame�ṩ�Լ������ڴ��С�Ĳ�ѯ�ӿ�
	virtual size_t GetSize();

	//��ȡ�����ڴ�Key(����0��ʾ��Frame����)
	virtual int32_t GetKey(){return 0;}

	//��ʼ���ӿڵ��õ�ʱ�� �����ڴ��ѷ���
	virtual int32_t Initialize();
	virtual int32_t Resume();
	virtual int32_t Uninitialize();

public:
	//��ȡ��ҹ���������
	CPlayerMgt& GetPlayerMgt()
	{
		return *m_pPlayerMgt;
	}

	CRegionMgt& GetRegionMgt()
	{
		return *m_pRegionMgt;
	}

	CRoomMgt& GetRoomMgt()
	{
		return *m_pRoomMgt;
	}

	CMediaMgt& GetMediaMgt()
	{
		return *m_pMediaMgt;
	}

	ServerLoadManagerMap &GetServerLoadManagerMap()
	{
		return m_sServerLoadMap;
	}

	CRoomCacheMgt& GetRoomCacheMgt()
	{
		return *m_pRoomCacheMgt;
	}

	CPlayerCacheMgt* GetPlayerCacheMgt(ServerID index)
	{
		if(index < 0 || index > MaxRoomServerCountPerRoomDispatcher)
		{
			return NULL;
		}
		return m_arrPlayerCacheMgt[index];
	}
	//ȡ����Կ��е�roomserverid
	ServerID GetIdleRoomServerID();
	//ȡ��һ����е�mediaserver
	int32_t GetIdleMediaServer(CMedia *arrMediaServer[], int32_t arrSize);

	//����server��Ϣ
	void UpdateServerInfo(EntityType nEntityType, ServerID nServerID, int32_t nRoomCount, int32_t nPlayerCount,int32_t nPacketCount,NetType nNetType);

	//server��Ϣ�Ƿ����
	bool IsServerInfoExpire(EntityType nEntityType, ServerID nServerID);

	int32_t GetAllPlayers(const RoomID nRoomID, CPlayer *arrPlayer[],
			const int32_t arrSize, int32_t &nPlayerCount);

	int32_t GetAllPlayers(CRoom *pRoom, CPlayer *arrPlayer[],
			const int32_t arrSize, int32_t &nPlayerCount);

	int32_t GetRoomCahce(ServerID nServerID, CRoomCache *arrRoomCahce[], int32_t nRoomCacheSize,
			int32_t &nRoomCacheCount);

protected:

	CPlayerMgt				*m_pPlayerMgt;								//��ҹ���������
	CRegionMgt				*m_pRegionMgt;								//��������������
	CRoomMgt				*m_pRoomMgt;								//�������������
	CMediaMgt				*m_pMediaMgt;								//ý�����������
	ServerLoadManagerMap	 m_sServerLoadMap;
	CRoomCacheMgt			*m_pRoomCacheMgt;							//���仺�����������
	CPlayerCacheMgt			*m_arrPlayerCacheMgt[MaxRoomServerCountPerRoomDispatcher];	//��һ������������

	int32_t					m_nServerCount;
	ServerInfo				m_arrServerInfo[MaxServerInfoCount];
};


#define	CREATE_DALDATACENTER_INSTANCE		CSingleton<CServerDataCenter>::CreateInstance
#define	GET_DALDATACENTER_INSTANCE			CSingleton<CServerDataCenter>::GetInstance
#define	DESTROY_DALDATACENTER_INSTANCE		CSingleton<CServerDataCenter>::DestroyInstance

#define g_DataCenter		GET_DALDATACENTER_INSTANCE()
#define g_PlayerMgt			GET_DALDATACENTER_INSTANCE().GetPlayerMgt()
#define g_RegionMgt			GET_DALDATACENTER_INSTANCE().GetRegionMgt()
#define g_RoomMgt			GET_DALDATACENTER_INSTANCE().GetRoomMgt()
#define g_MediaMgt			GET_DALDATACENTER_INSTANCE().GetMediaMgt()
#define g_ServerLoadManager	GET_DALDATACENTER_INSTANCE().GetServerLoadManagerMap()
#define g_RoomCacheMgt		GET_DALDATACENTER_INSTANCE().GetRoomCacheMgt()
#define g_PlayerCacheMgt(i)	GET_DALDATACENTER_INSTANCE().GetPlayerCacheMgt(i)


FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* SERVER_DATACENTER_H_ */
