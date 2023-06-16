#pragma once
#include <cstdint>


void KeyInputInit();
int	 KeyInputUpdate();
void KeyInputExit();

bool IsKeyOn( uint8_t key_id );
bool IsKeyRelease( uint8_t key_id );
bool IsKeyRepeat( uint8_t key_id );

bool CheckTriggerKey( uint8_t key_id );
bool CheckReleaseKey( uint8_t key_id );

bool PushHitKey(int key);
