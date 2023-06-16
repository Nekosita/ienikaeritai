#include "TrapBall.h"
#include "../src/System/Vector3.h"
#include "Player.h"
#include "Npc.h"
#include "Boss.h"
#include "../src/System/AppendFunction.h"

//コンストラクタ
//----------------------------------
TrapBall::TrapBall(size_t ball_num)
{
    SetRadius(300.0f);   //半径設定

    ball_num = int(ball_num);

    pos           = {0.0f, -100000.0f, 0.0f};               //位置設定
    model_handle  = MV1LoadModel("Data/Stage/rock.mqoz");   //モデル設定
    ball_action   = STAY;
    speed         = {0.0f, 0.0f, 0.0f};
    now_attacking = 0;
}
//デストラクタ
//---------------------------------
TrapBall::~TrapBall()
{
    Exit();
}
bool TrapBall::CheckHitWithPlayer()
{
    return AttachCheck_BalltoChractor(GetPos(), GetRadius(), player_ptr->GetPos(), player_ptr->GetRadius(), 150.f);
}
void TrapBall::Trap_BallReset()
{
    ball_action = STAY;
    pos         = {0.0f, -100000.0f, 0.0f};   //位置設定
}
//更新処理
//------------------------------
void TrapBall::Update()
{
    const VECTOR& player_pos = player_ptr->GetPos();
    const VECTOR& boss_pos   = boss_ptr->GetPos();

    //もしもボスが死にましたら強制出来にSTAYに入ります
    if(boss_ptr->Get_is_dead()) {
        ball_action = STAY;
    }
    else {
        if(boss_ptr->Get_ball_summon() && boss_ptr->GetIsAttatched()) {
            ball_action = ATTACK;
        }
        else {
            ball_action = STAY;
        }
    }

    switch(ball_action) {
    case TrapBall::STAY:
        Trap_BallReset();
        break;
    case TrapBall::ATTACK:
        //位置を準備して
        //時間切れあるいはプレイヤーとNPCと当たった時
        if(now_attacking > 60 || CheckHitWithPlayer()) {
            now_attacking = 0;
        }
        if(now_attacking == 0) {
            //位置設置
            int x, y;
            x     = GetRand(5);
            y     = GetRand(5);
            pos.x = (boss_pos.x - 800.0f * 2) + 800.0f * x;
            pos.y = boss_pos.y + 800.f;
            pos.z = (boss_pos.z - 800.0f * 2) + 800.0f * y;
        }
        //ベクトル作成
        if((now_attacking > 1) && (now_attacking < 4)) {
            //プレイヤーを狙って
            speed.x = (player_pos.x - pos.x) / 60;
            speed.y = (player_pos.y - pos.y + 150.f) / 60;
            speed.z = (player_pos.z - pos.z) / 60;
        }
        //発射
        now_attacking++;
        break;
    }

    pos.x += speed.x;
    pos.y += speed.y;
    pos.z += speed.z;
}
//トラップボールの描画
//--------------------------------------
void TrapBall::Render()
{
    //ボールを描画します
    SetUseLighting(TRUE);
    //ボール表面の描画
    MV1SetScale(model_handle, VGet(3.0f, 3.0f, 3.0f));   //モデルの大きさを変えます
    MV1SetRotationXYZ(model_handle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(model_handle, pos);
    MV1DrawModel(model_handle);
    //ボールを描画します
    SetUseLighting(FALSE);
}
void TrapBall::Exit()
{
    MV1DeleteModel(model_handle);
}