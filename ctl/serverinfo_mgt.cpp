/*
 * serverinfo_mgt.cpp
 *
 *  Created on: 2012-4-19
 *      Author: jimm
 */

#include "common/common_datetime.h"
#include "def/dal_def.h"
#include "serverinfo_mgt.h"
#include "media_mgt.h"
#include "server_datacenter.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

const int32_t MAX_LOAD_VALUE = 100;

CServerLoadManager::CServerLoadManager() : m_bCreateRange(true), m_nWholeSection(0)
{

}

CServerLoadManager::~CServerLoadManager()
{
	m_sInfoMap.clear();
	m_arrServerRange.clear();
}


ServerID CServerLoadManager::LocateServer(ServerRangeArray arrServerRange,int32_t nWholeSection)
{
	ServerID nServerID = enmInvalidServerID;
	int32_t seed = rand() % nWholeSection;

	//采用2分查找定位对应的掉落区间
	int32_t maxPos = arrServerRange.size();
	int32_t minPos = 0;
	int32_t pos = 0;
	do
	{
		pos = (maxPos + minPos) / 2;

		if(pos > arrServerRange.size() - 1)
		{
			nServerID = arrServerRange[arrServerRange.size() - 1].serverID;
			break;
		}

		if(pos < 0)
		{
			nServerID = arrServerRange[0].serverID;
			break;
		}

		if(arrServerRange[pos].maxValue >= seed && arrServerRange[pos].minValue <= seed)
		{
			nServerID = arrServerRange[pos].serverID;
			break;
		}
		else if(arrServerRange[pos].maxValue < seed)
		{
			minPos = pos + 1;
		}
		else if(arrServerRange[pos].minValue > seed)
		{
			maxPos = pos - 1;
		}
	}while(true);

	return nServerID;
}

ServerID CServerLoadManager::SelectServer()
{
	if(m_bCreateRange)
	{
		m_nWholeSection = 0;

		m_arrServerRange.clear();

		int32_t nLeftLoadValue = 0;
		ServerRange range = {0};
		for(ServerLoadInfoMap::const_iterator it = m_sInfoMap.begin(); it != m_sInfoMap.end();)
		{
			//load >= 100的节点根本不会参与选取
			if(it->second.loadValue >= MAX_LOAD_VALUE)
			{
				++it;
				continue;
			}

			if(it->second.nLastUpdateTime + ServerInfoExpireTime < CDateTime::CurrentDateTime().Seconds())
			{
				ServerLoadInfoMap::const_iterator it_temp = it++;
				m_sInfoMap.erase(it_temp->first);
				continue;
			}

			nLeftLoadValue = MAX_LOAD_VALUE - it->second.loadValue;

			range.serverID = it->second.serverID;
			range.minValue = m_nWholeSection;
			m_nWholeSection += nLeftLoadValue;
			range.maxValue = m_nWholeSection - 1;

			m_arrServerRange.push_back(range);
			++it;
		}
		m_bCreateRange = false;
	}

	if(m_nWholeSection <= 0)
	{
		return enmInvalidServerID;
	}

	return LocateServer(m_arrServerRange,m_nWholeSection);
}

ServerID CServerLoadManager::SelectMediaServer(NetType nNetType)
{
	ServerRangeArray arrServerRange;

	m_nWholeSection = 0;
	arrServerRange.clear();

	int32_t nLeftLoadValue = 0;
	ServerRange range = {0};
	for(ServerLoadInfoMap::const_iterator it = m_sInfoMap.begin(); it != m_sInfoMap.end();)
	{
		//load >= 100的节点根本不会参与选取
		if(it->second.nNetType!=nNetType)
		{
			++it;
			continue;
		}
		if(it->second.loadValue >= MAX_LOAD_VALUE)
		{
			++it;
			continue;
		}

		CMedia * pMediaServer = NULL;
		g_MediaMgt.GetMedia(it->second.serverID,pMediaServer);
		if(pMediaServer == NULL)
		{
			++it;
			continue;
		}

		if(pMediaServer->GetPartServerID() == enmInvalidServerID)
		{
			++it;
			continue;
		}

		if(it->second.nLastUpdateTime + ServerInfoExpireTime < CDateTime::CurrentDateTime().Seconds())
		{
			ServerLoadInfoMap::const_iterator it_temp = it++;
			m_sInfoMap.erase(it_temp->first);
			continue;
		}

		nLeftLoadValue = MAX_LOAD_VALUE - it->second.loadValue;

		range.serverID = it->second.serverID;
		range.minValue = m_nWholeSection;
		m_nWholeSection += nLeftLoadValue;
		range.maxValue = m_nWholeSection - 1;

		arrServerRange.push_back(range);
		++it;
	}

	if(m_nWholeSection <= 0)
	{
		return enmInvalidServerID;
	}

	return LocateServer(arrServerRange,m_nWholeSection);
}

bool CServerLoadManager::SelectServer(ServerLoadInfo& info)
{
	ServerID id = SelectServer();
	if(id <= 0)
	return false;

	ServerLoadInfoMap::iterator it = m_sInfoMap.find(id);
	if(it != m_sInfoMap.end())
	{
		info = it->second;
		return true;
	}

	return false;
}

void CServerLoadManager::AddServerLoad(const ServerLoadInfo &info)
{
	m_sInfoMap[info.serverID] = info;
	m_bCreateRange = true;
}

void CServerLoadManager::UpdateServerLoad(const ServerLoadInfo &info)
{
	AddServerLoad(info);
}

void CServerLoadManager::DelServerLoad(ServerID serverID)
{
	m_sInfoMap.erase(serverID);
	m_bCreateRange = true;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

