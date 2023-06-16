#include "Boss.h"
#include "ModelBase.h"
#include "../src/System/Random.h"
#include "../src/System/Vector3.h"
#include "../src/System/Effect.h"
#include "../src/System/AppendFunction.h"
#include "../src/Game/Object/Stage/MoveBlock/MoveBlock.h"
#include "Player.h"
#include "Npc.h"
#include "DxLib.h"
#include <vector>
#include <array>

#define SET_Y_ADJEST_VALUE 325.0f
#define BACK               2.f

//コンストラクタ
//-----------------------------------------------
Boss::Boss(size_t num)
{
    boss_num = int(num);

    // 普通のモデル設定
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制Bossフォルダセットします
    std::string strM = std::string(str) + "/Data/Character/Boss/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("Seated Idle.mv1");
    //Gameフォルダに戻します
    SetCurrentDirectory(str);

    //初期状態
    boss_action = STAY;
    MV1SetScale(model_handle, VGet(5.0f, 5.0f, 5.0f));   //モデルの大きさを変えます

    //位置設定
    constexpr VECTOR BOSS_POS_LIST[] = {
        {270.0f * 200.0f, 4400.0f, 10.0f * 200.0f},
    };

    pos = BOSS_POS_LIST[boss_num];

    //角度設定
    float Start_rot = TO_RADIAN(90.0f);
    rot             = {0.0f, Start_rot, 0.0f};

    //当たり半径設定
    SetRadius(400.0f);
    //打たれ無い
    hurt_count = -99;
    //まだ接触していない
    is_attatched = false;
    //HPが削られてない
    Hp = 600.f;
    //未だ死んでない
    is_dead = false;

    //モデル用設定
    root_node = 0;
    CreateIdentityMatrix(&identity);
}
Boss::~Boss()
{
    Exit();
}
void Boss::PlayAnimation()
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
void Boss::SetIsAttatched(bool now_)
{
    is_attatched = now_;
}
void Boss::Set_Transparency(float now_transparency)
{
    transparency = now_transparency;
}
void Boss::Set_is_dead(bool now_is_dead)
{
    is_dead = now_is_dead;
}
void Boss::ResetSetHurtCount()
{
    hurt_count = 30;
}
void Boss::Set_Hp(bool now_HP)
{
    Hp = now_HP;
}
//固定位置用の関数
//--------------------
void Boss::Fixpos(VECTOR& pos)
{
    this->pos = {pos.x, pos.y, pos.z};
}
bool Boss::CheckHitWithPlayerBullet()
{
    return AttachCheck_PlayertoBoss(player_ptr->GetBulletPos(), player_ptr->GetBulletRadius(), GetPos(), GetRadius());
}
void Boss::SetPos_HurtByPlayerBullet(float bullet_rot)
{
    hurt_count = 30;
    hurt_rot   = bullet_rot;
}
void Boss::ResetAllCounterAndKind()
{
    //一番最初のカウンター
    start_attack_count = 0;
    //HPが200以上の時のカウンター
    now_is_attacking_start_hp_up200_count = -300;
    //HPが200以下の時のカウンター
    now_is_attacking_start_hp_down200_count = -300;
    attack_kind_up200_count                 = 0;
    attack_kind_down200_count               = 0;
} 
void Boss::Attacking_When_Hp_up_200()
{
    now_is_attacking_start_hp_up200_count++;
    if(now_is_attacking_start_hp_up200_count > 0) {
        boss_action = THINKING;
    }
    //0~4 50％の確率
    if((attack_kind_up200_count < 5) && (now_is_attacking_start_hp_up200_count < 0)) {
        boss_action = JUMP_ATTACK;
    }
    //5~7 30％の確率
    if((attack_kind_up200_count >= 5) && (attack_kind_up200_count <= 7) &&
       (now_is_attacking_start_hp_up200_count < 0)) {
        boss_action = ATTACK_DOWNWARD;
    }
    //8~9 20％の確率
    if((attack_kind_up200_count >= 8) && (attack_kind_up200_count <= 9) &&
       (now_is_attacking_start_hp_up200_count < 0)) {
        boss_action = BALL_SUMMON;
    }
}
void Boss::Attacking_When_Hp_down_200()
{

    now_is_attacking_start_hp_down200_count++;
    if(now_is_attacking_start_hp_down200_count > 0) {
        boss_action = THINKING;
    }
    //0~4 50％の確率
    if((attack_kind_down200_count < 5) && (now_is_attacking_start_hp_down200_count < 0)) {
        boss_action = WAVE_;
    }
    //5~7 30％の確率
    if((attack_kind_down200_count >= 5) && (attack_kind_down200_count <= 7) &&
       (now_is_attacking_start_hp_down200_count < 0)) {
        boss_action = JUMP_ATTACK;
    }
    //8~9 20％の確率
    if((attack_kind_down200_count >= 8) && (attack_kind_down200_count <= 9) &&
       (now_is_attacking_start_hp_down200_count < 0)) {
        boss_action = BALL_SUMMON;
    }
}
void Boss::Get_Hurt()
{
    //HPの削り
    Hp -= 0.25f;
    //透明度切り替え
    transparency_in_damage(Gehurtcount());
    //蓄積用のカウンターをリセットします
    accumlate_count = 0;
}
void Boss::Get_Hurt_in_bigamage()
{
    //HPの削り（普通より痛い
    Hp -= 1.0f;
    //透明度切り替え
    transparency_in_damage(Gehurtcount());
}
void Boss::transparency_in_damage(int damage_count)
{
    if(damage_count % 5 == 0) {
        transparency = 0.5f;
    }
    else {
        transparency = 1.0f;
    }
}
void Boss::ResetAllAttackBool()
{
    //攻撃時で使われたトリガーを全てリセットします。
    is_jump_attack                    = false;   //Jump Attack用のトリガー
    ball_summon                       = false;   //天上からボールを召喚する用のトリガー
    is_standing_melee_attack_downward = false;   //standing_melee_attack_downward用のトリガー
    on_wave                           = false;   //衝撃波用のトリガー
}
void Boss::CalculateAngle_boss_to_player(const VECTOR& pos_1,
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
void Boss::Update()
{
    const VECTOR& player_pos = player_ptr->GetPos();
    const VECTOR& player_rot = player_ptr->GetRot();
    VECTOR        Standerd   = {0.0f, 0.0f, -1.0f};   //基準用のモデル角度
    VECTOR Boss_to_player = {0.0f, 0.0f, 0.0f};   //ゴブリンがプレイヤーに向けている方向の初期値
    Boss_to_player.x      = (player_pos.x - pos.x);
    Boss_to_player.z      = (player_pos.z - pos.z);

    //開幕演出
    //1回しか入らない
    if((GetDistance(pos, player_pos) < 5000.0f) && !is_attatched) {
        boss_action = ATTACK_START;
    }

    accumlate_count++;
    //蓄積計算用(指定時間以内に指定のダメージに届いてないなら計算リセット
    if(accumlate_count > 180) {
        //カウンターリセット
        accumlate_damage = 0;
        //普通は-99
        accumlate_count = -99;
    }
    //3秒以内で十分なダメージを貰ったら有効なダメージとして計算します。
    if((Gehurtcount() > 0) && (accumlate_count > 0) && (accumlate_count < 180)) {
        //(指定時間以内に指定のダメージに届いたら大ダメージに入ります
        if((accumlate_damage > 60.f)) {
            count_in_big_damage = 60;
            in_big_damage       = true;
        }
        //蓄積ダメージ用
        accumlate_damage += 0.25f;
    }

    //開幕演出終わった時正常モードに入ります
    if(is_attatched) {
        //大ダメージ貰った時
        if(in_big_damage) {
            boss_action = HURT;
            //ダメージ処理
            if(Gehurtcount() > 0) {
                Get_Hurt_in_bigamage();
            }
            else {
                transparency = 1.0;
            }
        }
        //大ダメージ貰って無い時 普通に攻撃する
        if(!in_big_damage) {
            //HPが200以上の時
            if(Hp > 200.0f) {
                Attacking_When_Hp_up_200();
            }
            //HPが200以下の時
            if(Hp < 200.0f) {
                Attacking_When_Hp_down_200();
            }
            //ダメージ処理
            if(Gehurtcount() > 0) {
                Get_Hurt();
            }
            else {
                transparency = 1.0;
            }
        }
    }

    hurt_count--;
    //死ぬ時
    if(Hp < 0.0f) {
        boss_action = DEAD;
    }
    switch(boss_action) {
    case Boss::STAY:
        transparency = 1.0f;
        is_attatched = false;
        //ゴブリンのニメーション設定
        SetAnimation(anim_handle, 0);
        Fixpos(pos);   //位置固定
        ResetAllAttackBool();
        ResetAllCounterAndKind();
        //蓄積カウンター用
        accumlate_count  = 0;
        accumlate_damage = 0.0f;
        break;
    case Boss::ATTACK_START:
        //5秒の攻撃終わったらら正常な攻撃パターンに入る
        if(start_attack_count > 300) {
            is_attatched = true;
        }
        if(start_attack_count < 180) {
            SetAnimation(anim_handle, 1);
            //衝撃波出す
            ball_summon = false;
            on_wave     = true;
        }
        else {
            SetAnimation(anim_handle, 3);
            //ボール攻撃出す
            ball_summon = true;
            on_wave     = false;
        }
        start_attack_count++;
        break;
    case Boss::THINKING:
        //考えます
        SetAnimation(anim_handle, 7);
        Fixpos(pos);   //位置固定
        //HPが200以上の場合
        if(now_is_attacking_start_hp_up200_count > 30) {
            //先に全ての攻撃パターンをリセット
            ResetAllAttackBool();
            //ここで攻撃パターン抽選
            attack_kind_up200_count               = GetRand(9);
            now_is_attacking_start_hp_up200_count = -300;
        }
        //HPが200以下の場合
        if(now_is_attacking_start_hp_down200_count > 30) {
            //先に全ての攻撃パターンをリセット
            ResetAllAttackBool();
            //ここで攻撃パターン抽選
            attack_kind_down200_count               = GetRand(9);
            now_is_attacking_start_hp_down200_count = -300;
        }
        break;
    case Boss::BALL_SUMMON:
        SetAnimation(anim_handle, 3);
        Fixpos(pos);   //位置固定
        //プレーヤーに向いて
        CalculateAngle_boss_to_player(player_pos, pos, Standerd, Boss_to_player);
        //攻撃開始
        ball_summon = true;
        break;
    case Boss::ATTACK_DOWNWARD:
        //特定の範囲内なら攻撃開始
        if((GetDistance(pos, player_pos) < 800.0f)) {
            //プレイヤーに向いて殴る
            SetAnimation(anim_handle, 6);
            //プレーヤーに向いて
            CalculateAngle_boss_to_player(player_pos, pos, Standerd, Boss_to_player);
            is_standing_melee_attack_downward = true;
        }
        else {
            is_standing_melee_attack_downward = false;
            SetAnimation(anim_handle, 4);
            //プレーヤーに向いて
            CalculateAngle_boss_to_player(player_pos, pos, Standerd, Boss_to_player);
            //移動量を沿って動きます
            pos.x -= (float)(BOSS_SPEED * sin(rot.y));
            pos.z -= (float)(BOSS_SPEED * cos(rot.y));
        }
        break;
    case Boss::WAVE_:
        Fixpos(pos);   //位置固定
        //ウェーブ起動
        on_wave = true;
        SetAnimation(anim_handle, 1);
        break;
    case Boss::JUMP_ATTACK:
        //特定の範囲内なら攻撃開始
        if((GetDistance(pos, player_pos) < 800.0f)) {
            //プレイヤーに向いて殴る
            SetAnimation(anim_handle, 5);
            //プレーヤーに向いて走る
            CalculateAngle_boss_to_player(player_pos, pos, Standerd, Boss_to_player);
            is_jump_attack = true;
        }
        else {
            SetAnimation(anim_handle, 4);
            is_jump_attack = false;
            //プレーヤーに向いて走る
            CalculateAngle_boss_to_player(player_pos, pos, Standerd, Boss_to_player);
            //移動量を沿って動きます
            pos.x -= (float)(BOSS_SPEED * sin(rot.y));
            pos.z -= (float)(BOSS_SPEED * cos(rot.y));
        }
        break;
    case Boss::HURT:
        //アニメーション
        SetAnimation(anim_handle, 2);
        //1秒終わったら　普通なモードに戻る
        if(count_in_big_damage < 0) {
            count_in_big_damage = -99;
            in_big_damage       = false;
        }
        count_in_big_damage--;
        //移動量を沿って動きます
        pos.x -= (float)(BACK * sin(hurt_rot));
        pos.z -= (float)(BACK * cos(hurt_rot));
        break;
    case Boss::DEAD:
        SetAnimation(anim_handle, 2);
        ResetAllAttackBool();
        is_attatched = false;
        transparency -= 0.01f;
        if(transparency < 0) {
            Set_is_dead(true);
            transparency = 0.0f;
            pos          = {28.0f * 200.0f, -150000.0f, 0.0f * 200.0f};
        }

        break;
    }

    //左右手の座標更新
    framePosL = MV1GetFramePosition(model_handle, 22);
    framePosR = MV1GetFramePosition(model_handle, 46);

    //プレイヤーY座標＝　(調整量)　+（重力補正）
    if(Get_is_dead()) {
        pos.y += (ADD_GRAVITY * (-1));
    }
    else {
        if(pos.y < 4150.0f) {
            pos.y = 4150.0f;
        }
        else {
            if(pos.y > 4150.0f) {
                pos.y += (ADD_GRAVITY * (-1));
            }
        }
    }

    //アニメーションの処理をする
    PlayAnimation();
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
//描画処理
//---------------
void Boss::Render()
{
    //2D表示計算
    auto screen_position = ConvWorldPosToScreenPos(pos);
    //位置の上の側に表示
    int x = static_cast<int>(screen_position.x);         //X座標設定
    int y = static_cast<int>(screen_position.y) - 100;   //Y座標設定

    SetFontSize(60);
    //攻撃状態の描画設定
    switch(boss_action) {
    case Boss::ATTACK_START:
        SetFontSize(25);
        DrawString(x, y, "家に帰さない！", GetColor(150, 200, 0));
        break;
    case Boss::HURT:
        SetFontSize(25);
        DrawString(x, y, "痛い！", GetColor(150, 200, 0));
        break;
    }

    //もしも接触したら
    if(is_attatched) {
        DrawFillBox((int)(SCREEN_W / 2 - 300),
                    10,
                    static_cast<int>(SCREEN_W / 2 - 300 + Hp),
                    50,
                    GetColor(225, 50, 50));
        DrawLineBox((int)(SCREEN_W / 2 - 300),
                    10,
                    static_cast<int>(SCREEN_W / 2 - 300 + 600),
                    50,
                    GetColor(63, 72, 204));
    }

    SetFontSize(60);

    MV1SetOpacityRate(model_handle, transparency);
    //平常状態の描画設定
    MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
    MV1SetPosition(model_handle, VGet(pos.x, pos.y + SET_Y_ADJEST_VALUE, pos.z));
    MV1DrawModel(model_handle);
}
//終了処理
//---------------
void Boss::Exit()
{
    MV1DeleteModel(model_handle);
}
