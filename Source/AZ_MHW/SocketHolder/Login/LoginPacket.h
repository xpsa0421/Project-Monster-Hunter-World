#pragma once

struct CS_LOGIN_SIGNIN_REQ : public PACKET_HEADER
{
	char user_id[33];
	char user_pw[33];
	CS_LOGIN_SIGNIN_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_LOGIN_SIGNIN_REQ;
		packet_length = sizeof(CS_LOGIN_SIGNIN_REQ);
	}
};

struct SC_LOGIN_SIGNIN_RES : public PACKET_HEADER
{
	UINT32 client_index;
	SC_LOGIN_SIGNIN_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_LOGIN_SIGNIN_RES;
		packet_length = sizeof(SC_LOGIN_SIGNIN_RES);
	}
};

struct CS_LOGIN_SIGNUP_REQ : public PACKET_HEADER
{
	char user_id[33];
	char user_pw[33];
	CS_LOGIN_SIGNUP_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_LOGIN_SIGNUP_REQ;
		packet_length = sizeof(CS_LOGIN_SIGNUP_REQ);
	}
};

struct SC_LOGIN_SIGNUP_RES : public PACKET_HEADER
{
	SC_LOGIN_SIGNUP_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_LOGIN_SIGNUP_RES;
		packet_length = sizeof(SC_LOGIN_SIGNUP_RES);
	}
};