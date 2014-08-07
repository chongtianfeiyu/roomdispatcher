/*
 * dal_room.h
 *
 *  Created on: 2011-11-30
 *      Author: jimm
 */

#ifndef DAL_ROOM_H_
#define DAL_ROOM_H_

#include "common/common_complextype.h"
#include "common/common_hashlist.h"
#include "common/common_string.h"
#include "../../lightframe/frame_typedef.h"
#include "dal/dal_player.h"
#include "def/server_namespace.h"

#include "public_typedef.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

typedef CHashList<RoleID, MaxUserCountPerRoom, RoomPlayerBucketSize> RoomPlayerList;

class CRoom : public CObject
{
public:

	CRoom()
	{
		Reset();
	}

	virtual ~CRoom()
	{

	}

	int32_t Initialize()
	{
		Reset();
		return m_sRoomPlayerList.Initialize();
	}

	int32_t Resume()
	{
		Reset();
		return m_sRoomPlayerList.Resume();
	}

	int32_t Uninitialize()
	{
		Reset();
		return m_sRoomPlayerList.Uninitialize();
	}

	int32_t AddPlayer(const RoleID nRoleID);

	int32_t DeletePlayer(const RoleID nRoleID);

	bool IsPlayerInRoom(const RoleID nRoleID);

	int32_t GetAllPlayers(RoleID arrRoleID[], const int32_t arrSize, int32_t &nPlayerCount);

	int32_t DeleteAllPlayers();

	void SetRegionID(const RegionID nRegionID)
	{
		m_nRegionID = nRegionID;
	}

	RegionID GetRegionID() const
	{
		return m_nRegionID;
	}

	void SetChannelID(const ChannelID nChannelID)
	{
		m_nChannelID = nChannelID;
	}

	ChannelID GetChannelID() const
	{
		return m_nChannelID;
	}

	RoleID GetHostID() const
	{
		return m_nHostID;
	}

	void SetHostID(const RoleID nRoleID)
	{
		m_nHostID = nRoleID;
	}

	int32_t GetCurUserCount() const
	{
		return m_nCurUserCount;
	}

	int32_t GetCurVisitorCount() const
	{
		return m_nCurVisitorCount;
	}

	int32_t GetTotalPeopleCount() const
	{
		return m_nCurUserCount + m_nCurVisitorCount;
	}

	int32_t GetMaxShowUserLimit() const
	{
		return m_nMaxShowUserLimit;
	}

	int32_t GetMaxRealUserLimit() const
	{
		return m_nMaxRealUserLimit;
	}

	RoomType GetRoomType() const
	{
		return m_nRoomType;
	}

	void SetRoomID(const RoomID nRoomID)
	{
		m_nRoomID = nRoomID;
	}

	RoomID GetRoomID() const
	{
		return m_nRoomID;
	}

	const char *GetRoomName()
	{
		return m_strRoomName.GetString();
	}

	void SetRoomName(const char *strRoomName)
	{
		m_strRoomName =  strRoomName;
	}

	void SetRoomServerID(const RoomID nRoomServerID)
	{
		m_nRoomServerID = nRoomServerID;
	}

	ServerID GetRoomServerID() const
	{
		return m_nRoomServerID;
	}

	int32_t	 GetRobotCount()
	{
		return m_nRobotCount;
	}

	void 	SetRobotCount(int32_t nRobotCount)
	{
		m_nRobotCount = nRobotCount;
	}

	int32_t	 GetTempAdminCount()
	{
		return m_nTempAdminCount;
	}

	void	 SetTempAdminCount(int32_t nTempAdminCount)
	{
		m_nTempAdminCount = nTempAdminCount;
	}

protected:
	void Reset()
	{
		m_nRoomServerID = enmInvalidServerID;
		m_nRegionID = enmInvalidRegionID;
		m_nChannelID = enmInvalidChannelID;
		m_nRoomID = enmInvalidRoomID;
		m_nRoomType = enmInvalidRoomType;
		m_nHostID = enmInvalidRoleID;
		m_nCurUserCount = 0;
		m_nCurVisitorCount = 0;
		m_nMaxShowUserLimit = MaxDefaultShowUserLimit;
		m_nMaxRealUserLimit = MaxDefaultRealUserLimit;
		m_sRoomPlayerList.Initialize();
		m_nRobotCount = 0;
		m_nTempAdminCount = 0;
	}

private:
	RoomPlayerList::Key MakePlayerKey(const RoleID nRoleID) const;

private:
	ServerID							m_nRoomServerID;					//���ڵ�roomserver
	RegionID							m_nRegionID;						//����ID
	ChannelID							m_nChannelID;						//Ƶ��ID
	RoomID								m_nRoomID;							//����ID
	CString<MaxRoomNameLength> 			m_strRoomName;						//���������
	RoomType							m_nRoomType;						//���������
	RoleID								m_nHostID;							//����roleid
	int32_t								m_nCurUserCount;					//��ǰ�����û������������ˣ������οͣ�
	int32_t								m_nCurVisitorCount;					//��ǰ�����ο�����
	int32_t								m_nMaxShowUserLimit;				//�ͻ�����ʾ������û�����
	int32_t								m_nMaxRealUserLimit;				//ʵ����ʵ�����ɵ��û�����

	int32_t								m_nRobotCount;						//�������������
	int32_t								m_nTempAdminCount;					//�����ٹ�����

	RoomPlayerList						m_sRoomPlayerList;						//��Ҷ����
};


FRAME_ROOMDISPATCHER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_ROOMDISPATCHER_NAMESPACE::CRoom)

#endif /* DAL_ROOM_H_ */
