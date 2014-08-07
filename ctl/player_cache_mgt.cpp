/*
 * player_cache_mgt.cpp
 *
 *  Created on: 2012-4-24
 *      Author: jimm
 */

#include "../../lightframe/frame_errordef.h"
#include "player_cache_mgt.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

CPlayerCacheMgt::CPlayerCacheMgt()
{
}
CPlayerCacheMgt::~CPlayerCacheMgt()
{

}

//��ʼ����ҹ�����
int32_t CPlayerCacheMgt::Initialize()
{
	m_sPlayerCachePool.Initialize();
	m_sPlayerCacheList.Initialize();

	return S_OK;
}

//�ָ���ҹ�����
int32_t CPlayerCacheMgt::Resume()
{
	m_sPlayerCachePool.Resume();
	m_sPlayerCacheList.Resume();

	return S_OK;
}

//ע����ҹ�����
int32_t CPlayerCacheMgt::Uninitialize()
{
	m_sPlayerCachePool.Uninitialize();
	m_sPlayerCacheList.Uninitialize();

	return S_OK;
}

int32_t CPlayerCacheMgt::CreatePlayerCache(const RoleID nRoleID, PlayerCachePool::CIndex *&pIndex)
{
	pIndex = m_sPlayerCachePool.CreateObject();
	if(NULL == pIndex)
	{
		return E_LIBF_CREATEPLAYER;
	}

	PlayerCacheList::Key key = MakeUserKey(nRoleID);
	PlayerCacheList::CIndex *pListIndex = m_sPlayerCacheList.Insert(key, pIndex->Index());
	if (NULL == pListIndex)
	{
		m_sPlayerCachePool.DestroyObject(pIndex);
		pIndex = NULL;
		return E_LIBF_CREATEPLAYERINDEX;
	}

	//���������浽�������ݱ���
	int32_t ret = pIndex->SetAdditionalData(enmAdditionalIndex_HashListIndex, (uint32_t)pListIndex->Index());
	if (0 > ret)
	{
		m_sPlayerCacheList.Erase(pListIndex);
		m_sPlayerCachePool.DestroyObject(pIndex);
		pIndex = NULL;
		return ret;
	}

	return S_OK;
}

int32_t CPlayerCacheMgt::CreatePlayerCache(const RoleID nRoleID, CPlayerCache *&pPlayerCache)
{
	PlayerCachePool::CIndex* pIndex = NULL;

	int32_t ret = CreatePlayerCache(nRoleID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pPlayerCache = pIndex->ObjectPtr();
	pPlayerCache->SetRoleID(nRoleID);

	return S_OK;
}

int32_t CPlayerCacheMgt::GetPlayerCache(const RoleID nRoleID, PlayerCachePool::CIndex*& pIndex)
{
	//������������
	PlayerCacheList::Key key = MakeUserKey(nRoleID);
	PlayerCacheList::CIndex* pListIndex = m_sPlayerCacheList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_PLAYERNOTFOUND;
	}

	pIndex = m_sPlayerCachePool.GetIndex(pListIndex->GetObject());
	if (NULL == pIndex)
	{
		return E_LIBF_PLAYERNOTFOUND;
	}

	return S_OK;
}

int32_t CPlayerCacheMgt::GetPlayerCache(const RoleID nRoleID, CPlayerCache *&pPlayerCache)
{
	PlayerCachePool::CIndex* pIndex = NULL;

	int32_t ret = GetPlayerCache(nRoleID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pPlayerCache = pIndex->ObjectPtr();

	return S_OK;
}

int32_t CPlayerCacheMgt::DestoryPlayerCache(const RoleID nRoleID)
{
	//������������
	PlayerCacheList::Key key = MakeUserKey(nRoleID);
	PlayerCacheList::CIndex* pListIndex = m_sPlayerCacheList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_PLAYERNOTEXIST;
	}

	PlayerIndex playerIndex = pListIndex->GetObject();

	//����ҴӶ���ؼ���������ɾ��
	m_sPlayerCacheList.Erase(pListIndex);
	m_sPlayerCachePool.DestroyObject(playerIndex);

	return S_OK;
}

int32_t CPlayerCacheMgt::GetAllPlayerCache(CPlayerCache *arrPlayerCacheObject[], const int32_t arrSize, int32_t &nPlayerCacheCount)
{
	nPlayerCacheCount = 0;
	PlayerCachePool::CIndex *pIndex = m_sPlayerCachePool.First();

	while(NULL != pIndex && nPlayerCacheCount < arrSize)
	{
		arrPlayerCacheObject[nPlayerCacheCount++] = pIndex->ObjectPtr();
		pIndex = pIndex->Next();
	}

	return S_OK;
}


int32_t CPlayerCacheMgt::Clear()
{
	m_sPlayerCachePool.Clear();
	m_sPlayerCacheList.Clear();
	return S_OK;
}

//��ȡ���������
uint32_t CPlayerCacheMgt::GetCapacity() const
{
	return m_sPlayerCachePool.Capacity();
}

//��ȡ������ж�������
uint32_t CPlayerCacheMgt::GetPlayerCacheCount() const
{
	return m_sPlayerCachePool.ObjectCount();
}

//������Ƿ�Ϊ��
bool CPlayerCacheMgt::IsEmpty() const
{
	return m_sPlayerCachePool.IsEmpty();
}

//������Ƿ�����
bool CPlayerCacheMgt::IsFull() const
{
	return m_sPlayerCachePool.IsFull();
}

PlayerCacheList::Key CPlayerCacheMgt::MakeUserKey(const RoleID nRoleID) const
{
	PlayerCacheList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(RoleID);
	*(RoleID*)key.arrKey = nRoleID;

	return key;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

