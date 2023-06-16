#include "Goblin.h"
#include "ModelBase.h"
#include "../src/System/Random.h"
#include "../src/System/Vector3.h"
#include "../src/Game/Object/Camera.h"
#include "../src/System/AppendFunction.h"
#include "../src/Game/Object/Stage/MoveBlock/MoveBlock.h"
#include "Player.h"
#include "Npc.h"
#include "DxLib.h"
#include <vector>
#include <array>

#define BACK               50.0f
#define SET_Y_ADJEST_VALUE 15.0f


//コンストラクタ
//-----------------------------------------------
Goblin::Goblin(size_t num)
    : move_count(0)
    , is_move(false)
{
    //個体番号確認
    goblin_num = int(num);

    //ゴブリンモデル実装
    model_handle = MV1LoadModel("Data/Character/Goblin/Goblin.mv1");
    attachindex  = MV1AttachAnim(model_handle, 0, anim_handle[anim_no], FALSE);
    totaltime    = MV1GetAttachAnimTotalTime(model_handle, attachindex);

    //ゴブリンの初期状態
    goblin_action = STAY;

    MV1SetScale(model_handle, VGet(2.5f, 2.5f, 2.5f));   //モデルの大きさを変えます

    constexpr VECTOR GOBLIN_POS_LIST[] = {
#include "GoblinPos.inc"
    };
    pos = GOBLIN_POS_LIST[goblin_num];

    //ゴブリンの角度設定
    int StartRot = GetRand(4);
    rot          = {0.0f, TO_RADIAN(StartRot * (90.0f)), 0.0f};

    //射撃関係設定
    SetIsShoot(false);
    bullet_pos    = {(0.0f, 0.0f, 0.0f)};
    bullet_radius = 100.f;
    target_rot    = 0.0f;
    is_move       = false;
    roty          = 0.0f;
    count         = 0;

    //当たり判定用
    SetRadius(200.0f);
    is_hurt = false;
    Hp      = 500.f;
    Set_is_dead(false);

    // 重力ラインの初期設定
    SetGravityLineStart(pos, 150.0f);
    SetGravityLineEnd(pos, 50.0f);
    // 頭上ラインの初期設定
    SetTopLineStart(pos, 250.0f);
    SetTopLineEnd(pos, 350.0f);

    root_node = MV1SearchFrame(model_handle, "root");
    CreateIdentityMatrix(&identity);
}
Goblin::~Goblin()
{
    Exit();
}
//アニメーション演出
//---------------
void Goblin::PlayAnimation()
{
    playtime += 0.5f;   //アニメーションを進める

    if(totaltime < playtime) {
        playtime = 0;
        MV1DetachAnim(model_handle, attachindex);
        attachindex = MV1AttachAnim(model_handle, 0, anim_handle[anim_no], FALSE);
        totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    }
    MV1SetAttachAnimTime(model_handle, attachindex, playtime);   //再生時間をセットする
}
bool Goblin::MovieORDead()
{
    if((camera_ptr->GetOpenAnimeCount() > 0) || Check_Is_Dead())
        return true;
    return false;
}
void Goblin::CalculateAngle_zonbi_to_target(const VECTOR& pos_1,
                                            const VECTOR& pos_2,
                                            const VECTOR& vector1,
                                            const VECTOR& vector2)
{
    roty *= -1;
    if(pos_1.x < pos_2.x)
        roty = -(roty);
    rot.y = roty;
    //計算した角度はプラスとマイナスがない
    roty = CalculateAngle(vector1, vector2);
}
//更新処理
//---------------
void Goblin::Update()
{
    const VECTOR& player_rot = player_ptr->GetRot();
    const VECTOR& player_pos = player_ptr->GetPos();
    VECTOR        Standerd   = {0.0f, 0.0f, -1.0f};   //基準用ゴブリンのモデル角度
    VECTOR Goblin_to_player = {0.0f, 0.0f, 0.0f};   //ゴブリンがプレイヤーに向けている方向の初期値

    Goblin_to_player.x = (player_pos.x - pos.x);
    Goblin_to_player.z = (player_pos.z - pos.z);

    //演出中あるいは死んだとき
    if(MovieORDead()) {
        if(camera_ptr->GetOpenAnimeCount() > 0) {
            goblin_action = STAY;
        }
        if(Check_Is_Dead()) {
            goblin_action = DEAD;
        }
    }
    if(!MovieORDead()) {
        if(hurt_count > 0) {
            goblin_action = HURT;
        }
        else {
            //透明度を戻します
            transparency  = 1.0;
            goblin_action = STAY;
            if(GetDistance(pos, player_pos) < 15000.0f)
                goblin_action = SHOOT;
        }
    }

    hurt_count--;
    switch(goblin_action) {
    case Goblin::STAY:
        SetAnimation(anim_handle, 1);
        Fixpos(pos);
        break;
    case Goblin::MOVE:
        //アニメーション設定
        SetAnimation(anim_handle, 2);
        //モデル方向
        //それを位置に合わせて調整します
        roty *= -1;
        if(player_pos.x < pos.x)
            roty = -(roty);
        rot.y = roty;
        rot.y = CalculateAngle(Standerd, Goblin_to_player);
        //指定された方向で進む
        pos.x += (float)((-1) * GOBLIN_DUSH_SPEED * sinf(rot.y));
        pos.z += (float)((-1) * GOBLIN_DUSH_SPEED * cosf(rot.y));
        break;
    case Goblin::SHOOT:
        SetAnimation(anim_handle, 0);
        transparency = 2.0f;
        if(count == 0) {
            //弾丸の角度を再設定
            SetBulletPos(GetPos());
            //弾丸座標設定
            bullet_rot = rot.y;
        }
        Goblin_to_player.x = (player_pos.x - pos.x);
        Goblin_to_player.z = (player_pos.z - pos.z);
        //それを位置に合わせて調整します
        CalculateAngle_zonbi_to_target(player_pos, pos, Standerd, Goblin_to_player);
        count++;
        //3秒を越えならリロード
        if(count > 180) {
            Reload();
        }
        break;
    case Goblin::HURT:
        //アニメーション設定
        SetAnimation(anim_handle, 4);
        Hp -= 3.f;
        if((hurt_count < 0) && (Hp > 0.0f)) {
            Reset_tostay();
        }
        if((hurt_count % 5) == 0) {
            transparency = 0.5f;
        }
        else {
            transparency = 1.0f;
        }
        //移動量を沿って動きます
        pos.x -= (float)(BACK * sin(hurt_rot));
        pos.z -= (float)(BACK * cos(hurt_rot));
        hurt_count--;
        break;
    case Goblin::DEAD:
        SetAnimation(anim_handle, 5);
        transparency -= 0.01f;
        if(transparency < 0) {
            is_dead      = true;
            transparency = 0.0f;
            pos          = {28.0f * 200.0f, -14000.0f, 0.0f * 200.0f};
        }
        break;
    }

    Gravity();
    //bulletの角度に応じて前進する
    bullet_pos.x += (float)((-1) * (BULLET_SPEED * sin(bullet_rot)));
    bullet_pos.z += (float)((-1) * (BULLET_SPEED * cos(bullet_rot)));
    //アニメーションの処理をする
    PlayAnimation();
    GravityLine();
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
//重力設定
//---------------
void Goblin::Gravity()
{
    //床の或いは動く床の上に居る時重力稼働しない
    if(is_stepped || on_move_block) {
        gravity = 0;
    }
    else {
        gravity = (-5) * ADD_GRAVITY;
    }

    //プレイヤーY座標＝　(調整量)　+（重力補正）
    pos.y += (gravity);
}
//描画処理
//---------------
void Goblin::Render()
{

    //弾丸
    DrawSphere3D(VGet(bullet_pos.x, bullet_pos.y + 200.0f, bullet_pos.z),
                 bullet_radius,
                 32,
                 GetColor(0, 255, 0),
                 GetColor(0, 0, 0),
                 TRUE);

    //平常状態の描画設定
    MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
    MV1SetPosition(model_handle, VGet(pos.x, pos.y + SET_Y_ADJEST_VALUE, pos.z));
    MV1DrawModel(model_handle);
}
bool Goblin::Check_Is_Dead()
{
    if(Hp < 0.0f || pos.y < 0)
        return true;
    return false;
}
void Goblin::SetDiedPos()
{
    pos = {0.0f, -500.0f, 0.0f};
}
//固定位置用の関数
//--------------------
void Goblin::Fixpos(VECTOR& pos)
{
    this->pos = {pos.x, pos.y, pos.z};
}
//終了処理
//---------------
void Goblin::Exit()
{
    MV1DeleteModel(model_handle);
}
//弾丸ポスの設定
//---------------
void Goblin::SetBulletPos(const VECTOR& now_pos)
{
    bullet_pos = now_pos;
}
//弾丸射出したのがの設定
//---------------
void Goblin::SetIsShoot(bool now_is_shoot)
{
    is_shoot = now_is_shoot;
}
//弾丸リロードの設定
//---------------
void Goblin::Reload()
{
    SetBulletPos(VGet(0.0f, -20000.0f, 0.0f));
    SetIsShoot(false);   //処理を強制にフォルスさせる
    count = 0;           //countを０にする
}
//当たり判定用の重力ラインの設定
//---------------
void Goblin::GravityLine()
{
    // 重力ラインの初期設定
    SetGravityLineStart(pos, 150.0f);
    SetGravityLineEnd(pos, 50.0f);
    // 頭上ラインの初期設定
    SetTopLineStart(pos, 250.0f);
    SetTopLineEnd(pos, 350.0f);
}
//プレイヤーとブロックの確認用の関数
//--------------------------------------------------
//判定が被らないように　先に上と下の判定を消します
//--------------------------------------------------
void Goblin::ResetBeforeCheck()
{
    on_move_block = false;
    is_stepped    = false;
}
void Goblin::SetDownWhenHit(float block_hight)
{
    SetIsStepped(true);
    SetPosY(block_hight);
}
bool Goblin::HitWithStageBlock(const BlockBase* block)
{
    //判定する前に先に消す
    ResetBeforeCheck();
    //周りのブロックの当たり判定を取る
    CheckIfHitWithBlock(block->GetBlock1TopA(),
                        block->GetBlock1TopB(),
                        block->GetBlock1TopC(),
                        block->GetBlock1TopD(),
                        block->GetBlock1BottomA(),
                        block->GetBlock1BottomB(),
                        block->GetBlock1BottomC(),
                        block->GetBlock1BottomD());

    //当たってない時処理を飛ばして
    if(!CheckHitStageFloor())
        return false;
    // 当たった時
    is_stepped = true;
    SetDownWhenHit(block->GetBlockHight());
    return true;
}
bool Goblin::HitWithMoveBlock(const MoveBlock* block)
{
    //判定する前に先に消す
    ResetBeforeCheck();
    //周りのブロックの当たり判定を取る
    CheckIfHitWithBlock(block->GetBlock1TopA(),
                        block->GetBlock1TopB(),
                        block->GetBlock1TopC(),
                        block->GetBlock1TopD(),
                        block->GetBlock1BottomA(),
                        block->GetBlock1BottomB(),
                        block->GetBlock1BottomC(),
                        block->GetBlock1BottomD());

    //当たってない時処理を飛ばして
    if(!CheckHitStageFloor())
        return false;
    // 当たった時
    is_stepped    = true;
    on_move_block = true;
    SetDownWhenHit(block->GetBlockHight());
    //ブロックのヴェクターを渡します
    SetPosOnMoveBlock(block->GetMoveVector());
    return on_move_block;
}
//踏んでいるのかとぶつかっているのかの関数
//---------------------------------
void Goblin::SetIsStepped(bool now_is_stepped)
{
    is_stepped = now_is_stepped;
}
//Ｙ座標の設定
//---------------------------------
void Goblin::SetPosY(float fix_y)
{
    pos.y = fix_y;
}
//ムーヴブロックと当たった時の設定
//---------------------------------
void Goblin::SetPosOnMoveBlock(const Vector3& dir)
{
    pos.x += dir.x;
    pos.y += dir.y + 15.f;
    pos.z += dir.z;
}
//透明度の設定
//---------------------------------
void Goblin::Set_Transparency(float now_transparency)
{
    transparency = now_transparency;
}
//死亡フラグの設定
//---------------------------------
void Goblin::Set_is_dead(bool now_is_dead)
{
    is_dead = now_is_dead;
}
//ダメージの設定
//---------------------------------
void Goblin::Set_is_hurt(bool now_is_hurt)
{
    is_hurt = now_is_hurt;
}
//ＨＰの設定
//---------------------------------
void Goblin::Set_Hp(bool now_HP)
{
    Hp = now_HP;
}
//プレーヤーの弾丸に打たれた時の設定
//---------------------------------
void Goblin::SetPos_HurtByPlayerBullet(float bullet_rot)
{
    hurt_count    = 30;
    is_hurt       = true;
    goblin_action = HURT;
    hurt_rot      = bullet_rot;
}
//待機状態に戻る
//---------------------------------
void Goblin::Reset_tostay()
{
    is_hurt       = false;
    goblin_action = STAY;
}