#include "AZ_MHW/SocketHolder/PacketFunction.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"

/*void UPacketFunction::RequestChatting(UINT32 client_index, LOGIN_REQUEST_PACKET* packet)
{
	LOGIN_REQUEST_PACKET login_res_packet;
	login_res_packet.packet_id = (int)PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS;
	login_res_packet.packet_length = sizeof(login_res_packet);
	//login_res_packet.clinet_id = client_index;
	strcpy_s(login_res_packet.user_id, sizeof(login_res_packet.user_id), packet->user_id);

	game_instance_->BroadCastSendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
}*/

// server(chat)
void UPacketFunction::ChatMsgCommand(UINT32 client_index, CS_CHAT_MSG_CMD* packet)
{

}

// client(chat)
void UPacketFunction::ChatMsgCommand(SC_CHAT_MSG_CMD* packet)
{

}