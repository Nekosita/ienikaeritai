#include "KeyInput.h"
#include <DxLib.h>
#include <memory>
#include <cstdint>
#include <vector>


namespace
{

	const unsigned int MAX_KEY_NUM = 256;
	std::vector<char>  key_old(MAX_KEY_NUM);
	std::vector<char>  key_now(MAX_KEY_NUM);

}	 // namespace

// 初期化(必要なし)
//----------------------------------------------------------------------------
void KeyInputInit() {}

// 更新
//----------------------------------------------------------------------------
int KeyInputUpdate()
{
	memcpy(&key_old[ 0 ], &key_now[ 0 ], key_now.size());
	GetHitKeyStateAll(&key_now[ 0 ]);

	return 0;
}

// 終了(必要なし)
//----------------------------------------------------------------------------
void KeyInputExit() {}
//----------------------------------------------------------------------------
//! 特定キーの1回だけ押下検証
//! @param key_id チェックするキー
//! @retval true 押されている
//! @retval false 押されてない
//----------------------------------------------------------------------------
bool IsKeyOn(uint8_t key_id)
{
	// key がいま押される
	// key が1フレーム前に押されてない
	return (key_now[ key_id ] != 0) && (key_old[ key_id ] == 0);
}

//DxLib風の名前での関数
//---------------------------------------------------------------------------
bool CheckTriggerKey(uint8_t key_id)
{
	return IsKeyOn(key_id);
}

//特定キーの解放検証
//----------------------------------------------------------------------------
bool IsKeyRelease(uint8_t key_id)
{
	return (key_now[ key_id ] == 0) && (key_old[ key_id ] != 0);
}

//DxLib風の名前での関数
bool CheckReleaseKey(uint8_t key_id)
{
	return IsKeyRelease(key_id);
}

//特定キーの長押し検証(2フレーム)
//----------------------------------------------------------------------------
bool IsKeyRepeat(uint8_t key_id)
{
	return (key_now[ key_id ] == 1 && key_old[ key_id ] == 1);
}

//キーが押された瞬間を取得する
//---------------------------------------------------------------------------------
bool PushHitKey(int key)
{
	return (key_now[ key ] != 0) && (key_old[ key ] == 0);
}