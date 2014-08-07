/*
 * room_cache_mgt.cpp
 *
 *  Created on: 2012-4-24
 *      Author: jimm
 */

#include "../../lightframe/frame_errordef.h"
#include "room_cache_mgt.h"
#include "server_datacenter.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

CRoomCacheMgt::CRoomCacheMgt()
{
}
CRoomCacheMgt::~CRoomCacheMgt()
{

}

//初始化玩家管理器
int32_t CRoomCacheMgt::Initialize()
{
	m_sRoomCachePool.Initialize();
	m_sRoomCacheList.Initialize();

	return S_OK;
}

//恢复玩家管理器
int32_t CRoomCacheMgt::Resume()
{
	m_sRoomCachePool.Resume();
	m_sRoomCacheList.Resume();

	return S_OK;
}

//注销玩家管理器
int32_t CRoomCacheMgt::Uninitialize()
{
	m_sRoomCachePool.Uninitialize();
	m_sRoomCacheList.Uninitialize();

	return S_OK;
}

int32_t CRoomCacheMgt::CreateRoomCache(const RoomID nRoomID, RoomCachePool::CIndex *&pIndex)
{
	pIndex = m_sRoomCachePool.CreateObject();
	if(NULL == pIndex)
	{
		return E_LIBF_CREATEROOM;
	}

	RoomCacheList::Key key = MakeRoomKey(nRoomID);
	RoomCacheList::CIndex *pListIndex = m_sRoomCacheList.Insert(key, pIndex->Index());
	if (NULL == pListIndex)
	{
		m_sRoomCachePool.DestroyObject(pIndex);
		pIndex = NULL;
		return E_LIBF_CREATEROOMINDEX;
	}

	//将索引保存到附加数据表中
	int32_t ret = pIndex->SetAdditionalData(enmAdditionalIndex_HashListIndex, (uint32_t)pListIndex->Index());
	if (0 > ret)
	{
		m_sRoomCacheList.Erase(pListIndex);
		m_sRoomCachePool.DestroyObject(pIndex);
		pIndex = NULL;
		return ret;
	}

	return S_OK;
}

int32_t CRoomCacheMgt::CreateRoomCache(const RoomID nRoomID, CRoomCache *&pRoomCache)
{
	RoomCachePool::CIndex* pIndex = NULL;

	int32_t ret = CreateRoomCache(nRoomID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pRoomCache = pIndex->ObjectPtr();
	pRoomCache->SetRoomID(nRoomID);

	return S_OK;
}

int32_t CRoomCacheMgt::GetRoomCache(const RoomID nRoomID, RoomCachePool::CIndex*& pIndex)
{
	//根据索引查找
	RoomCacheList::Key key = MakeRoomKey(nRoomID);
	RoomCacheList::CIndex* pListIndex = m_sRoomCacheList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_ROOMNOTFOUND;
	}

	pIndex = m_sRoomCachePool.GetIndex(pListIndex->GetObject());
	if (NULL == pIndex)
	{
		return E_LIBF_ROOMNOTFOUND;
	}

	return S_OK;
}

int32_t CRoomCacheMgt::GetRoomCache(const RoomID nRoomID, CRoomCache *&pRoomCache)
{
	RoomCachePool::CIndex* pIndex = NULL;

	int32_t ret = GetRoomCache(nRoomID, pIndex);
	if (0 > ret||pIndex==NULL)
	{
		return ret;
	}

	pRoomCache = pIndex->ObjectPtr();

	return S_OK;
}

int32_t CRoomCacheMgt::DestoryRoomCache(const RoomID nRoomID)
{
	//根据索引查找
	RoomCacheList::Key key = MakeRoomKey(nRoomID);
	RoomCacheList::CIndex* pListIndex = m_sRoomCacheList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_ROOMNOTEXIST;
	}

//	DeleteAllPlayersInRoom(nRoomID);

	RoomIndex playerIndex = pListIndex->GetObject();

	//将玩家从对象池及索引表中删除
	m_sRoomCacheList.Erase(pListIndex);
	m_sRoomCachePool.DestroyObject(playerIndex);

	return S_OK;
}

int32_t CRoomCacheMgt::Clear()
{
	m_sRoomCachePool.Clear();
	m_sRoomCacheList.Clear();
	return S_OK;
}

//获取对象池容量
uint32_t CRoomCacheMgt::GetCapacity() const
{
	return m_sRoomCachePool.Capacity();
}

//获取对象池中对象数量
uint32_t CRoomCacheMgt::GetRoomCacheCount() const
{
	return m_sRoomCachePool.ObjectCount();
}

//对象池是否为空
bool CRoomCacheMgt::IsEmpty() const
{
	return m_sRoomCachePool.IsEmpty();
}

//对象池是否已满
bool CRoomCacheMgt::IsFull() const
{
	return m_sRoomCachePool.IsFull();
}

int32_t CRoomCacheMgt::GetAllRoomCache(CRoomCache *arrRoomCacheObject[], const int32_t arrSize, int32_t &nRoomCacheObjectCount)
{
	nRoomCacheObjectCount = 0;
	RoomCachePool::CIndex *pIndex = m_sRoomCachePool.First();

	while(NULL != pIndex && nRoomCacheObjectCount < arrSize)
	{
		arrRoomCacheObject[nRoomCacheObjectCount++] = pIndex->ObjectPtr();
		pIndex = pIndex->Next();
	}

	return S_OK;
}

RoomCacheList::Key CRoomCacheMgt::MakeRoomKey(const RoomID nRoomID) const
{
	RoomCacheList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(RoomID);
	*(RoomID*)key.arrKey = nRoomID;

	return key;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

