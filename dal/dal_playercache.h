/*
 * dal_playercache.h
 *
 *  Created on: 2012-4-23
 *      Author: jimm
 */

#ifndef DAL_PLAYERCACHE_H_
#define DAL_PLAYERCACHE_H_

#include "common/common_complextype.h"
#include "common/common_hashlist.h"
#include "common/common_string.h"
#include "dal/dal_player.h"
#include "def/server_namespace.h"
#include "def/dal_def.h"

#include "public_typedef.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CPlayerCache : public CObject
{
public:
	CPlayerCache()
	{
		Reset();
	}

	virtual ~CPlayerCache()
	{

	}

	int32_t Initialize()
	{
		Reset();
		return S_OK;
	}

	int32_t Resume()
	{
		Reset();
		return S_OK;
	}

	int32_t Uninitialize()
	{
		Reset();
		return S_OK;
	}

	void SetRoleID(RoleID nRoleID)
	{
		m_nRoleID = nRoleID;
	}

	RoleID GetRoleID()
	{
		return m_nRoleID;
	}

	void SetServerID(ServerID nServerID)
	{
		m_nServerID = nServerID;
	}

	ServerID GetServerID()
	{
		return m_nServerID;
	}

	int32_t UpdatePlayerCache(uint32_t nStartPos, uint32_t nEndPos, uint32_t nDataOffset, int32_t nDataSize, uint8_t arrData[])
	{
		uint32_t nPlayerCacheSize = nEndPos - nStartPos;
		if(nPlayerCacheSize > PlayerCacheSize)
		{
			return E_HS_DATASIZEERROR;
		}

		if((nDataOffset > nPlayerCacheSize) || (nDataOffset < 0))
		{
			return E_HS_DATASIZEERROR;
		}

		if((uint32_t)nDataSize > PlayerCacheSize - nDataOffset)
		{
			return E_HS_DATASIZEERROR;
		}

		//拷贝内存到playercache相应位置
		memcpy(m_arrPlayerCache + nDataOffset, arrData, nDataSize);

		//更新playerchche实际大小
		m_nPlayerCacheSize = nPlayerCacheSize;

		return S_OK;
	}

	uint32_t GetPlayerCacheSize()
	{
		return m_nPlayerCacheSize;
	}

	uint8_t *GetPlayerCache()
	{
		return m_arrPlayerCache;
	}
private:
	void Reset()
	{
		m_nRoleID = enmInvalidRoleID;
		m_nServerID = enmInvalidServerID;
		m_nPlayerCacheSize = 0;
		memset(m_arrPlayerCache, 0, sizeof(m_arrPlayerCache));
	}

private:
	RoleID 						m_nRoleID;
	ServerID					m_nServerID;
	uint32_t					m_nPlayerCacheSize;
	uint8_t						m_arrPlayerCache[PlayerCacheSize];
};

FRAME_ROOMDISPATCHER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_ROOMDISPATCHER_NAMESPACE::CPlayerCache)

#endif /* DAL_PLAYERCACHE_H_ */
