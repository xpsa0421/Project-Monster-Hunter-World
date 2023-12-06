#pragma once

#define PACKET_HEADER_SIZE 4
#include "AZWindows.h"

const UINT32 MAX_SOCKBUF = 256;	//패킷 크기
const UINT32 MAX_SOCK_SENDBUF = 4096;	// 소켓 버퍼의 크기
const UINT64 RE_USE_SESSION_WAIT_TIMESEC = 3;

enum class IOOperation
{
	ACCEPT,
	RECV,
	SEND
};

//WSAOVERLAPPED구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct stOverlappedEx
{
	WSAOVERLAPPED wsa_overlapped_;		//Overlapped I/O구조체
	WSABUF		wsa_buf_;				//Overlapped I/O작업 버퍼
	IOOperation E_operation_;			//작업 동작 종류
	UINT32 session_index_ = 0;
};

struct Test_Login_Send_Packet
{
	unsigned short packet_length;
	unsigned short packet_id;
	//bool packet_type;
	char user_id[33];
	char user_pw[33];

};
//typedef struct
//{
//	int32 PACKET_HEADER_SIZE2 = 5;
//	int MAX_USER_ID_BYTE_LENGTH = 33;
//	int MAX_USER_PW_BYTE_LENGTH = 33;
//	int MAX_CHAT_MSG_SIZE = 257;
//}HEADER_PACKET2;

//#define PACKET_CHAR_MSG   1000      // client ->
//#define PACKET_CHATNAME_REQ   1001  // server -> client
//#define PACKET_NAME_REQ   2000		// client -> server
//#define PACKET_NAME_ACK   3000		// server -> client
//#define PACKET_JOIN_USER  4000		// server -> client

class Defind
{
public:
	// Fstring to char
	char* FStringToCharArray(FString fString)
	{
		std::string stdString(TCHAR_TO_UTF8(*fString));
		char* charArray = new char[stdString.length() + 1];
		strcpy_s(charArray, sizeof(charArray), stdString.c_str());

		return charArray;
	}

	// char To FString
	FString CharArrayToFString(const char* CharArray)
	{
		// Convert char[] to std::string
		std::string StdString = CharArray;

		return FString(StdString.c_str());
	}
};
