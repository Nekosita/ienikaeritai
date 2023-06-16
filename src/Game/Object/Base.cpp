#include"Base.h"


//コンストラクタ
//-----------------------------------------------
Base::Base():
	pos{ (0.0f, 0.0f,0.0f) },
	rot{ (0.0f, 0.0f,0.0f) }
{
	
}
//デストラクタ
//----------------------------------------------
Base::~Base()
{

}
//プレイヤーの位置設定用の設定関数
//-----------------------------------------------
void Base::SetPos(const VECTOR& now_pos)
{
	pos = now_pos;
}
//プレイヤーの当たり半径用の設定関数
//-----------------------------------------------
void Base::SetRadius(float now_radius)
{
	radius = now_radius;
}
void Base::SetRot(const VECTOR& now_rot)
{
    rot = now_rot;
}
//角度計算関数
float Base::CalculateAngle(const VECTOR& V1, const VECTOR& V2)
{
    float ans;
    ans = ((V1.x * V2.x) + (V1.y * V2.y) + (V1.z * V2.z)) / ((sqrtf((V1.x * V1.x) + (V1.y * V1.y) + (V1.z * V1.z))) *
                                                             (sqrtf((V2.x * V2.x) + (V2.y * V2.y) + (V2.z * V2.z))));

    ans = acosf(ans);

    return ans;
}
void Base::Update()
{

}
void Base::Render()
{

}
void Base::Exit()
{
    
}

