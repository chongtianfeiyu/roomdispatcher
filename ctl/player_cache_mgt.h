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

//定义玩家对象池和索引类型
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

	//初始化玩家管理器
	virtual int32_t Initialize();
	//恢复玩家管理器
	virtual int32_t Resume();
	//注销玩家管理器
	virtual int32_t Uninitialize();

	int32_t CreatePlayerCache(const RoleID nRoleID, CPlayerCache *&pPlayerCache);

	int32_t GetPlayerCache(const RoleID nRoleID, CPlayerCache *&pPlayerCache);

	int32_t GetAllPlayerCache(CPlayerCache *arrPlayerCacheObject[], const int32_t arrSize, int32_t &nPlayerCacheCount);

	int32_t DestoryPlayerCache(const RoleID nRoleID);

	int32_t Clear();

	//获取对象池容量
	uint32_t GetCapacity() const;
	//获取对象池中对象数量
	uint32_t GetPlayerCacheCount() const;
	//对象池是否为空
	bool IsEmpty() const;
	//对象池是否已满
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
