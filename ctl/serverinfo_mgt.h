/*
 * serverinfo_mgt.h
 *
 *  Created on: 2012-4-19
 *      Author: jimm
 */

#ifndef SERVERINFO_MGT_H_
#define SERVERINFO_MGT_H_

#include <vector>
#include <map>
#include "common/common_singleton.h"
#include "../../lightframe/frame_typedef.h"
#include "def/server_namespace.h"
#include "dal/dal_serverinfo.h"

using namespace std;

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

typedef map<ServerID, ServerLoadInfo>		ServerLoadInfoMap;

typedef struct stServerRange
{
	int32_t				minValue;
	int32_t				maxValue;
	ServerID			serverID;
}ServerRange;

typedef vector<ServerRange>		ServerRangeArray;

class CServerLoadManager
{
public:
	CServerLoadManager();
	~CServerLoadManager();

	void				AddServerLoad(const ServerLoadInfo& info);
	void				UpdateServerLoad(const ServerLoadInfo& info);
	void				DelServerLoad(ServerID serverID);
	//选取一个适合的SERVER，并返回ID,如果-1表示失败
	ServerID			SelectServer();
	//选取一个适合的Server,并返回选取服务器的具体信息
	bool				SelectServer(ServerLoadInfo& info);
	ServerID 			SelectMediaServer(NetType nNetType);\
	bool IsFind(ServerID serverID)
	{
		if(m_sInfoMap.find(serverID)==m_sInfoMap.end())
		{
			return false;
		}
		return true;
	}

private:
	ServerID			LocateServer(ServerRangeArray arrServerRange,int32_t nWholeSection);
private:
	ServerLoadInfoMap	m_sInfoMap;				//服务器负载信息列表
	ServerRangeArray	m_arrServerRange;		//一定时期的概率区间表
	bool				m_bCreateRange;			//概率区间表是否要进行重建
	int32_t				m_nWholeSection;		//概率全区间
};

//#define	CREATE_SERVERLOADMANAGER_INSTANCE		CSingleton<CServerLoadManager>::CreateInstance
//#define	GET_SERVERLOADMANAGER_INSTANCE			CSingleton<CServerLoadManager>::GetInstance
//#define	DESTROY_SERVERLOADMANAGER_INSTANCE		CSingleton<CServerLoadManager>::DestroyInstance
//
//#define g_ServerLoadManager		GET_SERVERLOADMANAGER_INSTANCE()

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* SERVERINFO_MGT_H_ */
