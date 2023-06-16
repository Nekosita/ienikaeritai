#pragma once
#include "Player.h"
#include "../src/Game/Object/Stage/MoveBlock/MoveBlock.h"
#include "../src/Game/Object/GoalArea.h"
#include "../src/Game/Object/Camera.h"
#include "../src/Game/Object/Model/Boss.h"
#include "../src/Game/Object/Model/ShokeWave.h"
#include "../src/System/AppendFunction.h"
#include "../src/System/Vector3.h"
#include "../src/System/KeyInput.h"
#include <stdio.h>
#include "DxLib.h"

#define B_SPEED            200.f
#define STEP_BACK          45.0f
#define SET_Y_ADJEST_VALUE 15.0f

//プレイヤーが１体の時しか使えない
//-----------------------------------------------
namespace
{
Player* instance = nullptr;   // プレイヤー
}

bool Player::Check_MovieNowShowing()
{
    if((comera_ptr->GetOpenAnimeCount() > 0) || (comera_ptr->GetEndAnimeCount() < 90))
        return true;
    return false;
}

//プレイヤーを取得
//-----------------------------------------------
Player* Player::GetPlayer()
{
    return instance;
}
//コンストラクタ
//-----------------------------------------------
Player::Player()
    : now_fall(false)
{
    instance = this;
    // 普通のモデル設定
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制mouseフォルダセットします
    std::string strM = std::string(str) + "/Data/Character/Mousey/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("Idle.mv1");
    //Gameフォルダに戻します
    SetCurrentDirectory(str);

    // モデルの大きさ設定
    MV1SetScale(model_handle, VGet(2.5f, 2.5f, 2.5f));

    // プレイヤーの状態の初期設定
    // プレイヤー初期座標の設定
    pos = {230 * 200.0f, 5000.f, 0 * 200.0f};

    //プレイヤー初期rotを設定（向いている
    VECTOR start_rot{0.0f, -1.51f, 0.0f};
    SetRot(start_rot);

    is_jump     = false;                  //最初はジャンプしていない
    on_ground   = true;                   //地面にいない
    now_fall    = false;                  //落下していない
    mov_y       = 0.0f;                   //ジャンプ距離の初期化
    gravity     = 0.0f;                   //重力の初期化
    is_stepped  = false;                  //最初は踏めてない
    is_collided = false;                  //上にも立ってない
    SetRadius(150.0f);                    //当たり半径設定
    is_run   = false;                     //最初走ってない
    stand_by = rot.y + TO_RADIAN(0.0f);   //is_run = false; の時使ったロット

    rigor = 0;   //弾丸の硬直

    //待機用モード用
    player_is_boring = false;
    boring_count     = 120;

    knocked_off_variation = VGet(0.0f, 0.0f, 0.0f);   //初期化
    now_player            = COMMON;

    jump_count = 120;   //ジャンプカウンター
    SetToHintBar(false);

    // ゴールバー用の変数初期化
    is_goal_actived = false;

    count              = -99;
    openmovieupbar_y   = 200.0f;
    openmoviedownbar_y = 650.0f;

    npc_pause    = false;
    Hp_handle    = LoadGraph("Data/UI/player_hp.png");
    Hp_framework = LoadGraph("Data/UI/framework.png");
    Hp_handle_dying = LoadGraph("Data/UI/player_hp_dying.png");
    Hp = 100.0f;

    // 重力ラインの初期設定
    SetGravityLineStart(pos, 50.0f);
    SetGravityLineEnd(pos, 170.0f);

    // 頭上ラインの初期設定
    SetTopLineStart(pos, 150.0f);
    SetTopLineEnd(pos, 250.0f);

    // 普通のアニメーションの初期設定
    attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
    totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    root_node   = 1;
    CreateIdentityMatrix(&identity);
}
//デストラクタ
//---------------------------------
Player::~Player()
{
    Exit();
    instance = nullptr;
}
//アニメの設定
//---------------------------------
void Player::SetAnimation(const std::vector<int>& handle, int new_anim_no)
{
    MV1DetachAnim(model_handle, attachindex);
    anim_no     = new_anim_no;
    attachindex = MV1AttachAnim(model_handle, 1, handle[anim_no], FALSE);
    totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
}
//プレイヤーアニメーション演出
//---------------------------------------------------------
void Player::PlayAnimation()
{
    playtime += 0.5f;   //アニメーションを進める
    if(totaltime <= playtime) {
        playtime = 0.0f;
        if(anim_no == 0) {
            MV1DetachAnim(model_handle, attachindex);
            anim_no     = 1;
            attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
            totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
        }
    }
    MV1SetAttachAnimTime(model_handle, attachindex, playtime);   //再生時間をセットする
}
void Player::CheckPlayerIsBoring()
{
    //退屈すぎて踊り用のカウンターが始まる
    boring_count--;
    //3秒経過してまだ動いていないなら　
    if(boring_count < 0) {
        player_is_boring = true;
    }
    else {
        player_is_boring = false;
    }
}
void Player::ResetPlayerIsBoring()
{
    boring_count     = 300;     //カウンターもリセット
    player_is_boring = false;   //動いたら待機実行しない
}
//-----------------------------------------------------------------------
// Jumpと重力関係処理
// ---------------------------------------------------------------
void Player::JumpProcess(bool now_is_collided)
{
    //もしもプレイヤーが動いてないなら
    if((!is_run) && !(is_jump)) {
        CheckPlayerIsBoring();
    }

    //地面から飛べる時とブロックから飛ぶ時の判断
    IsJump();

    //飛んだら
    if(is_jump) {
        mov_y += JUMP_SPEED;
        OnResistance();
        //もしもブロックにぶつかったらリセット
        if(now_is_collided) {
            JumpReset();
            now_fall = true;
        }
    }
    //飛んでいた時と落ちた時重力起動
    GravityInJump();

    //調整量は常に重力追加
    mov_y -= ADD_GRAVITY;

    //プレイヤーY座標＝　(調整量)　+（重力補正）
    pos.y += (mov_y) + (gravity);

    //地面に居てそして飛んでいないなら
    if((pos.y < 0.0f) && !is_jump) {
        JumpResetGround();
    }
}
void Player::IsJump()
{
    //地面から飛んた場合（ジャンプしてないと落ちてない時のみジャンプ）
    if(PushHitKey(KEY_INPUT_SPACE) && (!is_jump) && (!now_fall)) {
        JumpOnTheGround();
    }

    //階段に踏んだ場合
    if(is_stepped) {
        is_run = false;
        //ジャンプの中初期化
        JumpReset();
        //ブロックから飛んた場合（ジャンプしてないと落ちでない時のみジャンプ）
        if(PushHitKey(KEY_INPUT_SPACE)) {
            JumpOnTheBlock();
        }
    }
}
void Player::JumpReset()
{
    //ジャンプの中初期化用変数
    mov_y    = 0.0f;
    gravity  = 0.0f;
    is_jump  = false;
    now_fall = false;
}
void Player::JumpResetGround()
{
    //ジャンプの中初期化用変数
    pos.y    = 0.0f;
    mov_y    = 0.0f;
    gravity  = 0.0f;
    is_jump  = false;
    now_fall = false;
}
void Player::JumpHit()
{
    //ジャンプの中初期化用変数
    if(mov_y > 0)
        mov_y = 0.0f;
    is_jump  = false;
    now_fall = true;
}
void Player::JumpOnTheBlock()
{
    player_is_boring = false;   //動いたら待機実行しない
    boring_count     = 300;     //カウンターもリセット
    is_run           = true;
    is_jump          = true;
}
void Player::JumpOnTheGround()
{
    player_is_boring = false;   //動いたら待機実行しない
    boring_count     = 300;     //カウンターもリセット
    is_run           = true;
    is_jump          = true;
    is_stepped       = false;
}
void Player::OnResistance()
{
    is_jump  = false;   //重力加速度は常にかかり続けるほうが良い
    now_fall = true;    //本来このフラグは必要ない mov_yの方向でわかる
}
void Player::GravityInJump()
{
    if(is_jump || now_fall) {
        gravity -= 0.5f;
    }
}
void Player::SetTopWhenHit(float block_hight)
{
    SetPosY(block_hight - 300.0f);
    SetIsCollided(true);
    JumpHit();
}
void Player::SetDownWhenHit(float block_hight)
{
    SetIsStepped(true);
    SetPosY(block_hight);
}
bool Player::IsJumpUp()
{
    return mov_y > 0;
}
void Player::SetPosOnMoveBlock(const Vector3& dir)
{
    pos.x += dir.x;
    pos.y += dir.y;
    pos.z += dir.z;
}
//踏んでいるのかとぶつかっているのかの関数
//---------------------------------
void Player::SetIsStepped(bool now_is_stepped)
{
    is_stepped = now_is_stepped;
}
//ブロックとぶつかったのがどうがを設定する用の関数
//---------------------------------
void Player::SetIsCollided(bool now_is_collided)
{
    is_collided = now_is_collided;
}

void Player::SetPosY(float fix_y)
{
    pos.y = fix_y;
}
//プレイヤーとブロックの確認用の関数
//--------------------------------------------------
//判定が被らないように　先に上と下の判定を消します
//--------------------------------------------------
void Player::ResetBeforeCheck()
{
    SetIsStepped(false);
    SetIsCollided(false);
}
void Player::SetToHintBar(bool now_hint)
{
    to_hint_bar = now_hint;
}
//プレイヤーとステージブロックの当たり用
//--------------------------------------------------
bool Player::HitWithStageBlock(const BlockBase* block)
{
    if(IsJumpUp()) {
        SetIsCollided(false);
        //上昇中の時
        //当たてないなら処理は戻る
        if(!block->CheckHitStageBottom()) {
            return false;
        }
        //当たてたら処理はこちに来ます
        SetTopWhenHit(block->GetBlockHight());
        return true;
    }
    else {
        //下から上何がぶつかったら処理に戻る
        if(GetIsCollided())
            return false;
        //当たてないなら処理は戻る
        if(!block->CheckHitStageFloor())
            return false;
        //当たてたら処理はこちに来ます
        SetDownWhenHit(block->GetBlockHight());
        return true;
    }
    return false;
}
bool Player::HitWithMoveBlock(const MoveBlock* block)
{
    //判定先に初期化します
    on_move_block = false;
    if(IsJumpUp()) {
        SetIsCollided(false);
        //上昇中の時
        if(!block->CheckHitStageBottom()) {
            return false;
        }
        // 当たった時
        on_move_block = true;
        SetTopWhenHit(block->GetBlockHight());
        return true;
    }
    else {
        if(GetIsCollided())
            return false;
        if(!block->CheckHitStageFloor())
            return false;
        // 当たった時
        on_move_block = true;
        SetDownWhenHit(block->GetBlockHight());
        //ブロックのヴェクターを渡します
        SetPosOnMoveBlock(block->GetMoveVector());
        return true;
    }
    return on_move_block;
}
void Player::GravityLine()
{
    // 重力ラインの設定
    SetGravityLineStart(pos, 50.0f);
    SetGravityLineEnd(pos, 180.0f);

    // 頭上ラインの設定
    SetTopLineStart(pos, 150.0f);
    SetTopLineEnd(pos, 250.0f);
}

//-----------------------------------------------------
//移動処理全般
//-----------------------------------------------------
void Player::Move()
{
    int ClickY, Button;
    if((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
        if((MOUSE_INPUT_RIGHT) != 0) {
            GetMouseInputLog(&Button, &click_mouse_x, &ClickY, TRUE);   //押した瞬間だけ取得
            GetMousePoint(&now_mouse_x, &now_mouse_y);                  //マウスのXはいつも更新される
            //判定に入る
            //押した時のマウスを記録して、押されていない時と比べる
            if(now_mouse_x < click_mouse_x) {
                rot.y += TO_RADIAN(PLAYER_ROT_SPEED);
            }
            if(now_mouse_x > click_mouse_x) {
                rot.y -= TO_RADIAN(PLAYER_ROT_SPEED);
            }
        }
    }

    if(CheckHitKey(KEY_INPUT_D)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(右
        SetRight();
    }
    if(CheckHitKey(KEY_INPUT_A)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(左
        SetLeft();
    }
    if(CheckHitKey(KEY_INPUT_W)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(前
        SetFront();
    }
    if(CheckHitKey(KEY_INPUT_S)) {
        SetBeforeRun();
        //モデルの方向をカメラの方向に向いています(後
        SetBack();
    }

    //if(MoveFlag == TRUE) {
    //    VECTOR TempMoveVector;

    //    // カメラの角度に合わせて移動ベクトルを回転してから加算
    //    SinParam         = sin(CameraHAngle / 180.0f * DX_PI_F);
    //    CosParam         = cos(CameraHAngle / 180.0f * DX_PI_F);
    //    TempMoveVector.x = MoveVector.x * CosParam - MoveVector.z * SinParam;
    //    TempMoveVector.y = 0.0f;
    //    TempMoveVector.z = MoveVector.x * SinParam + MoveVector.z * CosParam;

    //    Position = VAdd(Position, TempMoveVector);
    //}
}
void Player::SetBeforeRun()
{
    ResetPlayerIsBoring();
    is_run = true;   //走っています
    SetAnimation(anim_handle, 1);
}

#if 1
void Player::SetRight()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(90.0f);
    stand_by           = rot.y + TO_RADIAN(90.0f);   //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED)*sinf(rot.y + TO_RADIAN(270.0f));
    pos.z += (PLAYER_MOVE_SPEED)*cosf(rot.y + TO_RADIAN(270.0f));
}
void Player::SetLeft()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(270.0f);
    stand_by           = rot.y + TO_RADIAN(270.0f);   //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED)*sinf(rot.y + TO_RADIAN(90.0f));
    pos.z += (PLAYER_MOVE_SPEED)*cosf(rot.y + TO_RADIAN(90.0f));
}
void Player::SetFront()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(0.0f);
    stand_by           = rot.y + TO_RADIAN(0.0f);   //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED)*sinf(rot.y + TO_RADIAN(180.0f));
    pos.z += (PLAYER_MOVE_SPEED)*cosf(rot.y + TO_RADIAN(180.0f));
}
void Player::SetBack()
{
    //モデルの方向をカメラの方向に向いています
    player_model_rot.y = rot.y + TO_RADIAN(180.0f);
    stand_by           = rot.y + TO_RADIAN(180.0f);   //is_run = false; の時使ったロット
                                                      //is_run = false; の時使ったロット
    pos.x += (PLAYER_MOVE_SPEED)*sinf(rot.y);
    pos.z += (PLAYER_MOVE_SPEED)*cosf(rot.y);
}
#endif   // 0

void Player::ResetPlayerPosition()
{
    if(player_is_boring) {
        player_model_rot.y = stand_by;   //角度は合わせて調整
        SetAnimation(anim_handle, 4);
    }
    else {
        player_model_rot.y = stand_by;   //角度は合わせて調整
        SetAnimation(anim_handle, 0);
    }
}
//----------------------------------------------------
//攻撃弾丸用
//-----------------------------------------------------
void Player::Reload()
{
    SetBulletPos(bullet_dead_pos);
    SetIsShoot(false);   //処理終わってたらフォルスになる
    count = 0;           //countを０にする
    rigor = 0;
}
void Player::ShootProcess()
{
    if((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        ResetPlayerIsBoring();
        SetAnimation(anim_handle, 8);
        if(is_run)
            SetAnimation(anim_handle, 9);
        if(!GetIsShoot()) {
            SetIsShoot(true);
            SetBulletPos(GetPos());
            bullet_rot = player_model_rot.y;
        }
    }
}
void Player::SetBulletPos(const VECTOR& now_pos)
{
    bullet_pos = now_pos;
}

void Player::SetIsShoot(bool now_is_shoot)
{
    is_shoot = now_is_shoot;
}
float Player::Heal()
{
    Hp += 30.0f;
    //HPの最大値決め
    if(Hp > 100.0f) {
        Hp = 100.0f;
    }
    return Hp;
}
bool Player::Check_Is_hurt()
{
    if((hurt_count > 0) || wave_count > 0 || (ball_count > 0) || (part_count > 0)) {
        return true;
    }
    return false;
}

void Player::GetHurt()
{
    SetAnimation(anim_handle, 2);
    Hp -= 0.2f;
    if((hurt_count % 5) == 0) {
        transparency = 0.5f;
    }
    else {
        transparency = 1.0f;
    }
    //移動量を沿って動きます
    pos.x -= (float)(STEP_BACK * sin(hurt_rot));
    pos.z -= (float)(STEP_BACK * cos(hurt_rot));
    hurt_count--;
}
void Player::ResetCount(float attack_rot)
{
    hurt_rot   = attack_rot;
    hurt_count = 40;
}
bool Player::CheckWave(float addjust_y)
{
    if(boss_ptr->Get_on_wave() && AttachCheck(shoke_wave_ptr->GetPos(),
                                              shoke_wave_ptr->Get_wave(),
                                              VGet(pos.x, pos.y + addjust_y, pos.z),
                                              GetRadius()))
        return true;
    return false;
}
bool Player::CheckHitWithBossHands(float addjust_y)
{
    //攻撃中その上両手に殴られた時
    if(boss_ptr->Getis_jump_attack() && (AttachCheck(boss_ptr->GetframePosLPos(),
                                                     boss_ptr->Get_part_radius(),
                                                     VGet(pos.x, pos.y + addjust_y, pos.z),
                                                     GetRadius()) ||
                                         AttachCheck(boss_ptr->GetframePosRPos(),
                                                     boss_ptr->Get_part_radius(),
                                                     VGet(pos.x, pos.y + addjust_y, pos.z),
                                                     GetRadius()))) {
        return true;
    }
    return false;
}
bool Player::CheckHitWithBossRightHands(float addjust_y)
{
    //攻撃中その上両手に殴られた時
    if(boss_ptr->Getis_standing_melee_attack_downward() && (AttachCheck(boss_ptr->GetframePosRPos(),
                                                                        boss_ptr->Get_part_radius(),
                                                                        VGet(pos.x, pos.y + addjust_y, pos.z),
                                                                        GetRadius()))) {
        return true;
    }
    return false;
}
void Player::GetHurt_wave()
{
    SetAnimation(anim_handle, 2);
    Hp -= 0.2f;
    if((wave_count % 5) == 0) {
        transparency = 0.5f;
    }
    else {
        transparency = 1.0f;
    }

    //位置変更
    Vector3 dir;
    float   new_radius;
    dir        = {VGet(GetPos().x, GetPos().y + 150.0f, GetPos().z) - shoke_wave_ptr->GetPos()};
    new_radius = shoke_wave_ptr->Get_wave() + GetRadius();
    dir.SetLength(new_radius);
    SetPosWithWave(shoke_wave_ptr->GetPos(), dir);

    wave_count--;
}
void Player::SetPosWithWave(const VECTOR& now_pos, const Vector3& dir)
{
    pos.x = now_pos.x + dir.x * (1.3f);
    pos.y = now_pos.y + dir.y * (1.2f);
    pos.z = now_pos.z + dir.z * (1.3f);
}
void Player::GetHurt_BossPart()
{
    //重傷モデル
    SetAnimation(anim_handle, 7);
    Hp -= 0.2f;
    if((part_count % 5) == 0) {
        transparency = 0.5f;
    }
    else {
        transparency = 1.0f;
    }
    part_count--;
}
void Player::GetHurt_ball()
{
    SetAnimation(anim_handle, 2);
    Hp -= 0.15f;
    if((ball_count % 5) == 0) {
        transparency = 0.5f;
    }
    else {
        transparency = 1.0f;
    }
    ball_count--;
}
void Player::SetHurtByBall()
{
    ball_count = 30;
}
//-------------------------------------------
// その他設定
//-------------------------------------------
void Player::PlayerReset()
{
    Hp                 = 100.0f;
    pos                = VGet(-10 * 200.0f, 1750.f, 7.5 * 200.0f);
    player_model_rot.y = TO_RADIAN(180.0f);
    //プレイヤー初期rotを設定（向いている
    VECTOR start_rot{0.0f, -1.51f, 0.0f};
    SetRot(start_rot);
    wave_count           = -99;
    hurt_count           = -99;
    ball_count           = -99;
    part_count           = -99;
    is_dead              = false;
    count_after_bossdead = 150;
}
bool Player::SetNpcPause()
{
    //Qキーを押して止まるがを確認
    if(IsKeyOn(KEY_INPUT_Q))
        return npc_pause ? false : true;
    //もしもボタンが押されていない場合は、現在のset_buttonの値が返る
    return npc_pause;
}
//プレイヤーのアップデート
//-----------------------------------
void Player::Update()
{
    //もしもBOSSの衝撃波に打たれたらwaveカウントが始まります
    if(CheckWave(125.f)) {
        wave_count = 30;
    }
    //BOSSの両手攻撃
    if(CheckHitWithBossHands(125.f)) {
        part_count = 30;
    }
    //BOSSの片手攻撃
    if(CheckHitWithBossRightHands(125.f)) {
        part_count = 10;
    }

    //上映中をチェック
    if(Check_MovieNowShowing()) {
        if(comera_ptr->GetOpenAnimeCount() > 0) {
            now_player = START_MOVIE;
        }
        if(comera_ptr->GetEndAnimeCount() < 90) {
            now_player = GAME_CLEAR_MOVIE;
        }
    }
    if(!Check_MovieNowShowing()) {
        //何も無い時
        now_player = COMMON;
        //怪我をしたら
        if(Check_Is_hurt())
            now_player = HURT;
    }

    //ボスの手の攻撃が食らった時演出に合わせてプレーヤーの角度を変えます
    if(part_count > 0) {
        player_model_rot.x = TO_RADIAN(270.0f);
    }
    else {
        player_model_rot.x = 0.0;
    }

    //プレイヤーの死亡確認
    if((GetPos().y < 50.0f) || (GetHp() < 25.0f)) {
        is_dead = true;
    }

    part_count--;
    wave_count--;
    hurt_count--;
    ball_count--;
    zonbi_count--;
    switch(now_player) {
    case PLAYER_STATUS::START_MOVIE:
        //プレイヤー設定
        transparency = 2.0f;
        SetAnimation(anim_handle, 6);
        pos                = VGet(-20 * 200.0f, 1750.f, 7.5 * 200.0f);
        player_model_rot.y = TO_RADIAN(180.0f);
        /*       pos                = VGet(195 * 200.0f, 1750.f, 12.5 * 200.0f);
        player_model_rot.y = TO_RADIAN(180.0f);*/
 /*       pos                = VGet(120 * 200.0f, 1750.f, -7.5 * 200.0f);
        player_model_rot.y = TO_RADIAN(180.0f);*/
        //pos                = VGet(220 * 200.0f, 4150.f, 7.5 * 200.0f);
        //player_model_rot.y = TO_RADIAN(180.0f);
        if(comera_ptr->GetOpenAnimeCount() < 350) {
            openmovieupbar_y -= 0.2f;
            openmoviedownbar_y += 0.2f;
        }
        break;
    case PLAYER_STATUS::HURT:
        //弾丸に打たれた時
        if(hurt_count > 0) {
            GetHurt();
        }
        //衝撃波に打たれた時
        if(wave_count > 0) {
            GetHurt_wave();
        }
        //Bossのボールに打たれた時
        if(ball_count > 0) {
            GetHurt_ball();
        }
        if(part_count > 0) {
            GetHurt_BossPart();
        }
        break;
    case PLAYER_STATUS::COMMON:
        transparency = 2.0f;
        //NPCとの処理
        npc_pause = SetNpcPause();
        //移動処理
        Move();
        //射撃の処理
        ShootProcess();
        //走ってない、打って無い、カメラ動いて無い時のみ待機確認に入ります
        if(!is_run && ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0) && ((GetMouseInput() & MOUSE_INPUT_RIGHT) == 0)) {
            ResetPlayerPosition();
        }

        JumpProcess(GetIsCollided());
        break;
    case PLAYER_STATUS::SHOOT:
        break;
    case PLAYER_STATUS::GAME_CLEAR_MOVIE:
        SetAnimation(anim_handle, 1);
        player_model_rot.y = TO_RADIAN(270.0f);
        if(comera_ptr->GetEndAnimeCount() > 88) {
            pos = VGet(280.0f * 200.f, 4150.f, 7.5f * 200.f);
        }
        //ここのポス変えて
        pos.y = 4150.f;
        pos.x += (float)((310.0 * 200.0f - pos.x) / 250);
        pos.z += (float)((10.0f * 200.0f - pos.z) / 250);
        break;
    }

    //！弾丸の硬直
    if(rigor > 20) {
        Reload();
    }
    if(GetIsShoot()) {
        rigor++;
    }

    //ボスが死んた時
    if(boss_ptr->Get_is_dead()) {
        count_after_bossdead--;
    }

    //！bulletの角度に応じて前進する
    bullet_pos.x += (float)((-1) * (B_SPEED * sin(bullet_rot)));
    bullet_pos.z += (float)((-1) * (B_SPEED * cos(bullet_rot)));

    //重力処理
    GravityLine();
    //動画の処理
    //アニメーションの処理をする
    PlayAnimation();
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
//描画処理用関数
//----------------------------------------------
void Player::Render()
{
    //2D表示計算
    auto screen_position = ConvWorldPosToScreenPos(pos);
    //位置の上の側に表示
    int x = static_cast<int>(screen_position.x);         //X座標設定
    int y = static_cast<int>(screen_position.y) - 100;   //Y座標設定
    SetUseLighting(FALSE);                               //ライティングOFF
    switch(now_player) {
    case PLAYER_STATUS::START_MOVIE:
        SetFontSize(60);
        if((comera_ptr->GetOpenAnimeCount() > 30) && (comera_ptr->GetOpenAnimeCount() < 90)) {
            DrawString(x, y, "了解！", GetColor(255, 137, 49));
        }
        if(comera_ptr->GetOpenAnimeCount() < 240) {
            DrawBoxAA(0, 0, SCREEN_W, openmovieupbar_y, GetColor(0, 0, 0), TRUE);
            DrawBoxAA(0, openmoviedownbar_y, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
        }
        SetFontSize(40);
        break;
    case PLAYER_STATUS::HURT:
        if(Hp < 50.0) {
            DrawCircleGauge(SCREEN_W - 210, SCREEN_H - 100, Hp, Hp_handle_dying, 25.0f, 0.75f);
        }else{
            DrawCircleGauge(SCREEN_W - 210, SCREEN_H - 100, Hp, Hp_handle, 25.0f, 0.75f);
        }
        DrawCircleGauge(SCREEN_W - 210, SCREEN_H - 100, 100.0f, Hp_framework, 25.0f, 0.75f);
        break;
    case PLAYER_STATUS::COMMON:
        //もしもHPが50.f~25.f以下なら赤くなります
        if(Hp<50.0) {
            DrawCircleGauge(SCREEN_W - 210, SCREEN_H - 100, Hp, Hp_handle_dying, 25.0f, 0.75f);
        }else{
            DrawCircleGauge(SCREEN_W - 210, SCREEN_H - 100, Hp, Hp_handle, 25.0f, 0.75f);
        }
        DrawCircleGauge(SCREEN_W - 210, SCREEN_H - 100, 100.0f, Hp_framework, 25.0f, 0.75f);
        SetFontSize(28);
        //もしもボスが死にましたなら
        if(boss_ptr->Get_is_dead()) {
            if(count_after_bossdead > 0) {
                DrawString(x - 150, y, "嘘！床が落ち始めた！！", GetColor(255, 137, 49));
            }
            if(count_after_bossdead < 0) {
                DrawString(x - 150, y, "そうだな！", GetColor(255, 137, 49));
            }
        }
        break;
    case PLAYER_STATUS::CLEAR:
        break;
    }
    // モデルの大きさ設定
    MV1SetScale(model_handle, VGet(2.5f, 2.5f, 2.5f));
    MV1SetOpacityRate(model_handle, transparency);
    MV1SetRotationXYZ(model_handle, player_model_rot);
    MV1SetPosition(model_handle, VGet(pos.x, pos.y + 70.0f + SET_Y_ADJEST_VALUE, pos.z));
    MV1DrawModel(model_handle);

    //打つ弾丸
    SetUseLighting(TRUE);   //ライティングON
    DrawSphere3D(VGet(bullet_pos.x, bullet_pos.y + 200.0f, bullet_pos.z),
                 bullet_radius,
                 32,
                 GetColor(255, 100, 0),
                 GetColor(255, 0, 0),
                 TRUE);

    SetUseLighting(TRUE);   //ライティングON
}
//消去処理
//---------------------------------------------------
void Player::Exit()
{
    MV1DeleteModel(model_handle);
}