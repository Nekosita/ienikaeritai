#pragma once
#include<DxLib.h>
#include<string>

class Effect final
{
public:
	VECTOR pos{};
	VECTOR rot{};
	VECTOR scale{ 1.0f,1.0f,1.0f };

	const static int INVALID_EFFECT = -1;

	Effect();
	~Effect();

	bool Load( const std::string& filename );

	bool Play2D( bool loop = false, float speed = 1.0f );
	bool Play3D( bool loop = false, float speed = 1.0f );

	void Update();

	void Draw();

	void Stop();
	
	// エフェクトが有効かどうか
	bool IsValid()
	{
		return effect_handle != INVALID_EFFECT;
	}

	// エフェクトが実行中か
	bool IsActive()
	{
		return playing_handle != INVALID_EFFECT;
	}

private:
	int	 effect_handle	= INVALID_EFFECT;
	int	 playing_handle = INVALID_EFFECT;
	bool is_3d			= false;
	bool is_loop		= false;
};


namespace EffectSystem
{
	constexpr int DEFAULT_PARTICLE_NUM = 8000;

	bool Init(int width, int height, int particle=DEFAULT_PARTICLE_NUM );
	void Update();
	void Draw();
	void Exit();
}