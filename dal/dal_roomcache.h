/*
 * dal_roomcache.h
 *
 *  Created on: 2012-4-23
 *      Author: jimm
 */

#ifndef DAL_ROOMCACHE_H_
#define DAL_ROOMCACHE_H_

#include "common/common_complextype.h"
#include "common/common_hashlist.h"
#include "common/common_string.h"
#include "../../lightframe/frame_typedef.h"
#include "dal/dal_player.h"
#include "def/server_namespace.h"
#include "def/dal_def.h"
#include <vector>
#include "dal/server_message.h"

#include "public_typedef.h"
using namespace std;
FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CReqDate
{
public:
	MessageHeadSS   stHead;
	CGetRoomServerIDReq  stGetRSIDReq;
	CReqDate()
	{
		ReSet();
	}
	void ReSet()
	{
		memset(&stHead,0,sizeof(stHead));
		memset(&stGetRSIDReq,0,sizeof(stGetRSIDReq));
	}
};

typedef vector<CReqDate>		ReqDateArray;


class CRoomCache : public CObject
{
public:
	CRoomCache()
	{
		Reset();
	}

	virtual ~CRoomCache()
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

	void SetRoomID(RoomID nRoomID)
	{
		m_nRoomID = nRoomID;
	}

	RoomID GetRoomID()
	{
		return m_nRoomID;
	}

	void SetServerID(ServerID nServerID)
	{
		m_nServerID = nServerID;
	}

	ServerID GetServerID()
	{
		return m_nServerID;
	}

	int32_t  UpdateRoomCache(uint32_t nStartPos, uint32_t nEndPos, uint32_t nDataOffset, int32_t nDataSize, uint8_t arrData[])
	{
		uint32_t nRoomCacheSize = nEndPos - nStartPos;
		if(nRoomCacheSize > RoomCacheSize)
		{
			return E_HS_DATASIZEERROR;
		}

		if((nDataOffset > nRoomCacheSize) || (nDataOffset < 0))
		{
			return E_HS_DATASIZEERROR;
		}

		if((uint32_t)nDataSize > RoomCacheSize - nDataOffset)
		{
			return E_HS_DATASIZEERROR;
		}

		//拷贝内存到roomcache相应位置
		memcpy(m_arrRoomCache + nDataOffset, arrData, nDataSize);

		//更新roomchche实际大小
		m_nRoomCacheSize = nRoomCacheSize;

		return S_OK;
	}

	uint32_t GetRoomCacheSize()
	{
		return m_nRoomCacheSize;
	}

	uint8_t *GetRoomCache()
	{
		return m_arrRoomCache;
	}

	void SetRebulid()
	{
		m_isRebulid = true;
	}
	void SetRebulidSuccess()
	{
		m_isRebulid = false;
	}

	bool IsRebulid()
	{
		return m_isRebulid;
	}
private:
	void Reset()
	{
		m_nRoomID = enmInvalidRoomID;
		m_nServerID = enmInvalidServerID;
		m_nRoomCacheSize = 0;
		memset(m_arrRoomCache, 0, sizeof(m_arrRoomCache));
		m_isRebulid = false;
		m_arrReqDate.clear();
	}

private:
	RoomID 						m_nRoomID;
	ServerID					m_nServerID;
	uint32_t					m_nRoomCacheSize;
	uint8_t						m_arrRoomCache[RoomCacheSize];
	bool 						m_isRebulid;		//是否在被恢复到其他的server上
public:
	ReqDateArray				m_arrReqDate;	//获取房间id的请求数据
};

FRAME_ROOMDISPATCHER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_ROOMDISPATCHER_NAMESPACE::CRoomCache)

#endif /* DAL_ROOMCACHE_H_ */
