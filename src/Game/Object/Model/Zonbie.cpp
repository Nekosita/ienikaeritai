#include "Zonbie.h"
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

#define BACK               15.0f
#define SET_Y_ADJEST_VALUE 75.0f

const char* Total_serifu_Zonbie[] = {
    "早く戻らないと！",
    "こいつを倒したらGAMEOVERだ！",
};
//コンストラクタ
//-----------------------------------------------
Zonbie::Zonbie(size_t num)
    : move_count(0)
    , is_move(false)
    , is_live(true)
{
    zonbie_num = int(num);

    // モデル実装
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制mouseフォルダセットします
    std::string strM = std::string(str) + "/Data/Character/Zonbi/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("Idle.mv1");
    //Gameフォルダに戻します
    SetCurrentDirectory(str);

    MV1SetScale(model_handle, VGet(2.5f, 2.5f, 2.5f));   //モデルの大きさを変えます

    constexpr VECTOR ZONBIE_POS_LIST[] = {
#include "ZonbiePos.inc"
    };

    pos = ZONBIE_POS_LIST[zonbie_num];

    int StartRot    = GetRand(4);
    rot             = {0.0f, TO_RADIAN(StartRot * (90.0f)), 0.0f};
 
    //初期状態
    zonbie_action = STAY;
    target_rot    = 0.0f;    //移動先の角度を0にする
    is_move       = false;   //今はまだ動いでない
    roty          = 0.0f;
    move_count    = 0;

    //当たり判定用
    SetRadius(200.0f);
    Hp         = 350.f;
    is_live    = true;
    hurt_count = -99;
    Set_is_dead(false);

    //追加未整理
    catch_npc               = false;
    now_is_attacking_player = false;

    // 重力ラインの初期設定
    SetGravityLineStart(pos, 300.0f);
    SetGravityLineEnd(pos, 150.0f);

    // 普通のアニメーションの初期設定
    attachindex = MV1AttachAnim(model_handle, 0, anim_handle[anim_no], FALSE);
    totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    root_node   = 0;
    CreateIdentityMatrix(&identity);
}
Zonbie::~Zonbie()
{
    Exit();
}
void Zonbie::PlayAnimation()
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
bool Zonbie::MovieORDead()
{
    if((camera_ptr->GetOpenAnimeCount() > 0) || Check_Is_Dead())
        return true;
    return false;
}
bool Zonbie::CheckNowIsKitching()
{
    const VECTOR& npc_pos    = npc_ptr->GetPos();
    const float   npc_radius = npc_ptr->GetRadius();

    // もしも捕まう途中その上当たり判定入ったら判定に入る
    if(now_Iscatching &&
       ((AttachCheck(framePosL, part_radius, VGet(npc_pos.x, npc_pos.y + 125.f, npc_pos.z), npc_radius) ||
         AttachCheck(framePosR, part_radius, VGet(npc_pos.x, npc_pos.y + 125.f, npc_pos.z), npc_radius)))) {
        return true;
    }

    return false;
}
void Zonbie::Setcatch_npc(bool now_catch)
{
    catch_npc = now_catch;
}
void Zonbie::ReleaseNpc()
{
    catch_npc = false;
}
void Zonbie::reload_now_is_attacking()
{
    now_is_attacking = -240;
}

void Zonbie::CalculateAngle_zonbi_to_target(const VECTOR& pos_1,
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
void Zonbie::Update()
{
    const VECTOR& npc_rot       = npc_ptr->GetRot();
    const VECTOR& npc_pos       = npc_ptr->GetPos();
    const VECTOR& player_rot    = player_ptr->GetRot();
    const VECTOR& player_pos    = player_ptr->GetPos();
    VECTOR        Standerd      = {0.0f, 0.0f, -1.0f};   //基準用の角度
    VECTOR        Zonbie_to_npc = {0.0f, 0.0f, 0.0f};    //NPCに向けている方向の初期値

    //暫定用
    VECTOR Zonbie_to_goal = {0.0f, 0.0f, 0.0f};

    GOAL_AREA = {15.0f * 200.0f, 1750.f, 50.0f * 200.0f};   //暫定の消失ポイント

    Zonbie_to_goal.x = GOAL_AREA.x - pos.x;
    Zonbie_to_goal.z = GOAL_AREA.z - pos.z;

    VECTOR Zonbie_to_player = {0.0f, 0.0f, 0.0f};
    Zonbie_to_player.x      = (player_pos.x - pos.x);
    Zonbie_to_player.z      = (player_pos.z - pos.z);

    Zonbie_to_npc.x = (npc_pos.x - pos.x);
    Zonbie_to_npc.z = (npc_pos.z - pos.z);


    //!<演出中あるいは死んだとき
    if(MovieORDead()) {
        if(camera_ptr->GetOpenAnimeCount() > 0) {
            zonbie_action = STAY;
        }
        if(Check_Is_Dead()) {
            zonbie_action = DEAD;
        }
    }
    else {
        if(hurt_count > 0) {
            zonbie_action = HURT;
        }
        else {
            if(catch_npc) {
                zonbie_action = CATCH;
            }
            else {
                //!もしもNPCを捕まったら特定の処理に入る
                if(npc_ptr->Getbe_catch() && !catch_npc) {
                    now_is_attacking++;
                    if(now_is_attacking < 0) {
                        zonbie_action = PLAYER_ATTACK;
                    }
                    if(now_is_attacking > 0) {
                        zonbie_action = THINKING;
                    }
                }
                else {
                    //!普通に動く
                    if(GetDistance(pos, player_pos) < 7000.f) {
                        zonbie_action = MOVE;
                    }
                }
            }
        }
    }

    hurt_count--;
    switch(zonbie_action) {
    case Zonbie::STAY:
        transparency = 2.0f;
        catch_npc    = false;
        //２秒ことにランダに角度変えて動いて行く
        if(move_count == 0) {
            SetAnimation(anim_handle, 0);
            SetPosBeforeMove(pos, target_rot);
        }
        SetAnimation(anim_handle, 5);
        rot.y = target_rot;
        pos.x += (float)((-1) * (1 * sin(rot.y)));
        pos.z += (float)((-1) * (1 * cos(rot.y)));
        //移動
        move_count++;
        //2秒を越え
        if(move_count > 240) {
            move_count = 0;
        }
        break;
    case Zonbie::THINKING:
        transparency = 2.0f;
        SetAnimation(anim_handle, 0);
        if(now_is_attacking > 30) {
            reload_now_is_attacking();
        }
        break;
    case Zonbie::MOVE:
        //アニメーション設定
        //!もしもプレイヤーを捕まったら　強制できにCATCHに入る
        transparency = 2.0f;
        if((GetDistance(pos, npc_pos) > 200.0f)) {
            SetAnimation(anim_handle, 2);
            //!捕まっていない
            catch_npc      = false;
            now_Iscatching = false;
            //NPCに向いて捕まいに行く
            CalculateAngle_zonbi_to_target(npc_pos, pos, Standerd, Zonbie_to_npc);
            pos.x += (float)((-1) * (30 * sin(roty)));
            pos.z += (float)((-1) * (30 * cos(roty)));
        }
        if((GetDistance(pos, npc_pos) < 200.0f)) {
            now_Iscatching = true;
            SetAnimation(anim_handle, 1);
            CalculateAngle_zonbi_to_target(npc_pos, pos, Standerd, Zonbie_to_npc);
            //!<NPCを捕まっているがどうがを確認
            if(CheckNowIsKitching()) {
                catch_npc = true;
            }
        }
        break;
    case Zonbie::CATCH:
        SetAnimation(anim_handle, 6);
        transparency = 2.0f;
        //!NPCを捕まった
        catch_npc = true;
        //NPCが消滅ポイントまで走る
        CalculateAngle_zonbi_to_target(GOAL_AREA, pos, Standerd, Zonbie_to_goal);
        pos.x += (float)((-1) * (20.f * sin(roty)));
        pos.z += (float)((-1) * (20.f * cos(roty)));
        break;
    case Zonbie::PLAYER_ATTACK:
        transparency = 2.0f;
        catch_npc      = false;   //NPCを捕まえて無い
        now_Iscatching = false;
        if(GetDistance(pos, player_pos) < 350.f) {
            now_is_attacking_player = true;
            SetAnimation(anim_handle, 3);
            CalculateAngle_zonbi_to_target(player_pos, pos, Standerd, Zonbie_to_player);
            attack_rot = roty;
        }
        else {
            SetAnimation(anim_handle, 2);
            //角度を戻す
            now_is_attacking_player = false;
            CalculateAngle_zonbi_to_target(player_pos, pos, Standerd, Zonbie_to_player);
            pos.x += (float)((-1) * (25.f * sin(roty)));
            pos.z += (float)((-1) * (25.f * cos(roty)));
        }
        if(now_is_attacking > 30) {
            reload_now_is_attacking();
        }

        break;
    case Zonbie::HURT:
        //アニメーション設定
        catch_npc               = false;
        now_is_attacking_player = false;   //殴られてた時フォールすにします
        SetAnimation(anim_handle, 4);
        Hp -= 2.0f;
        if((hurt_count < 0) && (Hp > 0.0f)) {
            Reset_tomove();
        }
        //透明度
        if((hurt_count % 5) == 0) {
            transparency = 0.5f;
        }
        else {
            transparency = 1.0f;
        }
        //移動量を沿って動きます
        pos.x -= (float)(BACK * sin(hurt_rot));
        pos.z -= (float)(BACK * cos(hurt_rot));
        break;
    case Zonbie::DEAD:
        SetAnimation(anim_handle, 4);
        catch_npc = false;
        transparency -= 0.01f;
        if(transparency < 0) {
            is_live      = false;
            transparency = 0.0f;
            pos          = {28.0f * 200.0f, -15000.0f, 0.0f * 200.0f};
        }
        break;
    }

    //手の座標設定
    framePosL = MV1GetFramePosition(model_handle, 22);
    framePosR = MV1GetFramePosition(model_handle, 43);

    screen_position = ConvWorldPosToScreenPos(pos);

    Gravity();
    //アニメーションの処理をする
    PlayAnimation();
    GravityLine();
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
    MV1GetFramePosition(model_handle, 0);
}
void Zonbie::Gravity()
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
void Zonbie::Render()
{
    //2D表示計算
    auto screen_position = ConvWorldPosToScreenPos(pos);
    //位置の上の側に表示
    int x = static_cast<int>(screen_position.x);         //X座標設定
    int y = static_cast<int>(screen_position.y) - 100;   //Y座標設定

    SetFontSize(20);
    //攻撃状態の描画設定
    switch(zonbie_action) {
    case Zonbie::HURT:
        break;
    case Zonbie::CATCH:
        DrawString(x, y, Total_serifu_Zonbie[0], GetColor(255, 255, 225));
        break;
    case Zonbie::PLAYER_ATTACK:
        DrawString(x, y, Total_serifu_Zonbie[1], GetColor(255, 255, 225));
        break;
    case Zonbie::DEAD:

        break;
    }

    SetFontSize(60);
   
    MV1SetOpacityRate(model_handle, transparency);

    //平常状態の描画設定
    MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
    MV1SetPosition(model_handle, VGet(pos.x, pos.y + 190.f, pos.z));
    MV1DrawModel(model_handle);
}
bool Zonbie::Check_Is_Dead()
{
    if(Hp < 0.0f || pos.y < 0)
        return true;
    return false;
}
void Zonbie::SetPosBeforeMove(VECTOR& pos, float rot)
{
    pos_before_move = pos;   //動く前の位置を記録

    float Rand = GetRandomF(0.0f, 360.0f);   //ランダム(0~360)な数値を設定

    target_rot = TO_RADIAN(Rand);   //それをターゲットとして設定
}
//固定位置用の関数
//--------------------
void Zonbie::Fixpos(VECTOR& pos)
{
    this->pos = {pos.x, pos.y, pos.z};
}
//終了処理
//---------------
void Zonbie::Exit()
{
    // return nullptr;
    MV1DeleteModel(model_handle);
}
void Zonbie::SetIsLive(bool now_live)
{
    is_live = now_live;
}
bool Zonbie::GetIsLive() const
{
    return is_live;
}
void Zonbie::GravityLine()
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
void Zonbie::ResetBeforeCheck()
{
    on_move_block = false;
    is_stepped    = false;
}
void Zonbie::SetDownWhenHit(float block_hight)
{
    SetIsStepped(true);
    SetPosY(block_hight);
}
bool Zonbie::HitWithStageBlock(const BlockBase* block)
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
bool Zonbie::HitWithMoveBlock(const MoveBlock* block)
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
void Zonbie::SetIsStepped(bool now_is_stepped)
{
    is_stepped = now_is_stepped;
}
void Zonbie::SetPosY(float fix_y)
{
    pos.y = fix_y + SET_Y_ADJEST_VALUE;
}
void Zonbie::SetPosOnMoveBlock(const Vector3& dir)
{
    pos.x += dir.x;
    pos.y += dir.y + SET_Y_ADJEST_VALUE;
    pos.z += dir.z;
}
void Zonbie::Set_Transparency(float now_transparency)
{
    transparency = now_transparency;
}
void Zonbie::Set_is_dead(bool now_is_dead)
{
    is_dead = now_is_dead;
}
void Zonbie::Set_Hp(bool now_HP)
{
    Hp = now_HP;
}
void Zonbie::SetPos_HurtByPlayerBullet(float bullet_rot)
{
    hurt_count = 60;
    zonbie_action = HURT;
    hurt_rot      = bullet_rot;
}
void Zonbie::Reset_tomove()
{
    zonbie_action = MOVE;
}