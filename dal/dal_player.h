/*
 * dal_player.h
 *
 *  Created on: 2011-11-30
 *      Author: jimm
 */

#ifndef DAL_PLAYER_H_
#define DAL_PLAYER_H_

#include "common/common_complextype.h"
#include "common/common_object.h"
#include "common/common_string.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_protocolhead.h"
#include "def/server_namespace.h"
#include "public_typedef.h"
#include "def/server_errordef.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class PlayerRoomInfo
{
public:
	ServerID		nServerID;			//所在的roomserverid
	RoomID			nRoomID;			//进入的房间
	PlayerState		nPlayerState;		//在房间的状态

	PlayerRoomInfo()
	{
		Reset();
	}

	void Reset()
	{
		nRoomID			= enmInvalidRoomID;
		nPlayerState	= enmInvalidPlayerState;
		nServerID		= enmInvalidServerID;
	}
};

class CPlayer : public CObject
{
public:
	CPlayer()
	{
		Reset();
	}

	virtual ~CPlayer()
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

	int32_t GetCurEnterRoomCount() const
	{
		return m_nCurRoomCount;
	}

	void GetAllEnterRoom(RoomID arrRoomID[], const int32_t arrSize, int32_t &nGetCurRoomCount)
	{
		nGetCurRoomCount = m_nCurRoomCount > arrSize ? arrSize : m_nCurRoomCount;
		for(int32_t i = 0; i < nGetCurRoomCount; ++i)
		{
			arrRoomID[i] = m_arrPlayerRoomInfo[i].nRoomID;
		}
	}

	void GetPlayerRoomInfo(PlayerRoomInfo arrPlayerRoomInfo[], const int32_t arrSize, int32_t &nGetInfoCount)
	{
		nGetInfoCount = m_nCurRoomCount > arrSize ? arrSize : m_nCurRoomCount;
		memcpy(arrPlayerRoomInfo, m_arrPlayerRoomInfo, sizeof(PlayerRoomInfo) * nGetInfoCount);
	}

	void GetAllPlayerStateInAllRoom(PlayerState arrPlayerState[], const int32_t arrSize, int32_t &nGetPlayerStateCount)
	{
		nGetPlayerStateCount = m_nCurRoomCount > arrSize ? arrSize : m_nCurRoomCount;
		for(int32_t i = 0; i < nGetPlayerStateCount; ++i)
		{
			arrPlayerState[i] = m_arrPlayerRoomInfo[i].nPlayerState;
		}
	}

	void SetRoleID(const RoleID nRoleID)
	{
		m_nRoleID = nRoleID;
	}

	RoleID GetRoleID() const
	{
		return m_nRoleID;
	}

	void SetRoleName(const char *strRoleName)
	{
		m_strRoleName = strRoleName;
	}

	const char *GetRoleName()
	{
		return m_strRoleName.GetString();
	}

	void SetAccountID(AccountID nAccountID)
	{
		m_nAccountID = nAccountID;
	}

	AccountID GetAccountID()
	{
		return m_nAccountID;
	}

	void SetVipLevel(VipLevel nVipLevel)
	{
		m_nVipLevel = nVipLevel;
	}

	VipLevel GetVipLevel()
	{
		return m_nVipLevel;
	}

	void SetUserLevel(UserLevel nUserLevel)
	{
		m_nUserLevel = nUserLevel;
	}

	UserLevel GetUserLevel()
	{
		return m_nUserLevel;
	}

	void SetPlayerGender(const Gender nGender)
	{
		m_nPlayerGender = nGender;
	}

	Gender GetPlayerGender() const
	{
		return m_nPlayerGender;
	}

	void SetLoginTime(const int64_t nLoginTime)
	{
		m_nLoginTime = nLoginTime;
	}

	int64_t GetLoginTime() const
	{
		return m_nLoginTime;
	}

	void SetConnInfo(const ConnUin &stInfo)
	{
		m_stConnInfo = stInfo;
	}

	const ConnUin &GetConnInfo()
	{
		return m_stConnInfo;
	}

	void SetOnlineTime(uint64_t nOnlineTime)
	{
		m_nOnlineTime = nOnlineTime;
	}

	uint64_t GetOnlineTime()
	{
		return m_nOnlineTime;
	}

	void SetLeftMoney(uint32_t nLeftMoney)
	{
		m_nLeftMoney = nLeftMoney;
	}

	uint32_t GetLeftMoney()
	{
		return m_nLeftMoney;
	}

	void SetExperience(int32_t nExperience)
	{
		m_nExperience = nExperience;
	}

	int32_t GetExperience()
	{
		return m_nExperience;
	}

	void SetLastVersion(uint32_t nLastVersion)
	{
		m_nLastVersion = nLastVersion;
	}

	uint32_t GetLastVersion()
	{
		return m_nLastVersion;
	}

/*获取新加字段*/
	int32_t GetPlayerAdminCount()
	{
		return m_nAdminCount;
	}

	RoleRank GetAdminRoleRank(RoomID nRoomID)
	{
		RoleRank nRoleRank = enmRoleRank_None;
		for(int32_t i = 0; i < MaxBeAdminPerPlayer; ++i)
		{
			if(m_arrRoomIDS[i] == nRoomID)
			{
				nRoleRank = m_arrRoleRank[i];
				break;
			}
		}
		return nRoleRank;
	}

/*设置新加字段*/
	int32_t SetPlayerAdminCount(int32_t nAdminCount)
	{
		if(nAdminCount > MaxBeAdminPerPlayer)
		{
			return E_HS_ADMINOUTOFBOUND;
		}
		m_nAdminCount = nAdminCount;
		return S_OK;
	}

	//添加新的房间权限记录并更新数目
	int32_t SetAdminRoleRank(RoomID nRoomID, RoleRank nRoleRank)
	{
		for(int32_t i = 0; i < MaxBeAdminPerPlayer; ++i)
		{
			if(m_arrRoomIDS[i] == nRoomID)
			{
				if(nRoleRank == enmRoleRank_None)
				{
					m_arrRoleRank[i] = m_arrRoleRank[m_nAdminCount - 1];
					m_arrRoomIDS[i] = m_arrRoomIDS[m_nAdminCount - 1];
					m_arrRoleRank[m_nAdminCount - 1] = enmRoleRank_None;
					m_arrRoomIDS[m_nAdminCount - 1] = enmInvalidRoomID;
					--m_nAdminCount;
					return S_OK;
				}
				else
				{
					m_arrRoleRank[i] = nRoleRank;
					return S_OK;
				}
			}
		}
		if((m_nAdminCount < MaxBeAdminPerPlayer) && (m_arrRoomIDS[m_nAdminCount] == enmInvalidRoomID))
		{
			m_arrRoomIDS[m_nAdminCount] = nRoomID;
			m_arrRoleRank[m_nAdminCount] = nRoleRank;
			m_nAdminCount++;
			return S_OK;
		}
		//超过最大管理数返回E_HS_ADMINOUTOFBOUND
		return E_HS_ADMINOUTOFBOUND;
	}

	//判断玩家是否有资格添加权限
	int32_t IsCanAddTitle(RoomID nRoomID, RoleRank nAddRoleRank)
	{
		RoleRank nRoleRank = GetAdminRoleRank(nRoomID);
		switch(nAddRoleRank)
		{
		case enmRoleRank_MinorHost:
			if(nRoleRank >= enmRoleRank_MinorHost)
			{
				return E_HS_NOPERMISSIONS;
			}
			break;
		case enmRoleRank_Admin:
			if(nRoleRank >= enmRoleRank_Admin)
			{
					return E_HS_NOPERMISSIONS;
			}
			break;
		case enmRoleRank_TempAdmin:
			if(nRoleRank >= enmRoleRank_TempAdmin)
			{
					return E_HS_NOPERMISSIONS;
			}
			break;
		default:
			return E_HS_NOTEXISTROLERANK;
			break;
		}
		if(m_nVipLevel <= enmVipLevel_NONE)
		{
			return E_HS_NOPERMISSIONS;
		}
		for(int32_t i = 0; i < MaxBeAdminPerPlayer; ++i)
		{
			if(m_arrRoomIDS[i] == nRoomID)
			{
				continue;
			}
			//加临管的时候不判断在其他房间是否为管理员
			else if(m_arrRoleRank[i] != enmRoleRank_None&&nAddRoleRank!=enmRoleRank_TempAdmin&&m_arrRoleRank[i]!=enmRoleRank_TempAdmin)
			{
				return E_HS_ISOTHERROOMADMIN;
			}
		}
		return S_OK;
	}

	void SetPlayerType(PlayerType nPlayerType)
	{
		m_nPlayerType = nPlayerType;
	}

	PlayerType GetPlayerType()
	{
		return m_nPlayerType;
	}

	bool CanEnterRoom() const
	{
		return !(m_nCurRoomCount >= MaxEnterRoomCount);
	}

	void EnterRoom(const RoomID nRoomID, const ServerID nServerID)
	{
		if(!CanEnterRoom())
		{
			//todo : write something
			return;
		}

		m_arrPlayerRoomInfo[m_nCurRoomCount].nRoomID = nRoomID;
		m_arrPlayerRoomInfo[m_nCurRoomCount].nServerID = nServerID;
		m_arrPlayerRoomInfo[m_nCurRoomCount].nPlayerState = enmPlayerState_InRoom;

		++m_nCurRoomCount;
	}

	void ExitRoom(const RoomID nRoomID)
	{
		for(int32_t i = 0; i < m_nCurRoomCount; ++i)
		{
			if(nRoomID == m_arrPlayerRoomInfo[i].nRoomID)
			{
				//将最后一位填充删除的位置
				memcpy(&m_arrPlayerRoomInfo[i], &m_arrPlayerRoomInfo[m_nCurRoomCount - 1], sizeof(PlayerRoomInfo));

				m_arrPlayerRoomInfo[m_nCurRoomCount - 1].Reset();

				if(m_nCurRoomCount > 0)
				{
					--m_nCurRoomCount;
				}

				break;
			}
		}
	}

	void SetIdentityType(const IdentityType nIdentityType)
	{
		m_nIdentityType = nIdentityType;
	}

	IdentityType GetIdentityType()
	{
		return m_nIdentityType;
	}
	bool IsOfficialPlayer()
	{
		return ((m_nIdentityType&enmIdentityType_OPERATOR)==enmIdentityType_OPERATOR)||((m_nIdentityType&enmIdentityType_PATROLMAN)==enmIdentityType_PATROLMAN);
	}
private:
	void Reset()
	{
		m_nRoleID = enmInvalidRoleID;
		m_strRoleName.Initialize();
		m_nAccountID = enmInvalidAccountID;
		m_nVipLevel = enmInvalidVipLevel;
		m_nUserLevel = 0;
		m_nPlayerGender = enmGender_Unknown;
		m_nCurRoomCount = 0;
		memset(m_arrPlayerRoomInfo, 0, sizeof(m_arrPlayerRoomInfo));
		m_nLoginTime = 0;
		m_stConnInfo.Reset();
		m_nOnlineTime = 0;
		m_nLeftMoney = 0;
		m_nExperience = 0;
		m_nLastVersion = 0;
		/*处理新加字段*/
		m_nAdminCount = 0;
		memset(m_arrRoomIDS, enmInvalidRoomID, sizeof(m_arrRoomIDS));
		memset(m_arrRoleRank, enmRoleRank_None, sizeof(m_arrRoleRank));
		m_nPlayerType = enmPlayerType_Normal;
		m_nIdentityType = enmIdentityType_None;
	}
private:
	RoleID				m_nRoleID;
	CString<MaxRoleNameLength>	m_strRoleName;
	AccountID			m_nAccountID;
	VipLevel			m_nVipLevel;
	UserLevel			m_nUserLevel;
	Gender				m_nPlayerGender;
	int32_t				m_nCurRoomCount;									//当前进入的房间数
	PlayerRoomInfo		m_arrPlayerRoomInfo[MaxEnterRoomCount];
	int64_t				m_nLoginTime;										//登陆时间
	ConnUin				m_stConnInfo;
	uint64_t			m_nOnlineTime;
	uint32_t			m_nLeftMoney;
	int32_t				m_nExperience;
	uint32_t			m_nLastVersion;
	PlayerType			m_nPlayerType;
	/*新加字段,用户管理权限和相关房间*/
	int32_t 			m_nAdminCount;										//管理的房间数
	RoomID          	m_arrRoomIDS[MaxBeAdminPerPlayer];		//管理的房间ID
	RoleRank        	m_arrRoleRank[MaxBeAdminPerPlayer];		//管理权限
	IdentityType        m_nIdentityType;//玩家身份
};


FRAME_ROOMDISPATCHER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_ROOMDISPATCHER_NAMESPACE::CPlayer)

#endif /* DAL_PLAYER_H_ */


