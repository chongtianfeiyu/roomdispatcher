/*
 * player_cache_mgt.h
 *
 *  Created on: 2012-4-24
 *      Author: jimm
 */

#ifndef PLAYER_CACHE_MGT_H_
#define PLAYER_CACHE_MGT_H_

#include "common/common_pool.h"
#include "common/common_hashlist.h"
#include "dal/dal_playercache.h"
#include "def/server_namespace.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

//������Ҷ���غ���������
typedef CPool<CPlayerCache, MaxPlayerCountPerRoomServer>	PlayerCachePool;
typedef CHashList<PlayerIndex, MaxPlayerCountPerRoomServer, MaxPlayerCountPerRoomServer * 256>	PlayerCacheList;

class CPlayerCacheMgt : public CObject
{
protected:
	enum
	{
		enmAdditionalIndex_HashListIndex			= 0,
	};
public:
	CPlayerCacheMgt();
	virtual ~CPlayerCacheMgt();

	//��ʼ����ҹ�����
	virtual int32_t Initialize();
	//�ָ���ҹ�����
	virtual int32_t Resume();
	//ע����ҹ�����
	virtual int32_t Uninitialize();

	int32_t CreatePlayerCache(const RoleID nRoleID, CPlayerCache *&pPlayerCache);

	int32_t GetPlayerCache(const RoleID nRoleID, CPlayerCache *&pPlayerCache);

	int32_t GetAllPlayerCache(CPlayerCache *arrPlayerCacheObject[], const int32_t arrSize, int32_t &nPlayerCacheCount);

	int32_t DestoryPlayerCache(const RoleID nRoleID);

	int32_t Clear();

	//��ȡ���������
	uint32_t GetCapacity() const;
	//��ȡ������ж�������
	uint32_t GetPlayerCacheCount() const;
	//������Ƿ�Ϊ��
	bool IsEmpty() const;
	//������Ƿ�����
	bool IsFull() const;

protected:
	int32_t CreatePlayerCache(const RoleID nRoleID, PlayerCachePool::CIndex *&pIndex);

	int32_t GetPlayerCache(const RoleID nRoleID, PlayerCachePool::CIndex*& pIndex);

protected:
	PlayerCacheList::Key MakeUserKey(const RoleID nRoleID) const;
private:
	PlayerCachePool					m_sPlayerCachePool;
	PlayerCacheList					m_sPlayerCacheList;
};

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* PLAYER_CACHE_MGT_H_ */
