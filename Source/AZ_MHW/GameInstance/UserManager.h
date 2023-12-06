#pragma once
#include "AZWindows.h"
#include "ErrorCode.h"
#include "User.h"
class UserManager
{
public:
	UserManager() = default;
	~UserManager() = default;
	void Init(const INT32 max_user_count)
	{
		max_user_cnt_ = max_user_count;
		// 유저 미리 생성해서 쭉 담기
		user_obj_pool_ = std::vector<User*>(max_user_cnt_);

		for (auto i = 0; i < max_user_cnt_; i++)
		{
			// user index 및 packet data buffer 생성
			user_obj_pool_[i] = new User();
			user_obj_pool_[i]->Init(i);
		}
	}

	INT32 GetCurrentUserCnt() { return current_user_cnt_; }

	INT32 GetMaxUserCnt() { return max_user_cnt_; }

	void IncreaseUserCnt() { current_user_cnt_++; }

	void DecreaseUserCnt()
	{
		if (current_user_cnt_ > 0)
		{
			current_user_cnt_--;
		}
	}

	// 새로운 연결이 되면 ClinetInfo 세팅 index와 1:1 매칭
	ERROR_CODE Adduser(char* user_id, int client_index)
	{
		auto user_idx = client_index;

		user_obj_pool_[user_idx]->SetLogin(user_id);
		user_id_dictionary_.insert(std::pair<char*, int>(user_id, client_index));

		return ERROR_CODE::NONE;
	}

	INT32 FindUserIndexByID(char* user_id)
	{
		if (auto res = user_id_dictionary_.find(user_id); res != user_id_dictionary_.end())
		{
			return (*res).second;
		}

		return -1;
	}

	void DeleteUserInfo(User* user)
	{
		user_id_dictionary_.erase(user->GetUserId());
		user->Clear();
	}

	User* GetUserByConnIdx(INT32 client_index)
	{
		return user_obj_pool_[client_index];
	}

private:
	INT32 max_user_cnt_ = 0;
	INT32 current_user_cnt_ = 0;

	std::vector<User*> user_obj_pool_; //vector로
	std::unordered_map<std::string, int> user_id_dictionary_;
};

