/*
 * bll_event_serverstopnotice.h
 *
 *  Created on: 2012-4-23
 *      Author: jimm
 */

#ifndef BLL_EVENT_SERVERSTOPNOTICE_H_
#define BLL_EVENT_SERVERSTOPNOTICE_H_

#include "def/server_namespace.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"
#include "roomdispatcher_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CServerStopNoticeMessageEvent : public CBllBase, public ISystemEvent
{
public:
	CServerStopNoticeMessageEvent()
	{
	}

	virtual ~CServerStopNoticeMessageEvent()
	{
	}

	virtual int32_t OnSystemEvent(uint16_t nEventID, void *pParam);

private:
	int32_t RoomServerStop(ServerID nStopServerID,MessageHeadSS * pMsgHead);
	int32_t MediaServerStop(ServerID nStopServerID,MessageHeadSS * pMsgHead);

};

#define	CREATE_SERVERSTOPNOTICEEVENT_INSTANCE	CSingleton<CServerStopNoticeMessageEvent>::CreateInstance
#define	GET_SERVERSTOPNOTICEEVENT_INSTANCE		CSingleton<CServerStopNoticeMessageEvent>::GetInstance
#define	DESTROY_SERVERSTOPNOTICEEVENT_INSTANCE	CSingleton<CServerStopNoticeMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END


#endif /* BLL_EVENT_SERVERSTOPNOTICE_H_ */
