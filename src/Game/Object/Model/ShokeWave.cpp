#include "ShokeWave.h"
#include "../src/System/Vector3.h"
#include "Player.h"
#include "Npc.h"
#include "Boss.h"
#include "../src/System/AppendFunction.h"

//コンストラクタ
//----------------------------------
ShokeWave::ShokeWave()
{
    pos           = {0.0f, -100000.0f, 0.0f};                   //位置設定
    model_handle  = MV1LoadModel("Data/Stage/elc_rock.mqoz");   //モデル設定
    wave_action   = STAY;
    wave_radius   = 1000.0f;
    now_attacking = 0;
}
//デストラクタ
//---------------------------------
ShokeWave::~ShokeWave()
{
    Exit();
}
void ShokeWave::ShokeWaveReset()
{
    now_attacking = -10;   //攻撃用カウンターを消す
    wave_radius   = 100.0f;
    pos           = {0.0f, -100000.0f, 0.0f};   //位置設定
}
//更新処理
//------------------------------
void ShokeWave::Update()
{
    const VECTOR& player_pos = player_ptr->GetPos();
    const VECTOR& boss_pos   = boss_ptr->GetPos();


    if(boss_ptr->Get_on_wave()) {
        wave_action = ATTACK;
    }
    else {
        wave_action = STAY;
    }

    switch(wave_action) {
    case ShokeWave::STAY:
        ShokeWaveReset();
        break;
    case ShokeWave::ATTACK:
        //常にボスと同じpos
        pos.x = boss_pos.x;
        pos.y = boss_pos.y;
        pos.z = boss_pos.z;

        if((now_attacking > 0) && (now_attacking < 180)) {
            //プレイヤーを狙って
            wave_radius += 10.0f;
        }
        else {
            wave_radius += 25.0f;
        }
         
        //衝撃波出す
        now_attacking++;
        break;
    }

}
//トラップボールの描画
//--------------------------------------
void ShokeWave::Render()
{
    SetUseLighting(TRUE);
    MV1SetOpacityRate(model_handle, 0.25f);
    MV1SetScale(model_handle,
                VGet((float)(wave_radius / 100),
                     (float)(wave_radius / 100),
                     (float)(wave_radius / 100)));   //モデルの大きさを変えます
    MV1SetRotationXYZ(model_handle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(model_handle, pos);
    MV1DrawModel(model_handle);
    SetUseLighting(FALSE);
}
void ShokeWave::Exit()
{
    MV1DeleteModel(model_handle);
}