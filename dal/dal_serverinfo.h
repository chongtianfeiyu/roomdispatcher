/*
 * dal_serverinfo.h
 *
 *  Created on: 2012-4-19
 *      Author: jimm
 */

#ifndef DAL_SERVERINFO_H_
#define DAL_SERVERINFO_H_

#include "../../lightframe/frame_typedef.h"
#include "def/server_namespace.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class ServerLoadInfo
{
public:
	uint32_t	serverID;
	int32_t		loadValue; //0 到100范围，100表示负载最大
	int64_t		nLastUpdateTime;
	NetType 	nNetType;	//网络类型
};


FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* DAL_SERVERINFO_H_ */
