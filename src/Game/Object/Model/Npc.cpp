#pragma once
#include "Npc.h"
#include "../src/Game/Object/Stage/MoveBlock/MoveBlock.h"
#include "../src/System/AppendFunction.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Model/Boss.h"
#include "../src/Game/Object/Model/RedGoblin.h"
#include "../src/System/AppendFunction.h"
#include "../src/Game/Object/GoalArea.h"
#include "../src/Game/Object/Stage/CheckArea/CheckArea.h"
#include "../src/Game/Object/Stage/Npc_Sheltter/Npc_Sheltter.h"
#include "../src/System/Vector3.h"
#include "../src/Game/Object/Camera.h"
#include "../src/System/KeyInput.h"
#include "DxLib.h"

#define SET_Y_ADJEST_VALUE 15.0f
const char* Total_serifu_Npc[] = {
    "護衛は任せたぞ！",
    "じゃっ行きましょう！",
    "俺は終わるまでここで待ちます！",
};
//コンストラクタ
//-----------------------------------------------
Npc::Npc()
{
    // 普通のモデル設定
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制mouseフォルダセットします
    std::string strM = std::string(str) + "/Data/Character/mouse/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("Neutral Idle.mv1");
    //Gameフォルダに戻します
    SetCurrentDirectory(str);

    // モデルの大きさ設定
    MV1SetScale(model_handle, VGet(1.3f, 1.3f, 1.3f));   //モデルの大きさを変えます

    // プレイヤーの状態の初期設定
    // プレイヤー初期座標の設定
    VECTOR start_pos = {-15 * 200.0f, 3000.f, 10 * 200.0f};
    SetPos(start_pos);

    //プレイヤー初期のrotを設定
    VECTOR start_rot{0.0f, 270.0f, 0.0f};
    SetRot(start_rot);
    gravity    = 0.0f;                    //重力の初期化
    is_stepped = false;                   //最初は踏めてない
    SetRadius(125.0f);                    //当たり半径設定
    is_run   = false;                     //最初走ってない
    stand_by = rot.y + TO_RADIAN(0.0f);   //is_run = false; の時使ったロット

    now_npc = COMMON;

    //ボス
    after_boss_dead = false;

    //HPが削られてない
    Hp                 = 0.0f;
    hurt_little_people = 0.3f;
    Hp_handle          = LoadGraph("Data/UI/npc_hp.png");
    Hp_framework       = LoadGraph("Data/UI/framework_npc.png");
    hurt               = LoadGraph("Data/UI/damage.png");
    stay               = LoadGraph("Data/UI/stand.png");
    fallowing[0]       = LoadGraph("Data/UI/run_1.png");
    fallowing[1]       = LoadGraph("Data/UI/run_2.png");
    be_catch_picture   = LoadGraph("Data/UI/be_catch.png");
    frame              = LoadGraph("Data/UI/frame.png");
    running_count      = 0;

    // ゴールバー用の変数初期化
    is_goal_actived = false;
    //ダメージ用設定
    hurt_count = -99;
    wave_count = -99;
    ball_count = -99;

    // 普通のアニメーションの初期設定
    attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
    totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    root_node   = 1;
    GravityLine();
    CreateIdentityMatrix(&identity);
}
//デストラクタ
//---------------------------------
Npc::~Npc()
{
    Exit();
}
//アニメの設定
//---------------------------------
void Npc::SetAnimation(const std::vector<int>& handle, int new_anim_no)
{
    if(anim_no != new_anim_no) {
        MV1DetachAnim(model_handle, attachindex);
        anim_no     = new_anim_no;
        attachindex = MV1AttachAnim(model_handle, 1, handle[anim_no], FALSE);
        totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    }
}
//プレイヤーとブロックの確認用の関数
//--------------------------------------------------
//判定が被らないように　先に上と下の判定を消します
//--------------------------------------------------
void Npc::ResetBeforeCheck()
{
    on_move_block = false;
    is_stepped    = false;
}
//踏んでいるのかとぶつかっているのかの関数
//---------------------------------
void Npc::SetIsStepped(bool now_is_stepped)
{
    is_stepped = now_is_stepped;
}
bool Npc::CheckHitGoalBar(bool is_on_goal, float hight)
{
    if(is_on_goal) {
        is_goal_actived = true;
        SetPosY(hight);   //ここで止で欲しいY座標をプレイヤーに渡す
    }
    else {
        is_goal_actived = false;
    }
    return is_goal_actived;
}
bool Npc::HitWithStageBlock(const BlockBase* block)
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
    SetDownWhenHit(block->GetBlockHight());
    return true;
}
bool Npc::HitWithMoveBlock(const MoveBlock* block)
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
    SetDownWhenHit(block->GetBlockHight());
    //ブロックのヴェクターを渡します
    SetPosOnMoveBlock(block->GetMoveVector());
    return on_move_block;
}
void Npc::SetPosOnMoveBlock(const Vector3& dir)
{
    on_move_block = true;
    pos.x += dir.x;
    pos.y += dir.y - 25.f;
    pos.z += dir.z;
}
void Npc::Gravity()
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
void Npc::SetDownWhenHit(float block_hight)
{
    is_stepped = true;
    SetIsStepped(true);
    SetPosY(block_hight);
}
void Npc::SetPosY(float fix_y)
{
    pos.y = fix_y;
}
void Npc::Move()
{
    VECTOR Standerd  = {0.0f, 0.0f, -1.0f};   //基準用NPCの角度
    VECTOR to_player = {0.0f, 0.0f, 0.0f};    //NPCがプレイヤーに向けている方向の初期値
    //プレイヤーに向かっているベクターを作る
    //VECTORを基準化
    to_player.x = (player_ptr->GetPos().x - pos.x);
    to_player.z = (player_ptr->GetPos().z - pos.z);

    if((GetDistance(pos, player_ptr->GetPos()) < 130.f)) {
        SetAnimation(anim_handle, 0);
    }
    else {
        is_run = true;   //走っています
        SetAnimation(anim_handle, 1);
        float roty = CalculateAngle(Standerd, to_player);

        //計算した角度はプラスとマイナスがない
        //それを位置に合わせて調整します
        roty *= -1;
        if(player_ptr->GetPos().x < pos.x)
            roty = -(roty);
        rot.y              = roty;
        player_model_rot.y = rot.y;

        //指定された方向で進む
        pos.x += (float)((-1) * (NPC_SPEED * sin(rot.y)));
        pos.z += (float)((-1) * (NPC_SPEED * cos(rot.y)));
    }
}
void Npc::Move_AfterBossDead()
{
    VECTOR Standerd  = {0.0f, 0.0f, -1.0f};   //基準用NPCの角度
    VECTOR to_player = {0.0f, 0.0f, 0.0f};    //NPCがプレイヤーに向けている方向の初期値
    //プレイヤーに向かっているベクターを作る
    //VECTORを基準化
    to_player.x = (player_ptr->GetPos().x - pos.x);
    to_player.z = (player_ptr->GetPos().z - pos.z);

    is_run = true;   //走っています
    SetAnimation(anim_handle, 1);
    float roty = CalculateAngle(Standerd, to_player);

    //計算した角度はプラスとマイナスがない
    //それを位置に合わせて調整します
    roty *= -1;
    if(player_ptr->GetPos().x < pos.x)
        roty = -(roty);
    rot.y              = roty;
    player_model_rot.y = rot.y;

    //もしもプレーヤーの所に地下ついてたら　COMMON の処理に戻ります
    //ボスが死んたらプレーヤーまで最速に走るように　
    //１回しか使わない
    if(GetDistance(pos, player_ptr->GetPos()) < 130.f) {
        after_boss_dead = true;
        is_stay         = true;
    }
    //プレイヤーに近くにいたら遅くなるさせるように
    pos.x += to_player.x / 5;
    pos.z += to_player.z / 5;
}
//プレイヤーアニメーション演出
//---------------------------------------------------------
void Npc::PlayAnimation()
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
void Npc::GravityLine()
{
    // 重力ラインの設定
    SetGravityLineStart(pos, 75.0f);
    SetGravityLineEnd(pos, 210.0f);

    // 頭上ラインの設定
    SetTopLineStart(pos, 175.0f);
    SetTopLineEnd(pos, 250.0f);
}

//プレイヤーのアップデート
//-----------------------------------
void Npc::Update()
{

    //もしもボスが死んだら      シェスタから出ます
    if(boss_ptr->Get_is_dead()) {
        in_the_sheltter = false;
    }
    else {
        //安全のシェルターの中にいるがどうがを確認
        if(check_ptr->Get_player_is_in_area()) {
            in_the_sheltter = true;
        }
    }

    //!<演出の中
    if(Check_MovieNowShowing()) {
        if(comera_ptr->GetOpenAnimeCount() > 0) {
            now_npc = START_MOVIE;
        }
        if(comera_ptr->GetEndAnimeCount() < 90) {
            now_npc = GAME_CLEAR_MOVIE;
        }
    }

    //!<演出の中ではな時
    if(!Check_MovieNowShowing()) {
        if(in_the_sheltter)now_npc = IN_SHELTTER;
        if(be_catch)now_npc = BE_CATCH;
        //掴まれない、シェルターの中にもいないの時
        if(!in_the_sheltter && !be_catch) {
            now_npc = COMMON;
            if(Check_Is_hurt())now_npc = HURT;
            if(boss_ptr->Get_is_dead() && !after_boss_dead) {
                now_npc = AFTERBOSSDEAD;
            }
        }
    }

    //NPCが下まで落ちる、HPが消える、或いは消滅ポイントまで運ばれるなら強制死亡
    if((GetPos().y < 0.0f) || (GetHp() > 50.0f) || in_the_dead_point) {
        is_dead = true;
    }

    hurt_count--;
    ball_count--;
    switch(now_npc) {
    case NPC_STATUS::START_MOVIE:
        pos = VGet(-20 * 200.0f, 1760.f, 10 * 200.0f);
        //プレイヤー初期rotを設定（向いている
        player_model_rot.y = TO_RADIAN(0.0f);
        //pos = VGet(195 * 200.0f, 1760.f, 15 * 200.0f);
        ////プレイヤー初期rotを設定（向いている
        //player_model_rot.y = TO_RADIAN(0.0f);
        //pos = VGet(120 * 200.0f, 1760.f, -10 * 200.0f);
        ////プレイヤー初期rotを設定（向いている
        //player_model_rot.y = TO_RADIAN(0.0f);
        //pos = VGet(220 * 200.0f, 4150.f, 10 * 200.0f);
        ////プレイヤー初期rotを設定（向いている
        //player_model_rot.y = TO_RADIAN(0.0f);
        break;
    case NPC_STATUS::HURT:
        be_catch = false;
        // 演出処理
        SetAnimation(anim_handle, 4);
        // ダメージ処理
        GetHurt_bullet();
        //0.4~0.6の間循環
        if(hurt_little_people > 0.42) {
            hurt_little_people = 0.3f;
        }
        hurt_little_people += 0.01f;
        break;
    case NPC_STATUS::COMMON:
        //移動処理
        Move();
        break;
    case NPC_STATUS::BE_CATCH:
        //つかまれた時攻撃を受けない
        SetAnimation(anim_handle, 4);
        //!捕まれた時消滅ポイントに入ったら強制できに死亡フラグに入ります　
        CheckIsInDeadPoint();
        break;
    case NPC_STATUS::IN_SHELTTER:
        //シェルターにいる時
        SetAnimation(anim_handle, 0);
        //!ボスが死んだらAFTERBOSSDEADに入るように
        pos.x = sheltter_ptr->Getsheltterpos().x;
        pos.y = sheltter_ptr->Getsheltterpos().y;
        pos.z = sheltter_ptr->Getsheltterpos().z;
        break;
    case NPC_STATUS::AFTERBOSSDEAD:
        //1回用
        Move_AfterBossDead();
        break;
    case NPC_STATUS::GAME_CLEAR_MOVIE:
        if(comera_ptr->GetEndAnimeCount() > 88) {
            pos = VGet(280.0f * 200.f, 4150.f, 5.0f * 200.f);
        }
        SetAnimation(anim_handle, 1);
        player_model_rot.y = TO_RADIAN(270.0f);
        pos.y              = 4150.f;
        pos.x += (float)((310.0 * 200.0f - pos.x) / 225);
        pos.z += (float)((10.0f * 200.0f - pos.z) / 225);
        break;
    }

    if(is_run) {
        running_count++;
    }

    //ボスが死んた時
    if(boss_ptr->Get_is_dead()) {
        count_after_bossdead--;
    }

    //重力処理
    GravityLine();
    Gravity();
    //アニメーションの処理をする
    PlayAnimation();
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
//描画処理用関数
//----------------------------------------------
void Npc::Render(bool shadow)
{
    //2D表示計算
    auto screen_position = ConvWorldPosToScreenPos(pos);
    //位置の上の側に表示
    int x = static_cast<int>(screen_position.x);         //X座標設定
    int y = static_cast<int>(screen_position.y) - 100;   //Y座標設定
    SetUseLighting(FALSE);                               //ライティングOFF
    switch(now_npc) {
    case NPC_STATUS::START_MOVIE:
        SetFontSize(50);
        if(!shadow) {
            if((comera_ptr->GetOpenAnimeCount() > 110) && (comera_ptr->GetOpenAnimeCount() < 230)) {
                DrawString(x - 500, y - 100, Total_serifu_Npc[0], GetColor(19, 46, 225));
            }
            if((comera_ptr->GetOpenAnimeCount() < 60)) {
                DrawString(x - 500, y - 100, Total_serifu_Npc[1], GetColor(19, 46, 225));
            }
        }
        SetFontSize(40);
        break;
    case NPC_STATUS::BE_CATCH:
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 100.0f, Hp_framework, 0.0f, 0.75f);
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 25.0f, Hp_handle, -25.0f + Hp, 0.75f);
        DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 190, 0.4f, 0.0, be_catch_picture, TRUE, FALSE);
        DrawRotaGraph(SCREEN_W - 168, SCREEN_H - 190, 1.25f, 0.0, frame, TRUE, FALSE);
        //文字描画
        SetFontSize(28);
        DrawString(SCREEN_W - 100, SCREEN_H - 125, "NPC", GetColor(255, 255, 225));
        DrawString(SCREEN_W - 130, SCREEN_H - 60, "PLAYER", GetColor(255, 255, 225));
        break;
    case NPC_STATUS::HURT:
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 100.0f, Hp_framework, 0.0f, 0.75f);
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 25.0f, Hp_handle, -25.0f + Hp, 0.75f);
        DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 190, hurt_little_people, 0.0, hurt, TRUE, FALSE);
        DrawRotaGraph(SCREEN_W - 168, SCREEN_H - 190, 1.25f, 0.0, frame, TRUE, FALSE);
        //文字描画
        SetFontSize(28);
        DrawString(SCREEN_W - 100, SCREEN_H - 125, "NPC", GetColor(255, 255, 225));
        DrawString(SCREEN_W - 130, SCREEN_H - 60, "PLAYER", GetColor(255, 255, 225));
        break;
    case NPC_STATUS::COMMON:
        if(anim_no == 0) {
            DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 195, 0.4f, 0.0, stay, TRUE, FALSE);
        }
        else {
            if((running_count % 31) < 16) {
                DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 195, 0.4f, 0.0, fallowing[0], TRUE, FALSE);
            }
            else {
                DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 195, 0.4f, 0.0, fallowing[1], TRUE, FALSE);
            }
        }
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 100.0f, Hp_framework, 0.0f, 0.75f);
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 25.0f, Hp_handle, -25.0f + Hp, 0.75f);
        DrawRotaGraph(SCREEN_W - 168, SCREEN_H - 190, 1.25f, 0.0, frame, TRUE, FALSE);
        //文字描画
        SetFontSize(28);
        DrawString(SCREEN_W - 100, SCREEN_H - 125, "NPC", GetColor(255, 255, 225));
        DrawString(SCREEN_W - 130, SCREEN_H - 60, "PLAYER", GetColor(255, 255, 225));
        //もしもボスが死にましたなら
        if(!shadow) {
            if(boss_ptr->Get_is_dead()) {
                if(count_after_bossdead > 0) {
                    DrawString(x + 100, y, "あれだけ攻撃したら落ちるでしょう！！", GetColor(19, 46, 225));
                }
                if(count_after_bossdead < 0) {
                    DrawString(x + 100, y, "早く矢印の所へ行きましょう！！", GetColor(19, 46, 225));
                }
            }
        }
        break;
    case NPC_STATUS::IN_SHELTTER:
        DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 195, 0.4f, 0.0, stay, TRUE, FALSE);
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 100.0f, Hp_framework, 0.0f, 0.75f);
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 25.0f, Hp_handle, -25.0f + Hp, 0.75f);
        DrawRotaGraph(SCREEN_W - 168, SCREEN_H - 190, 1.25f, 0.0, frame, TRUE, FALSE);
        //文字描画
        SetFontSize(28);
        if(!shadow) {
            DrawString(x, y, Total_serifu_Npc[2], GetColor(19, 46, 225));
        }
        DrawString(SCREEN_W - 100, SCREEN_H - 125, "NPC", GetColor(255, 255, 225));
        DrawString(SCREEN_W - 130, SCREEN_H - 60, "PLAYER", GetColor(255, 255, 225));
        break;
    case NPC_STATUS::AFTERBOSSDEAD:
        if((running_count % 31) < 16) {
            DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 195, 0.4f, 0.0, fallowing[0], TRUE, FALSE);
        }
        else {
            DrawRotaGraph(SCREEN_W - 108, SCREEN_H - 195, 0.4f, 0.0, fallowing[1], TRUE, FALSE);
        }
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 100.0f, Hp_framework, 0.0f, 0.75f);
        DrawCircleGauge(SCREEN_W - 110, SCREEN_H - 195, 25.0f, Hp_handle, -25.0f + Hp, 0.75f);
        DrawRotaGraph(SCREEN_W - 168, SCREEN_H - 190, 1.25f, 0.0, frame, TRUE, FALSE);
        ////文字描画
        //文字描画
        SetFontSize(28);
        DrawString(SCREEN_W - 100, SCREEN_H - 125, "NPC", GetColor(255, 255, 225));
        DrawString(SCREEN_W - 130, SCREEN_H - 60, "PLAYER", GetColor(255, 255, 225));
        SetFontSize(28);
        if(!shadow) {
            DrawString(x + 150, y, "あれだけ攻撃したら落ちるでしょう！！", GetColor(19, 46, 225));
        }
        break;
    case NPC_STATUS::CLEAR:
        break;
    }
    SetFontSize(60);

    DrawSphere3D(TEST_POS, 500.f, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);

   //デバッグ用
    //DrawSphere3D(VGet(pos.x, pos.y + 125.0f, pos.z), GetRadius(),
    //             32,
    //             GetColor(255, 125, 0),
    //             GetColor(255, 0, 0),
    //             FALSE);

    // モデルの大きさ設定
    MV1SetScale(model_handle, VGet(1.3f, 1.3f, 1.3f));   //モデルの大きさを変えます
    MV1SetRotationXYZ(model_handle, player_model_rot);
    MV1SetPosition(model_handle, VGet(pos.x, pos.y + 75.0f + SET_Y_ADJEST_VALUE, pos.z));
    MV1DrawModel(model_handle);
    SetUseLighting(TRUE);   //ライティング
}
//消去処理
//---------------------------------------------------
void Npc::Exit()
{
    MV1DeleteModel(model_handle);
}
void Npc::ReleaseFromZonbie()
{
    be_catch = false;
}
void Npc::SetPosBeCatch(const Vector3& Hand_Pos)
{
    //捕まっていた
    be_catch = true;
    pos.x    = Hand_Pos.x;
    pos.y    = Hand_Pos.y - 50.f;
    pos.z    = Hand_Pos.z;
}
bool Npc::CheckIsInDeadPoint()
{
    //!もしも死亡ポイントの中に入ったら
    if(AttachCheck(pos, GetRadius(), TEST_POS, 500.f)) {
        in_the_dead_point = true;
    }
    return false;
}
bool Npc::Check_MovieNowShowing()
{
    if((comera_ptr->GetOpenAnimeCount() > 0) || (comera_ptr->GetEndAnimeCount() < 90))
        return true;
    return false;
}
//ステージ再開する度のデーター更新
//------------------------------------------------------
void Npc::NpcReset()
{
    is_goal_actived = false;
    SetAnimation(anim_handle, 6);
    pos = VGet(-10 * 200.0f, 1760.f, 10 * 200.0f);
    //プレイヤー初期rotを設定（向いている
    player_model_rot.y   = TO_RADIAN(0.0f);
    Hp                   = 0.0f;
    running_count        = 0;
    hurt_little_people   = 0.3f;
    hurt_count           = -99;
    is_dead              = false;
    in_the_dead_point    = false;
    be_catch             = false;
    in_the_sheltter      = false;
    after_boss_dead      = false;
    count_after_bossdead = 90;
}
//ダメージ関係設定
//----------------------------------------------------------
void Npc::ResetCount()
{
    hurt_count = 40;
}
bool Npc::GetHurt_bullet()
{
    return Hp += 0.2f;
    hurt_count--;
}
float Npc::Heal()
{
    Hp -= 25.0f;
    if(Hp < 0.0f) {
        //HPの最大値決め
        Hp = 0.0f;
    }
    return Hp;
}
bool Npc::Check_Is_hurt()
{
    if((hurt_count > 0) || (ball_count > 0)) {
        return true;
    }
    return false;
}
