#pragma once
#include "Define.h"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum.hpp"

enum class PACKET_ID : UINT16
{
	MIN_PACKET_ID = 1,

	//SYSTEM
	SYS_USER_CONNECT = 11,
	SYS_USER_DISCONNECT = 12,
	SYS_END = 30,

	//DB
	DB_END = 99,

	CS_LOGIN_SIGNIN_REQ = 1000,
	SC_LOGIN_SIGNIN_RES = 1001,
	CS_LOGIN_SIGNUP_REQ = 1002,
	SC_LOGIN_SIGNUP_RES = 1003,

	CS_CHAT_MSG_CMD = 1010,
	SC_CHAT_MSG_CMD = 1011,

	CS_ITEM_TOTAL_INFO_REQ = 1100,
	SC_ITEM_TOTAL_INFO_RES = 1101,
	CS_ITEM_UNEQUIP_REQ = 1102,
	SC_ITEM_UNEQUIP_RES = 1103,
	CS_ITEM_EQUIP_REQ = 1104,
	SC_ITEM_EQUIP_RES = 1105,
	CS_ITEM_MOVE_REQ = 1106,
	SC_ITEM_MOVE_RES = 1107,
	CS_ITEM_CREATE_REQ = 1108,
	SC_ITEM_CREATE_RES = 1109,
	CS_ITEM_USE_REQ = 1110,
	SC_ITEM_USE_RES = 1111,
	SC_ITEM_INFO_CMD = 1112,

	// Map----------------------------
	// Client -> Server
	CS_COMBAT_MAP_ENTER_REQ = 2000, 
	CS_COMBAT_MAP_LOAD_FINISH_CMD = 2001,
	
	// End of Map---------------------

	// Monster------------------------
	// Server -> Client
	SC_MONSTER_SPAWN_CMD = 7001,
	SC_MONSTER_SPAWN_END_CMD = 7002,
	SC_MONSTER_TRANSFORM_CMD = 7010,
	SC_MONSTER_BODY_STATE_CMD = 7011,
	SC_MONSTER_ACTION_START_CMD = 7100,
	SC_MONSTER_ACTION_END_CMD = 7101,
	SC_MONSTER_ENTER_COMBAT_CMD = 7200,
	SC_MONSTER_PART_CHANGE_CMD = 7201,
	SC_MONSTER_HIT_CMD = 7202,
	SC_MONSTER_ENRAGE_BEGIN_CMD = 7300,
	SC_MONSTER_ENRAGE_END_CMD = 7301,
	SC_MONSTER_DIE_CMD = 7302,
	
	// Client -> Server
	CS_MONSTER_UPDATE_REQ = 7500,
	
	// End of Monster-----------------

#pragma region Character
	//CLIENT(UI)
	//LOGIN SCREEN(UI)
	//로그인시
	//CS_PLAYER_PLAYABLE_GUID_REQ=9000,//아이디번호 요청(플레이어 번호? GUID)

	//CHARACTER SELECT SCREEN(메인메뉴에서 받아서 가지고 있다가 선택창에서 마네퀸 플레이어 액터생성후)
	CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ = 9001,//아이디 번호로 플레이어 캐릭터데이터(생김새만) 요청
	//DB에서 캐릭터 데이터 가져와서 보유상태로 넘겨줌
	SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES = 9002,//플레이어 캐릭터데이터(생김새만) 응답

	// 권성호 추가
	CS_PLAYER_CHARACTER_CREATE_REQ = 9201, // 캐릭터 생성 추가
	SC_PLAYER_CHARACTER_CREATE_RES = 9202, // 캐릭터 생성 응답

	//CHARACTER SELECT CREATE & DESTROY(UI)
	CS_PLAYER_PLAYABLE_CHARACTER_CREATE_REQ = 9003,//캐릭터창에서 캐릭터 정보 생성 요청
	//생성후 결과(캐릭터 데이터 = 캐릭터 번호)넘겨줌
	SC_PLAYER_PLAYABLE_CHARACTER_CREATE_RES = 9004,//캐릭터창에서 캐릭터 정보 생성 응답
	CS_PLAYER_PLAYABLE_CHARACTER_DESTROY_REQ = 9005,//캐릭터창에서 캐릭터 정보 제거 요청
	//제거후 결과 넘겨줌
	SC_PLAYER_PLAYABLE_CHARACTER_DESTROY_RES = 9006,//캐릭터창에서 캐릭터 정보 제거 응답
	
	//CHARACTER SELECT IN GAME(UI)
	CS_PLAYER_CHARACTER_SELECT_REQ = 9007,//캐릭터 선택함.->해당 캐릭터 데이터(인벤까지) 요청
	//->캐릭터 정보전체 DB로 정보 획득후, 남은 정보는 날리기
	SC_PLAYER_CHARACTER_SELECT_RES = 9008,//캐릭터 선택함.->해당 캐릭터 데이터(인벤까지) 응답
	//->데이터패킷 받아서 클라의 플레이어 스테이트의 캐릭터 정보 초기화 및 월드변경
	
	//CLIENT
	//INGAME SCREEN
	CS_PLAYER_ORIGIN_CREATE_REQ=9101,//원본생성 요청 -> 원래는 응답받고 성공햇는지 체크해야하지만...
	CS_PLAYER_ORIGIN_DESTROY_REQ=9102,//원본제거 요청
	CS_PLAYER_ORIGIN_ACTION_REQ=9103,//원본액션 요청
	CS_PLAYER_ORIGIN_EQUIP_REQ=9104,//원본장비교체 요청
	CS_PLAYER_ORIGIN_CHECK_REQ=9105,//원본확인 요청
	//CS_PLAYER_ORIGIN_BUY_REQ=9105,//상점 구매 요청
	//CS_PLAYER_ORIGIN_SELL_REQ=9106,//상점 판매 요청

	CS_PLAYER_ORIGIN_GESTURE_REQ=9107,//원본 씨퀀스 요청
	SC_PLAYER_REMOTABLE_GESTURE_CMD=9108,//원격 씨퀀스 명령 전파
	
	//SERVER
	//INGAME SCREEN
	SC_PLAYER_STATE_REMOTABLE_CREATE_CMD=9500,//원격 플레이어 상태 전파
	SC_PLAYER_REMOTABLE_CREATE_CMD=9501,//원격 플레이어 생성 전파
	SC_PLAYER_REMOTABLE_DESTROY_CMD=9502,//원격 플레이어 제거 전파
	SC_PLAYER_REMOTABLE_ACTION_CMD=9503,//원격 플레이어 액션 전파
	SC_PLAYER_REMOTABLE_EQUIP_CMD=9504,//원격 플레이어 장비 전파
	SC_PLAYER_REMOTABLE_UPDATE_CMD=9505,//원격 플레이어 갱신 전파
	SC_PLAYER_PLAYABLE_UPDATE_CMD=9506,//로컬 플레이어 상태갱신 명령

	SC_PLAYER_REMOTABLE_HIT_CMD= 9507,//원격 피격
	SC_PLAYER_PLAYABLE_HIT_CMD= 9508,//플레이어 피격
	

	SC_PLAYER_PLAYABLE_INTERPOLATION_CMD=9510,//
	
	//INVENTORY
	CS_PLAYER_PLAYABLE_INVENTORY_DATA_REQ = 9601,//인벤토리 데이터 요청
	SC_PLAYER_PLAYABLE_INVENTORY_START_RES = 9602,//인벤토리 데이터 시작 알림
	SC_PLAYER_PLAYABLE_INVENTORY_DATA_RES = 9603,//인벤토리 데이터패킷
	SC_PLAYER_PLAYABLE_INVENTORY_END_RES = 9604,//인벤토리 데이터 끝 알림

	//
	SC_ENVIRONMENT_UPDATE_MERCHANT_CMD = 9901,//상점 정보 갱신 전파(구매, 판매시)
	SC_ENVIRONMENT_UPDATE_FIELD_CMD=9902,//필드 정보 갱신 전파(수집, 파괴 등)

	//TEMP
	CS_DEVELOP_PLAYER_FORCE_UPDATE_CMD = 9998,//클라에서 캐릭터 강제이동 임시
#pragma endregion
	//
	MAX_PACKET_ID = 9999,
};

template<>
struct magic_enum::customize::enum_range<PACKET_ID>
{
	static constexpr int min = 1;
	static constexpr int max = 10000;
};

//#pragma pack(push,1)
struct PACKET_HEADER
{
	UINT16 packet_length;
	UINT16 packet_id;
	INT32 success = 0;
};

#include "AZ_MHW/SocketHolder/Character/CharacterPacket.h"
#include "AZ_MHW/SocketHolder/Item/ItemPacket.h"
#include "AZ_MHW/SocketHolder/Chat/ChatPacket.h"
#include "AZ_MHW/SocketHolder/Login/LoginPacket.h"
#include "AZ_MHW/SocketHolder/Map/MapPacket.h"

#include "AZ_MHW/SocketHolder/Monster/MonsterPacket.h"
// pack(pop) //위에 설정된 패킹설정이 사라짐
