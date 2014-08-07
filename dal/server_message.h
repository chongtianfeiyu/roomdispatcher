/*
 * server_message.h
 *
 *  Created on: 2012-2-12
 *      Author: jimm
 */

#ifndef SERVER_MESSAGE_H_
#define SERVER_MESSAGE_H_

#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_msg_impl.h"
#include "common/common_string.h"
#include "def/dal_def.h"
#include "public_typedef.h"
#include "roomserver_message_define.h"
#include "tunnelserver_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

//��������֪ͨ
class CCreateRoomNotice : public CCreateRoomNotice_Public
{
};

//���ٷ���֪ͨ
class CDestroyRoomNotice : public CDestoryRoomNotice_Public
{
};

//���뷿��֪ͨ
class CEnterRoomNotice : public CEnterRoomNotice_Public
{
};


//�˳�����֪ͨ
class CExitRoomNotice : public CExitRoomNotice_Public
{
};

//��½����
class CGetRoomServerIDReq : public CGetRoomServerIDReq_Public
{
};

class CGetRoomServerIDResp : public CGetRoomServerIDResp_Public
{
};

//�ϱ�������Ϣ��hall
class CUpdateInfoToHallNotice : public CUpdateInfoToHallNotice_Public
{
};

class CGetMediaInfoReq : public CGetMediaInfoReq_Public
{
};

class CGetMediaInfoResp : public CGetMediaInfoResp_Public
{
};


//add your message to map
MESSAGEMAP_DECL_BEGIN(roomdispatcher_from_server)

MESSAGEMAP_DECL_MSG(MSGID_RSMS_CREATE_ROOM_NOTICE, (new CCreateRoomNotice()))
MESSAGEMAP_DECL_MSG(MSGID_RSMS_DESTORY_ROOM_NOTICE, (new CDestroyRoomNotice()))
MESSAGEMAP_DECL_MSG(MSGID_RSMS_ENTER_ROOM_NOTICE, (new CEnterRoomNotice()))
MESSAGEMAP_DECL_MSG(MSGID_RSMS_EXIT_ROOM_NOTICE, (new CExitRoomNotice()))
MESSAGEMAP_DECL_MSG(MSGID_TSHS_GETROOMSERVERID_REQ, (new CGetRoomServerIDReq()))
MESSAGEMAP_DECL_MSG(MSGID_RSHS_UPDATEINFOTOHALL_NOTICE, (new CUpdateInfoToHallNotice()))
MESSAGEMAP_DECL_MSG(MSGID_RSHS_GETMEDIAINFO_REQ, (new CGetMediaInfoReq()))

MESSAGEMAP_DECL_END()

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* SERVER_MESSAGE_H_ */
