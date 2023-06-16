#pragma once

//! サウンドのベースクラス
//===============================================
class SoundEffect
{
private:

	int volume;//!<音量

	int get_sound;//!<アイテム拾った時の音楽
	int effect_sound;//!<選択する時のエフェクトの音楽


public:

	void ResetVolume();
	int SetVolume() ;
	int GetVolume() const;

	void PlayGetSound();
	void PlayEffectSound();
	void InitEffectSound();

	void Update();
	void Exit();

};

