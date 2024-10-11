// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/Widget/AZWidget_Waiting.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/HUD/AZHUD.h"

UAZSocketHolder::UAZSocketHolder()
{
	last_send_tag_number_ = 0;
}

void UAZSocketHolder::Init(ESocketHolderType holder_type)
{
	socket_holder_type_ = holder_type;
	game_instance_ = Cast<UAZGameInstance>(GetOuter());
	InitIsShowWaitWidgetException();
	InitSendLogger();
}

void UAZSocketHolder::Disconnect()
{
	if (game_instance_->client_check == true)
	{
		game_instance_->Client_Shutdown();
	}
}

bool UAZSocketHolder::SendPendingPacket(const FAZWaitProtocol& send_msg)
{
	if (game_instance_->client_check == false)
	{
		return false;
	}

	static uint8 send_msg_buffer[200000];
	memcpy(send_msg_buffer, send_msg.buffer_.GetData(), send_msg.buffer_.Num());

	int len = game_instance_->Server_Packet_Send((char*)send_msg_buffer, send_msg.buffer_.Num());

	bool write_log = true;
	if (write_log)
	{
		if (!send_msg.write_detail_log_)
		{
			UAZUtility::ShippingLog(FString::Printf(TEXT("[UAZSocketHolder::SendPacket] %s Success!! bytesSent=%d"), *send_msg.name_, len));
		}
	}
	return true;
}

void UAZSocketHolder::ScreenWaitProc(FString packet_name)
{
	TArray<FString> protocol_name_tag = GetPacketNameArray(packet_name);
	EraseWaitProtocol(protocol_name_tag);
}

bool UAZSocketHolder::_Connect(const FString& ip, int32 port)
{
	return game_instance_->Server_Connect(ip, port);
}

bool UAZSocketHolder::_IsHostDisconnected()
{
	if (game_instance_->client_check == false)
	{
		return true;
	}
	return false;
}

bool UAZSocketHolder::SendPacket(PACKET_HEADER* send_msg, int packet_size)
{
	if (game_instance_->client_check == false)
	{
		return false;
	}

	if (IsShowWaitWidget(send_msg))
	{
		ShowWaitingWidget(false);
	}

	AZ_PRINT_LOG_IF_FALSE(send_msg, false);
	//int len = game_instance_->Server_Packet_Send((char*)send_msg, packet_size);
	// 메모리 카피로 대체
	memcpy(game_instance_->send_buffer_ + game_instance_->send_buffer_offset_, send_msg, packet_size);
	game_instance_->send_buffer_offset_ += packet_size;

	if (IsShowWaitWidget(send_msg) == true)
	{
		FString packet_name = UAZUtility::EnumToString<PACKET_ID>((PACKET_ID)send_msg->packet_id);
		//auto hash_code = std::hash<unsigned short>((unsigned short)send_msg->packet_id);
		auto hash_code = GetTypeHash(send_msg->packet_id);
		AddWaitProtocolData(packet_name, (uint8*)send_msg, packet_size, hash_code, WriteDetailLog(send_msg), FAZWaitProtocol::WaitAck);
	}

	if (!WriteDetailLog(send_msg))
	{
		FString packet_name = UAZUtility::EnumToString<PACKET_ID>((PACKET_ID)send_msg->packet_id);
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UAZSocketHolder::Send] %s byteSent"), *packet_name, packet_size));
	}

	return true;
}

void UAZSocketHolder::InitIsShowWaitWidgetException()
{
	is_show_wait_widget_exception_protocols_.Empty();
	// 로딩창 막는 패킷
	is_show_wait_widget_exception_protocols_.Emplace((unsigned short)PACKET_ID::CS_CHAT_MSG_CMD);
	is_show_wait_widget_exception_protocols_.Emplace((unsigned short)PACKET_ID::CS_COMBAT_MAP_LOAD_FINISH_CMD); // TEMP
	is_show_wait_widget_exception_protocols_.Emplace((unsigned short)PACKET_ID::CS_MONSTER_UPDATE_REQ);
}

void UAZSocketHolder::InitSendLogger()
{
	// 추가하기
	//_RegistLoger<FCG_ATTACK_SKILL_SHOOT_SYN>()
}

void UAZSocketHolder::ShowWaitingWidget(bool is_forced)
{
	UAZWidget_Waiting* waiting_widget = nullptr;
	if (game_instance_.IsValid() && game_instance_->GetHUD())
	{
		//waiting_widget = game_instance_->GetHUD()->OpenUI<UAZWidget_Waiting>(EUIName::AZWidget_Waiting, true);
	}

	if (waiting_widget == nullptr)
	{
		return;
	}

	if (is_forced == true)
	{
		waiting_widget->OnForceWaiting();
	}
}

bool UAZSocketHolder::IsShowWaitWidget(PACKET_HEADER* send_msg)
{
	AZ_PRINT_LOG_IF_FALSE(send_msg, false);
	auto hash_code = GetTypeHash(send_msg->packet_id);
	if (is_show_wait_widget_exception_protocols_.Contains(hash_code))
	{
		return false;
	}

	return true;
}

void UAZSocketHolder::AddWaitProtocolData(FString name, const uint8* buffer, int32 size, uint64 hash_code, bool detail_log, FAZWaitProtocol::EWaitType wait_type)
{
	waiting_protocol_list_.Emplace(FAZWaitProtocol(name, buffer, size, hash_code, detail_log, wait_type));
}

void UAZSocketHolder::EraseWaitProtocol(TArray<FString> protocol_name_tag)
{
	FString remove_protocol_name = GetWaitProtocol(protocol_name_tag);
	if (!remove_protocol_name.IsEmpty())
	{
		for (int i = 0; i < waiting_protocol_list_.Num();)
		{
			if (waiting_protocol_list_[i] == remove_protocol_name)
			{
				waiting_protocol_list_.RemoveAt(i);
			}
			else
			{
				++i;
			}
		}
	}

	if (game_instance_->IsWaitingProtocolEmpty())
	{
		SafeHideWaitingWidget(true);
	}
}

bool UAZSocketHolder::CheckContainWaitProtocol(FString check_packet_name)
{
	for (FAZWaitProtocol& protocol : waiting_protocol_list_)
	{
		if (protocol.name_ == check_packet_name)
		{
			return true;
		}
	}

	return false;
}

void UAZSocketHolder::ClearWaitProtocol()
{
	waiting_protocol_list_.Empty();
}

bool UAZSocketHolder::IsWaitingProtocolEmpty()
{
	return waiting_protocol_list_.Num() <= 0 ? true : false;
}

void UAZSocketHolder::SafeHideWaitingWidget(bool is_clear)
{
	if (is_clear)
	{
		UAZWidget_Waiting::ClearForceWaiting();
	}

	if (game_instance_->GetHUD())
	{
		if (game_instance_->GetHUD()->IsInViewportWaitingWidget())
		{
			game_instance_->GetHUD()->CloseUI(EUIName::AZWidget_Waiting, true);
		}
	}
}

bool UAZSocketHolder::IsAlReadySendPacket(FString name)
{
	for (auto& protocol : waiting_protocol_list_)
	{
		if (protocol.name_.Equals(name))
		{
			return true;
		}
	}
	return false;
}

void UAZSocketHolder::OutRequestProtocol(PACKET_ID packet_id, FString& out_request_protocol)
{
	out_request_protocol = UAZUtility::EnumToString(packet_id);
}

void UAZSocketHolder::GetRecvPacketName(FString recv_packet_name)
{
	TArray<FString> protocol_name_tag = GetPacketNameArray(recv_packet_name);
	EraseWaitProtocol(protocol_name_tag);
}

TArray<FString> UAZSocketHolder::GetPacketNameArray(FString packet_name)
{
	TArray<FString> protocol_name_tag;
	packet_name.ParseIntoArray(protocol_name_tag, TEXT("_"));
	RemoveSeparateProtocolTag(protocol_name_tag);
	return protocol_name_tag;
}

void UAZSocketHolder::RemoveSeparateProtocolTag(TArray<FString>& protocol_name_tag)
{
	int32 tag_size = protocol_name_tag.Num();
	int32 end_tag_index = tag_size - 1;

	if (1 >= end_tag_index || 2 >= tag_size)
	{
		return;
	}

	protocol_name_tag.RemoveAt(end_tag_index);
	protocol_name_tag.RemoveAt(0);
}

FString UAZSocketHolder::GetWaitProtocol(TArray<FString> protocol_name_tag)
{
	for (const auto& protocol : waiting_protocol_list_)
	{
		FString key = protocol.name_;
		TArray<FString> diff_string_array = GetPacketNameArray(key);

		if (diff_string_array.Num() != protocol_name_tag.Num())
		{
			continue;
		}

		bool find = true;
		for (FString tag : protocol_name_tag)
		{
			if (key.Contains(tag) == false)
			{
				find = false;
				break;
			}
		}

		if (find == true)
		{
			return key;
		}
	}
	return TEXT("");
}

bool UAZSocketHolder::WriteDetailLog(PACKET_HEADER* send_msg)
{
	if (send_msg)
	{
		auto hash_code = GetTypeHash(send_msg->packet_id);
		if (auto it_find = write_log_map_.Find(hash_code))
		{
			CAZSendDetailLoger* loger = *it_find;
			return loger->WriteLog(send_msg);
		}
	}

	return false;
}

void UAZSocketHolder::SendPendingMessage()
{
	for (int i = 0; i < waiting_protocol_list_.Num();)
	{
		if (waiting_protocol_list_[i].wait_type_ == FAZWaitProtocol::SendFailed)
		{
			SendPendingPacket(waiting_protocol_list_[i]);
			++i;
		}
		else
		{
			waiting_protocol_list_.RemoveAt(i);
		}
	}
}

int32 UAZSocketHolder::GetLastRecvTagNumber() const
{
	return last_recv_tag_number_;
}

bool UAZSocketHolder::IsOnConnectingServer()
{
	return game_instance_->sock && (game_instance_->recevie_connected == true);
}

TArray<FString> UAZSocketHolder::GetWaitingProtocolNames() const
{
	TArray<FString> result;
	for (auto& wp : waiting_protocol_list_)
	{
		result.Emplace(wp.name_);
	}
	return result;
}

