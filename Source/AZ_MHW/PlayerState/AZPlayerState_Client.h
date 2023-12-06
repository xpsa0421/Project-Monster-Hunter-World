// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerState.h>
#include "AZ_MHW/Item/AZItemData.h"
#include "AZPlayerState_Client.generated.h"

/** UpdatePlayerState Helper*/
enum class ECharacterStateType
{
	//statics
	health = 0,
	stamina = 1,
	sharpness = 2,
	critical = 3,
	speed = 4,
	armor = 5,
	//elemental_bonus_type
	//elemental_bonus_type
	//elemental_bonus_type
	//elemental_bonus_type
	//elemental_bonus_type
	//elemental_bonus_type
	//elemental_resistance_type
	//elemental_resistance_type
	//elemental_resistance_type
	//elemental_resistance_type
	//elemental_resistance_type
	//elemental_resistance_type

	//animation
	anim_mask = 50,
	priority = 51,
	anim_state = 52,
};

/** 플레이어 액션(인풋) 상태 <-플레이어 컨트롤러가 갱신
 * 이외에도 추가할 것,
 * 아이템목록불러오기 메뉴 키도 추가하되, 서버에서는 처리안하기.
 */
USTRUCT(BlueprintType)
struct FAZPlayerActionState
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator input_direction;//캐릭터의 입력방향(플레이어의 시선방향(월드방향)기준 + 입력벡터)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 input_bitmask;
	
	FAZPlayerActionState()
	{
		//bitfield = Initializer 선언초기화 문법은 C++20에서가능
		//비트필드는 생성자에서 초기화해야한다.
		input_direction = FRotator::ZeroRotator;//플레이어가 인지하는 입력방향
		input_bitmask = 0;
	}
};
/** 플레이어 캐릭터의 상태<-플레이어 캐릭터가 갱신
 * 
 */
USTRUCT(BlueprintType)
struct FAZPlayerCharacterState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector character_position;//현재 캐릭터의 위치;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator character_direction;//현재 캐릭터의 전방방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 character_action_bitmask;//현재 캐릭터의 액션 상태
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 character_action_priority;//현재 캐릭터의 액션 우선도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 max_health_point;//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 current_health_point;//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 max_stamina;//스태미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 current_stamina;//스태미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 max_sharpness;//예리도//무기에 따른 변화
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 current_sharpness;//현재 예리도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 current_attack_point;//현재 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 speed_rate;//스피드 10000배 (Cast<float>)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 critical_rate;//회심율? 치명타인가 10000배 (Cast<float>)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 armor;//방어도
	
	//무기속성
	//속성별저항력?
	//회피도

	//장비벞
	//세트벞
	
	//전투
	//벞//디벞목록?(Enum?)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_die:1;//현재 죽은 상태인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_super_armor:1;//피격 경직저항상태인제
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 damage_reduce_rate;//데미지 감소율 10000배(Cast<float>)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_hit:1;//현재 피격상태인지
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_airborne:1;//현재 체공상태인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_climb:1;//현재 등반액션중인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_swing:1;//스윙액션인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_swim:1;//수영액션인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_mount:1;//마운트상태
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_slide:1;//미끄러지는 중인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_crouch:1;//웅크리기
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_evade:1;//회피중인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_guard:1;//방어중인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_action:1;//액션상태

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_can_battle:1;//전투 가능상태인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_can_airborne:1;//낭떠러지인지(발 앞이)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_can_slide:1;//경사인지(발 앞이)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_can_vault:1;//상태인지(전방에)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_can_climb:1;//벽인지(전방에)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bit_can_swing:1;//절벽과 덩굴인지(전방에)

	
	FAZPlayerCharacterState()
	{
		//Character Statics State
		max_health_point = 100;
		current_health_point = 100;
		max_stamina = 100;
		current_stamina = 100;
		max_sharpness = 100;
		current_sharpness = 50;

		current_attack_point = 100;
		critical_rate = 0;//10000배
		speed_rate = 20000;//10000배
		armor = 1;

		//Character Animation State
		character_position = FVector::ZeroVector;
		character_direction = FRotator::ZeroRotator;

		character_action_bitmask = 0;//현재 애니메이션(몽타주+섹션)
		character_action_priority = -1;//현재 애니메이션 우선도

		bit_hit = false;
	}
};
/** 플레이어 캐릭터의 장비 상태<-서버가 갱신
 * 
 */
USTRUCT(BlueprintType)
struct FAZPlayerEquipmentState
{
	GENERATED_USTRUCT_BODY()
	
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//int32 gender_type;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 FaceItemID;//표정을?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector4f hair_color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 hair_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 head_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 body_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 waist_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 arm_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 leg_item_id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 weapon_type;//소켓메시, 인풋매핑컨텍스트, 몽타주 변경처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 first_weapon_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 second_weapon_item_id;
	
	//슬롯에 낀 소비아이템품목
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 consume_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ammo_item_id;
	
	FAZPlayerEquipmentState()
	{
		//FaceItemID = 0;;
		//gender_type = -1;//-1 F, 1 M
		hair_color = FVector4f::Zero();

		/*
		//테스트 장비
		const int32& test_set_id = 297;
		body_item_id = 10000 + test_set_id;
		leg_item_id = 10500 + test_set_id;
		arm_item_id = 11000 + test_set_id;
		waist_item_id = 11500 + test_set_id;
		head_item_id = 12000 + test_set_id;
		hair_item_id = 12501 + 31;
		*/
		
		//TODO: 릴리즈시 각주풀기 
		body_item_id = 10000+500;//Default
		leg_item_id = 10500+500;//Default
		arm_item_id = 11000+500;//Default
		waist_item_id = 11500+500;//Default
		head_item_id = 12000+500;//Default
		hair_item_id = 12501+0;//Default
		
		
		weapon_type = 0;//0~13 무기종류(14종)
		first_weapon_item_id = 1000+(weapon_type*500) + 003;//대검
		second_weapon_item_id = 0;//무기아이디
		
		consume_item_id = 101;
		ammo_item_id = 201;
	}
};
/** 플레이어 제스쳐? 표정? 머리색? 피부색? */
//FAZPlayerDecorationState?

/**
 * 클라이언트유저 소유정의 = PlayerController + PlayerPawn + PlayerState
 * 실질적인 소유는 없다. 서버에서 전부 만들어서 전달
 * 모든 유저데이터와 현재유저의 상태(어떤 조작, 어떤 메뉴 등)인지 담아두는 객체
 * 맵을 넘겨도 사라지지 않는다.(그렇기에 유저의 정보 담아두기)
 */
UCLASS()
class AZ_MHW_API AAZPlayerState_Client : public APlayerState
{
	GENERATED_BODY()
	
public:
	AAZPlayerState_Client();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
public:
	/** OnPawnSet AddDynamic Function*/
	UFUNCTION() void SetPlayerCharacter(APlayerState* player_state, APawn* new_pawn, APawn* old_pawn);
	
	/** 이 PlayerState 고유 ID */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 uid_;//User ID
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 cid_;//Character ID
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FString nickname_;//character Name
	
	//플레이어 입력상태(애니메이션)
	//원격PC는 입력상태값을 받아서 움직인다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAZPlayerActionState action_state_;
	
	//이 구조체는 위젯 정보용이다.(공식의 처리(실제 체크, 가감) 서버에서 처리된다.)
	//캐릭터 상태(HP, 스태미나, 예리도 등)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAZPlayerCharacterState character_state_;
	
	//캐릭터 착용장비
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAZPlayerEquipmentState equipment_state_;

	//캐릭터 인벤토리
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//TMap<int32, FItemInfo> inventory_state_;
	//이 구조체는 위젯 단지 인벤토리 정보출력용이다.(실제 사용은 서버에서 처리된다.)
	//예시: 물약을먹는다. 클라이언트에는 1개, 서버에는 0개있다고 가정
	//클라이언트가 이 상태에서 물약사용을 시도하면 물약섭취 애니메이션을 하긴하지만,
	//서버에서는 물약이 0개라 처리가 이루어지지못함으로 체력이 회복되지않고,
	//클라이언트에서도 체력이 회복되는 이벤트를 못받앗음으로 체력이 회복되지 않는다.
	//탄약등 다양한 아이템들이 모두 마찬가지,
	//로컬 클라이언트가 시도하면, 이 구조체의 갯수 조건체크로 애니메이션은 실행되지만
	//실제처리는 서버에서 검증후 결과이벤트로 처리된다.
};
