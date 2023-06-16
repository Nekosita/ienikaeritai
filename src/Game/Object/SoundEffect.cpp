#include "SoundEffect.h"
#include"DxLib.h"



void SoundEffect::ResetVolume()
{
	volume = 0;
}

int SoundEffect::SetVolume()
{
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		volume += 5;
		if (volume > 255) {
			volume = 255;
		}
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		volume -= 5;
		if (volume < 0) {
			volume = 0;
		}
	}
	return volume;
}

int SoundEffect::GetVolume() const
{
	return volume;
}

void SoundEffect::PlayGetSound()
{
	PlaySoundMem(get_sound, DX_PLAYTYPE_BACK);
}

void SoundEffect::PlayEffectSound()
{
	PlaySoundMem(effect_sound, DX_PLAYTYPE_BACK);
}

void SoundEffect::InitEffectSound()
{
	//最初の音量
	volume = 100;
	get_sound = LoadSoundMem("Data/music/魔王魂 効果音 ワンポイント17.mp3");
	effect_sound = LoadSoundMem("Data/music/魔王魂 効果音 システム49.mp3");
    ChangeVolumeSoundMem(volume, effect_sound);
	ChangeVolumeSoundMem(volume, get_sound);}

void SoundEffect::Update()
{
	ChangeVolumeSoundMem(volume, get_sound);
	ChangeVolumeSoundMem(volume, effect_sound);
}

void SoundEffect::Exit()
{
	DeleteSoundMem(get_sound);
	DeleteSoundMem(effect_sound);
}
