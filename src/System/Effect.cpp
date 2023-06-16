#include"Effect.h"
#include<EffekseerForDXLib.h>
#include<unordered_map>

namespace
{
	int effect_width = 1920;
	int effect_height = 1080;

	std::unordered_map< std::string, int > effects;

	int loadEffect(const std::string& filename)
	{
		if (effects.count(filename) <= 0)
		{
			effects[filename] = LoadEffekseerEffect(filename.c_str());
		}
		return effects[filename];
	}
}

Effect::Effect()
{
}

Effect::~Effect()
{

}

bool Effect::Load(const std::string& filename)
{
	effect_handle = loadEffect(filename);
	assert("エフェクトファイルがありません" &&
		effect_handle != INVALID_EFFECT);

	return effect_handle != INVALID_EFFECT;
}

bool Effect::Play2D(bool loop, float speed)
{
	playing_handle = PlayEffekseer2DEffect(effect_handle);
	is_3d = false;
	is_loop = loop;

	SetSpeedPlayingEffekseer2DEffect(playing_handle, speed);
	return playing_handle != INVALID_EFFECT;
}

bool Effect::Play3D(bool loop, float speed)
{
	playing_handle = PlayEffekseer3DEffect(effect_handle);
	is_3d = true;
	is_loop = loop;

	SetSpeedPlayingEffekseer3DEffect(playing_handle, speed);
	return playing_handle != INVALID_EFFECT;
}

void Effect::Update()
{
	if (is_3d)
	{
		if (IsEffekseer3DEffectPlaying(playing_handle) == -1 && is_loop)
			playing_handle = PlayEffekseer3DEffect(effect_handle);
	}
	else
	{
		if (IsEffekseer2DEffectPlaying(playing_handle) == -1 && is_loop)
			playing_handle = PlayEffekseer2DEffect(effect_handle);
	}
}

void Effect::Draw()
{
	if (playing_handle == INVALID_EFFECT)
		return;

	if (is_3d)
	{
		SetPosPlayingEffekseer3DEffect(playing_handle, pos.x, pos.y, pos.z);
		SetScalePlayingEffekseer3DEffect(playing_handle, scale.x, scale.y, scale.z);
		SetRotationPlayingEffekseer3DEffect(playing_handle, rot.x, rot.y, rot.z);
	}
	else
	{
		SetPosPlayingEffekseer2DEffect(playing_handle, pos.x, pos.y, pos.z);
		SetScalePlayingEffekseer2DEffect(playing_handle, scale.x, scale.y, scale.z);
		SetRotationPlayingEffekseer2DEffect(playing_handle, rot.x, rot.y, rot.z);
	}
}

void Effect::Stop()
{
	if (is_3d)
	{ 
		StopEffekseer3DEffect(playing_handle);
	}
	else
	{
		StopEffekseer2DEffect(playing_handle);
	}
}


namespace EffectSystem
{
	bool Init(int width, int height, int particle)
	{
		// Effekseerを使用するには必ず設定しなければいけない
		SetUseDirect3DVersion(DX_DIRECT3D_11);

		// Effekseerを初期化する。
		// 引数には画面に表示する最大パーティクル数を設定する。
		if (Effekseer_Init(particle) == -1)
			return false;

		// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
		// Effekseerを使用する場合は必ず設定する。
		SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

		// DXライブラリのデバイスロストした時のコールバックを設定する。
		// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
		// ただし、DirectX11を使用する場合は実行する必要はない。
		Effekseer_SetGraphicsDeviceLostCallbackFunctions();

		// 3DでZBufferを使用する
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);

		effect_width = width;
		effect_height = height;

		return true;
	}

	void Update()
	{
		UpdateEffekseer3D();
		UpdateEffekseer2D();
	}

	void Draw()
	{
		// Effekseerにより再生中のエフェクトを描画する。
		Effekseer_Sync3DSetting();
		DrawEffekseer3D();

		// Effekseerにより再生中のエフェクトを描画する。
		Effekseer_Set2DSetting(effect_width, effect_height);
		DrawEffekseer2D();
	}

	void Exit()
	{
		//綴りが違うんだが…
		Effkseer_End();
	}
}