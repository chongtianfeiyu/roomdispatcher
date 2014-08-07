/*
 * room_cache_mgt.h
 *
 *  Created on: 2012-4-24
 *      Author: jimm
 */

#ifndef ROOM_CACHE_MGT_H_
#define ROOM_CACHE_MGT_H_

#include "common/common_pool.h"
#include "common/common_hashlist.h"
#include "dal/dal_roomcache.h"
#include "def/server_namespace.h"
#include "public_typedef.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

//定义房间对象池和索引类型
typedef CPool<CRoomCache, MaxRoomCount>	RoomCachePool;
typedef CHashList<RoomIndex, MaxRoomCount, RoomBucketSize>	RoomCacheList;

class CRoomCacheMgt : public CObject
{
protected:
	enum
	{
		enmAdditionalIndex_HashListIndex			= 0,
	};
public:
	CRoomCacheMgt();
	virtual ~CRoomCacheMgt();

	//初始化房间管理器
	virtual int32_t Initialize();
	//恢复房间管理器
	virtual int32_t Resume();
	//注销房间管理器
	virtual int32_t Uninitialize();

	int32_t CreateRoomCache(const RoomID nRoomID, CRoomCache *&pRoomCache);

	int32_t GetRoomCache(const RoomID nRoomID, CRoomCache *&pRoomCache);

	int32_t DestoryRoomCache(const RoomID nRoomID);

	int32_t Clear();

	//获取对象池容量
	uint32_t GetCapacity() const;
	//获取对象池中对象数量
	uint32_t GetRoomCacheCount() const;
	//对象池是否为空
	bool IsEmpty() const;
	//对象池是否已满
	bool IsFull() const;

	int32_t GetAllRoomCache(CRoomCache *arrRoomCacheObject[], const int32_t arrSize, int32_t &nRoomCacheObjectCount);

protected:
	int32_t CreateRoomCache(const RoomID nRoomID, RoomCachePool::CIndex *&pIndex);

	int32_t GetRoomCache(const RoomID nRoomID, RoomCachePool::CIndex*& pIndex);

protected:
	RoomCacheList::Key MakeRoomKey(const RoomID nRoomID) const;
private:
	RoomCachePool					m_sRoomCachePool;
	RoomCacheList					m_sRoomCacheList;
};

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* ROOM_CACHE_MGT_H_ */
