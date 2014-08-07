/*
 * dal_def.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef DAL_DEF_H_
#define DAL_DEF_H_

#include "server_namespace.h"
#include "../../lightframe/frame_typedef.h"
#include "common/common_list.h"
#include "common/common_def.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

#define SERVER_NAME_STRING		"roomdispatcher"
#define SERVER_VERSION			"Version: 1.0.build1, Release Date: 2011-12-02"

#define MaxTokenBuffSize		2048
//会话类型定义
enum
{
	enmSessionType_Invalid = 0x00,
	enmSessionType_GetRoleInfo = 0x01, //获取角色信息
	enmSessionType_GetRoomInfo = 0x02, //获取房间信息
};

//定时器类型定义
enum
{

};

//定时器超时时间定义
enum
{
	enmGetRoleInfoTimeoutPeriod = 3000 * US_PER_MS,
	enmGetRoomAdminCountTimeoutPerios = 3000 * US_PER_MS,
	enmSetSelfInfoTimeoutPerios = 3000 * US_PER_MS,
};

#define ServerInfoExpireTime	3 * SECOND_PER_MINUTE

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* DAL_DEF_H_ */
