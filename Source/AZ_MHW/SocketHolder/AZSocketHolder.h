// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZSocketHolder.generated.h"

class UAZGameInstance;
enum class PACKET_ID : UINT16;

UENUM(BlueprintType)
enum class ESocketResult : uint8
{
	Success,
	Failed,
	Error
};

enum class ESocketRecvResult : uint8
{
	Success,
	Failed,
	Disconnected
};

class CAZSendDetailLoger
{
public:
	CAZSendDetailLoger() {}
	virtual ~CAZSendDetailLoger() {}

	virtual bool WriteLog(struct PACKET_HEADER* send_msg) = 0;
};

template<typename msg_type>
class TAZSendDetailLoger: public CAZSendDetailLoger
{
public:
	TAZSendDetailLoger() {}
	virtual ~TAZSendDetailLoger() {}

	virtual bool WriteLog(PACKET_HEADER* send_msg) override
	{
		msg_type* cast_msg = (msg_type*)(send_msg);
		if (cast_msg)
		{
			OnWriteLog(*cast_msg);
			return true;
		}

		UE_LOG(LogClass, Warning, TEXT("AZSendDetailLoger WriteLog Error"));
		return false;
	}

	void OnWriteLog(const msg_type& send_msg);
};

USTRUCT()
struct FAZWaitProtocol
{
	GENERATED_BODY()

	enum EWaitType { WaitAck, SendFailed };

	UPROPERTY(EditAnywhere) FString name_;
	UPROPERTY(EditAnyWhere) TArray<uint8> buffer_;
	UPROPERTY(EditAnywhere) uint64 hash_code_;
	UPROPERTY(EditAnywhere) uint8 write_detail_log_ : 1;
	EWaitType wait_type_;

	FAZWaitProtocol() { }
	FAZWaitProtocol(FString name) : name_(name), wait_type_(WaitAck) { }
	FAZWaitProtocol(FString name, const uint8* buffer, int32 size, uint64 hash_code, bool detail_log, EWaitType wait_type)
		: name_(name), hash_code_(hash_code), write_detail_log_(detail_log), wait_type_(wait_type)
	{
		buffer_.Empty(size);
		buffer_.Append(buffer, size);
	}
	bool operator ==(const FAZWaitProtocol& other) { return name_ == other.name_; }
	bool operator ==(const FString& name) { return name_ == name; }
};
/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZSocketHolder : public UObject
{
	GENERATED_BODY()
private:
	ESocketHolderType socket_holder_type_;

	FString connect_ip_;
	int32 connect_port_;

	int32 last_send_tag_number_;
	int32 last_recv_tag_number_;

	TWeakObjectPtr<UAZGameInstance> game_instance_;

public:
	UAZSocketHolder();

	void Init(ESocketHolderType socketType);
	void Disconnect();

	template<typename T>
	void Connect(FString ip, int32 port, T lambda_func_connect_result)
	{
		Disconnect();

		connect_ip_ = ip;
		connect_port_ = port;

		ESocketResult socket_result = ESocketResult::Success;

		if (!_Connect(ip, port))
		{
			socket_result = ESocketResult::Failed;
		}

		lambda_func_connect_result(socket_result);
	}

	bool SendPacket(PACKET_HEADER* pSendMsg, int packet_size);
	bool ProcessPacket(PACKET_HEADER* recv_packet);

	ESocketHolderType GetHolderType() { return socket_holder_type_; }

	void InitIsShowWaitWidgetException();
	void InitSendLogger();

	void ShowWaitingWidget(bool is_forced = false);
	bool IsShowWaitWidget(PACKET_HEADER* send_msg);
	void AddWaitProtocolData(FString name, const uint8* buffer, int32 size, uint64 hash_code, bool detail_log, FAZWaitProtocol::EWaitType wait_type = FAZWaitProtocol::WaitAck);
	void EraseWaitProtocol(TArray<FString> protocol);
	bool CheckContainWaitProtocol(FString check_packet_name);
	void ClearWaitProtocol();
	bool IsWaitingProtocolEmpty();
	void SafeHideWaitingWidget(bool is_clear);
	bool IsAlReadySendPacket(FString name);
	void OutRequestProtocol(PACKET_ID packet_id, FString& out_request_protocol);


	UFUNCTION() void GetRecvPacketName(FString recv_packet_name);
	TArray<FString> GetPacketNameArray(FString packet_name);
	void RemoveSeparateProtocolTag(TArray<FString>& protocol_name_tag);
	FString GetWaitProtocol(TArray<FString> ProtocolNameTag);

	bool WriteDetailLog(PACKET_HEADER* send_msg);
	void SendPendingMessage();
	int32 GetLastRecvTagNumber() const;

	bool IsOnConnectingServer();

	TArray<FString> GetWaitingProtocolNames() const;

public:
	template<typename MsgType>
	void _RegistLoger()
	{
		MsgType msgType;
		auto itFind = write_log_map_.Find(msgType.m_HashCode);
		if (itFind)
		{
			return;
		}

		AZSendDetailLoger* loger = new AZBaseSendDetailLoger<MsgType>;
		WriteLogMap.Add(msgType.m_HashCode, loger);
	}

	bool SendPendingPacket(const FAZWaitProtocol& send_msg);

	void ScreenWaitProc(FString packet_name);

private:
	bool _Connect(const FString& ip, int32 port);
	bool _IsHostDisconnected();

	UPROPERTY() TArray<uint64> is_show_wait_widget_exception_protocols_;
	UPROPERTY() TArray<FAZWaitProtocol> waiting_protocol_list_;

	TMap<uint64, CAZSendDetailLoger*> write_log_map_;
};

template <typename msg_type>
void TAZSendDetailLoger<msg_type>::OnWriteLog(const msg_type& send_msg)
{
	UE_LOG(LogTemp, Log, TEXT("packet_type : %d"), send_msg.packet_id);
}
