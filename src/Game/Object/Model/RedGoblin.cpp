#include "RedGoblin.h"
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

#define BACK               30.0f
#define SET_Y_ADJEST_VALUE 20.0f


//コンストラクタ
//-----------------------------------------------
RedGoblin::RedGoblin(size_t num)
    : move_count(0)
    , is_move(false)
    , is_live(true)
{
    redgoblin_num = int(num);

    //ゴブリンモデル実装
    model_handle = MV1LoadModel("Data/Character/RedGoblin/RedGoblin.mv1");
    attachindex  = MV1AttachAnim(model_handle, 0, anim_handle[anim_no], FALSE);
    totaltime    = MV1GetAttachAnimTotalTime(model_handle, attachindex);

    //ゴブリンの初期状態

    goblin_action = MOVE;

    MV1SetScale(model_handle, VGet(2.5f, 2.5f, 2.5f));   //モデルの大きさを変えます

    constexpr VECTOR RED_GOBLIN_POS_LIST[] = {
#include "RedGoblinPos.inc"
    };
    pos = RED_GOBLIN_POS_LIST[redgoblin_num];

    int StartRot = GetRand(4);
    rot          = {0.0f, TO_RADIAN(StartRot * (90.0f)), 0.0f};

    //ブロックの上に立ったゴブリン以外はプレイヤーに自動攻撃出来るように
    goblin_action = STAY;
    roty          = 0.0f;
    count         = -180;

    //当たり判定用
    SetRadius(200.0f);
    is_hurt    = false;
    Hp         = 400.f;
    is_live    = true;
    hurt_count = -99;

    haved_hurt_by_player = false;
    in_sight             = false;   //視界の範囲内

    // 重力ラインの初期設定
    SetGravityLineStart(pos, 150.0f);
    SetGravityLineEnd(pos, 50.0f);
    // 頭上ラインの初期設定
    SetTopLineStart(pos, 250.0f);
    SetTopLineEnd(pos, 350.0f);

    root_node = MV1SearchFrame(model_handle, "root");
    CreateIdentityMatrix(&identity);
}
RedGoblin::~RedGoblin()
{
    Exit();
}
void RedGoblin::PlayAnimation()
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
bool RedGoblin::MovieORDead()
{
    if((camera_ptr->GetOpenAnimeCount() > 0) || Check_Is_Dead())
        return true;
    return false;
}
void RedGoblin::CalculateAngle_red_goblin_to_player(const VECTOR& pos_1,
                                                    const VECTOR& pos_2,
                                                    const VECTOR& vector1,
                                                    const VECTOR& vector2)
{
    //計算した角度はプラスとマイナスがない
    roty = CalculateAngle(vector1, vector2);
    roty *= -1;
    if(pos_1.x < pos_2.x)
        roty = -(roty);
    rot.y = roty;
}
//更新処理
//---------------
void RedGoblin::Update()
{
    const VECTOR& player_rot = player_ptr->GetRot();
    const VECTOR& player_pos = player_ptr->GetPos();
    VECTOR        Standerd   = {0.0f, 0.0f, -1.0f};   //基準用ゴブリンの角度
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
    else {
        //ダメージに入った時
        if(hurt_count > 0) {
            goblin_action = HURT;
        }
        else {
            //攻撃範囲に越えたら
            if(GetDistance(pos, player_pos) > 800.0f) {
                goblin_action = STAY;
            }
            //攻撃範囲の入ったら
            if(GetDistance(pos, player_pos) < 800.0f || haved_hurt_by_player || in_sight) {
                count++;
                in_sight = true;   //1度でも視界の中に入ったらずっと追跡
                if(count < 0) {
                    is_attacking  = true;
                    goblin_action = ATTACK;
                }
                if(count > 0) {
                    is_attacking  = false;
                    goblin_action = STAY;
                }
            }
        }
    }

    hurt_count--;
    switch(goblin_action) {
    case RedGoblin::STAY:
        transparency = 2.0f;
        SetAnimation(anim_handle, 1);
        Fixpos(pos);
        if(count > 30) {
            Reload();
        }
        break;
    case RedGoblin::MOVE:
        //アニメーション設定
        transparency = 2.0f;
        SetAnimation(anim_handle, 2);
        //計算した角度はプラスとマイナスがない
        CalculateAngle_red_goblin_to_player(player_pos, pos, Standerd, Goblin_to_player);
        //指定された方向で進む
        pos.x += (float)((-1) * GOBLIN_DUSH_SPEED * sinf(rot.y));
        pos.z += (float)((-1) * GOBLIN_DUSH_SPEED * cosf(rot.y));
        break;
    case RedGoblin::ATTACK:
        SetAnimation(anim_handle, 0);
        transparency = 2.0f;
        if(GetDistance(pos, player_pos) < 250.0f) {
            //計算した角度はプラスとマイナスがない
            CalculateAngle_red_goblin_to_player(player_pos, pos, Standerd, Goblin_to_player);
            //プレイヤーが貰ったダメージ用　暫定
            attack_rot = roty;
        }
        else {
            SetAnimation(anim_handle, 2);
            CalculateAngle_red_goblin_to_player(player_pos, pos, Standerd, Goblin_to_player);
            pos.x += (float)((-1) * 10 * sinf(rot.y));
            pos.z += (float)((-1) * 10 * cosf(rot.y));
        }

        if(count > 30) {
            Reload();
        }
        break;
    case RedGoblin::HURT:
        //アニメーション設定
        haved_hurt_by_player = true;    //１回でも当たったら
        is_attacking         = false;   //攻撃時しない
        SetAnimation(anim_handle, 4);
        Hp -= 2.5f;
        if((hurt_count < 0) && (Hp > 0.0f)) {
            Reset_tomove();
        }
        if((hurt_count % 5) == 0) {
            transparency = 0.5f;
        }
        else {
            transparency = 1.0f;
        }
        rot.y = hurt_rot + TO_RADIAN(180.f);
        //移動量を沿って動きます
        pos.x -= (float)(BACK * sin(hurt_rot));
        pos.z -= (float)(BACK * cos(hurt_rot));
        hurt_count--;
        break;
    case RedGoblin::DEAD:
        SetAnimation(anim_handle, 5);
        transparency -= 0.01f;
        if(transparency < 0) {
            is_live      = false;
            transparency = 0.0f;
            pos          = {28.0f * 200.0f, -15000.0f, 0.0f * 200.0f};
        }
        break;
    }

    //手の座標を更新
    framePos_R = MV1GetFramePosition(model_handle, 19);
    Gravity();
    //アニメーションの処理をする
    PlayAnimation();
    GravityLine();
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
void RedGoblin::Gravity()
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
void RedGoblin::Render()
{

    MV1SetOpacityRate(model_handle, transparency);
    //平常状態の描画設定
    MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
    MV1SetPosition(model_handle, pos);
    MV1DrawModel(model_handle);
}
bool RedGoblin::Check_Is_Dead()
{
    if(Hp < 0.0f || pos.y < 0)
        return true;
    return false;
}
//固定位置用の関数
//--------------------
void RedGoblin::Fixpos(VECTOR& pos)
{
    this->pos = {pos.x, pos.y, pos.z};
}
//終了処理
//---------------
void RedGoblin::Exit()
{
    MV1DeleteModel(model_handle);
}
void RedGoblin::SetIsLive(bool now_live)
{
    is_live = now_live;
}
bool RedGoblin::GetIsLive() const
{
    return is_live;
}
void RedGoblin::Reload()
{
    count = -180;   //countを-180にする
}
void RedGoblin::GravityLine()
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
void RedGoblin::ResetBeforeCheck()
{
    on_move_block = false;
    is_stepped    = false;
}
void RedGoblin::SetDownWhenHit(float block_hight)
{
    SetIsStepped(true);
    SetPosY(block_hight);
}
bool RedGoblin::HitWithStageBlock(const BlockBase* block)
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
bool RedGoblin::HitWithMoveBlock(const MoveBlock* block)
{
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
void RedGoblin::SetIsStepped(bool now_is_stepped)
{
    is_stepped = now_is_stepped;
}
void RedGoblin::SetPosY(float fix_y)
{
    pos.y = fix_y + SET_Y_ADJEST_VALUE;
}
void RedGoblin::SetPosOnMoveBlock(const Vector3& dir)
{
    pos.x += dir.x;
    pos.y += dir.y + SET_Y_ADJEST_VALUE;
    pos.z += dir.z;
}
void RedGoblin::Set_Transparency(float now_transparency)
{
    transparency = now_transparency;
}
void RedGoblin::Set_is_dead(bool now_is_dead)
{
    is_dead = now_is_dead;
}
void RedGoblin::Set_Hp(bool now_HP)
{
    Hp = now_HP;
}
void RedGoblin::SetPos_HurtByPlayerBullet(float bullet_rot)
{
    hurt_count    = 60;
    goblin_action = HURT;
    hurt_rot      = bullet_rot;
}
void RedGoblin::Reset_tomove()
{
    goblin_action = MOVE;
}