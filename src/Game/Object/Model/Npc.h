#pragma once
#include "ModelBase.h"
#include <vector>
#include <array>
#include <string>

class Base;
class BlockBase;
class MoveBlock;
class Vector3;
class Camera;
class GoalArea;
class Player;
class Boss;
class Npc_Sheltter;
class CkeckArea;

//! Playerクラス
//===============================================
class Npc : public ModelBase
{
private:
    static constexpr float NPC_SPEED = 42.0f;   //!<走るスピード

    // メンバ変数
    //===============================================
    //!フィーリングバーの状態設定
    enum STATUS_PATTERN
    {
        SLOW,
        DAMAGE,
        GET,
        NONE,
    };

    //!プレイヤーの状態設定
    enum NPC_STATUS
    {
        START_MOVIE,
        COMMON,
        HURT,
        AFTERBOSSDEAD,
        GAME_CLEAR_MOVIE,
        BE_CATCH,
        IN_SHELTTER,
        CLEAR,
    };

    //!キャラクターのモデル設定
    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/mouse/Neutral.mv1"),
        MV1LoadModel("Data/Character/mouse/Running.mv1"),
        MV1LoadModel("Data/Character/mouse/Jumping.mv1"),
        MV1LoadModel("Data/Character/mouse/Salsa Dancing.mv1"),
        MV1LoadModel("Data/Character/mouse/Falling Into Pool.mv1"),
        MV1LoadModel("Data/Character/mouse/Waving.mv1"),
        MV1LoadModel("Data/Character/mouse/Talking.mv1"),
    };

    //----------------------------------------------------------
    //!	@name	移動用設定
    //----------------------------------------------------------
    //!@{

    bool is_stay;
    bool is_stay_by_player;   //プレイヤーが止まるが止まらない
    //ボス戦用トリガー
    bool   after_boss_dead;
    VECTOR player_model_rot;   //!<プレイヤーのモデル用の角度
    //!@}

    //----------------------------------------------------------
    //!	@name	JUMP用設定
    //----------------------------------------------------------
    //!@{
    NPC_STATUS now_npc;    //!<NPCのステータス
    bool       is_run;     //!<走り用の変数
    float      stand_by;   //!<待機用のロット

    //!@}

    //----------------------------------------------------------
    //!	@name	重力関係設定
    //----------------------------------------------------------
    //!@{
    float gravity;      //!<重力
    bool  is_stepped;   //!<踏み台の上に踏んでいるのか
    bool  on_move_block;
    bool  is_goal_actived;   //!<ゴールバーの上に立っているがいないが
    //!@}

    //----------------------------------------------------------
    //!	@name	生死判定
    //----------------------------------------------------------
    //!@{
    float Hp;
    //ダメージ用のカウンター
    int hurt_count;
    int wave_count;
    int ball_count;
    bool is_dead = false;   //死亡確認
    //!@}

    //----------------------------------------------------------
    //!	@name	HPゲージ描画使う関係設定
    //----------------------------------------------------------
    //!@{
    int   Hp_handle;
    int   Hp_framework;
    int   hurt;
    int   stay;
    int   fallowing[2];
    int   be_catch_picture;   //捕まれた時
    int   frame;
    int   running_count;        //小人が追撃した時のカウンター設定
    float hurt_little_people;   //ダメージ貰った時の小人の設定
    //!@}

    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    const Player*   player_ptr = nullptr;   //!<プレイヤーを宣言して
    const Camera*   comera_ptr = nullptr;   //!<カメラーを宣言して
    const Boss*     boss_ptr   = nullptr;   //!<NPCーを宣言して
    const GoalArea* goal_ptr   = nullptr;
    const CkeckArea* check_ptr   = nullptr;
    const Npc_Sheltter* sheltter_ptr = nullptr;
    //!@}

    //追加未整理
    bool be_catch;

    //暫定の死亡ポイント
    //暫定用
    VECTOR TEST_POS= {15.0f * 200.0f, 1750.f, 50.0f * 200.0f};   //暫定の消失ポイント
    bool in_the_dead_point;//強制死亡ポイントの中に入れるがどうが

    //!シェルターの中にいます
    bool in_the_sheltter=false;   
    int count_after_bossdead=90;

public:
    //! コンストラクタ
    Npc();
    //! デストラクタ
    ~Npc();

    //! @name  アニメと移動処理の設定
    //!@{
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    //アニメーションを再生処理を行う用の関数
    void PlayAnimation();
    void Move();
    void Move_AfterBossDead();

    VECTOR GetPlayerModelRot() const { return player_model_rot; }
    //!@}

    //! @name    HP関係の設定
    //!@{
    float  Heal();
    bool Check_Is_hurt();
    void ResetCount();
    bool GetHurt_bullet();
    //! Hpを取得します。
    //! @return 現在のHp
    float GetHp() const { return Hp; }
    bool  Get_is_dead() const { return is_dead; }
    //!@}

    ////!@}
    //! @name ブロックとの当たり判定
    //!@{
    void ResetBeforeCheck();                          //!チェック前に他の変数をリセットして直す
    void GravityLine();                               //!<上下の当たりラインの処理
    bool HitWithStageBlock(const BlockBase* block);   //!<ステージブロックの当たり判定
    bool HitWithMoveBlock(const MoveBlock* block);    //!<ムーヴブロックの当たり判定
    bool CheckHitGoalBar(bool is_on_goal, float hight);
    void SetDownWhenHit(float block_hight);   //!<ステージブロックと当たった時ブロックの高さに合わせての設定
    void SetPosY(float fix_y);   //!<高さの設定
    void SetPosOnMoveBlock(const Vector3& dir);   //!<ムーヴブロックと当たった時ブロックの高さに合わせての設定
    //! is_stepped設定します。
    void SetIsStepped(bool now_is_stepped);
    //! is_steppedを取得します。
    //! @return 現在のis_stepped
    bool GetIsStepped() const { return is_stepped; }
    //! on_move_blockを取得します。
    //! @return 現在のon_move_block
    bool GetOnMoveBlock() const { return on_move_block; }
    void Gravity();   //!<重力設定
    //!@}

    //! @name　他に使うクラス設定
    //!@{
    //プレイヤーを宣言して
    void SetGoal(GoalArea* g) { goal_ptr = g; }
    void SetPlayer(Player* p) { player_ptr = p; }
    void SetBoss(Boss* p) { boss_ptr = p; }
    void SetCamera(Camera* c) { comera_ptr = c; }
    void SetCkeckArea(CkeckArea* c) { check_ptr = c; }
    void Set_Sheltter(Npc_Sheltter* s) { sheltter_ptr = s; }
    //!@}

    void ReleaseFromZonbie();
    bool Getbe_catch() const { return be_catch; }
    void SetPosBeCatch(const Vector3& Hand_Pos);
    bool CheckIsInDeadPoint();
    bool Getin_the_sheltter() const { return in_the_sheltter; }

    //! @name　その他設定
    //!@{
    //開幕演出あるいはクリア演出の中を確認
    bool Check_MovieNowShowing();
    //!NPCのステータスリセット
    void NpcReset();
    //!@}


    //!更新処理
    void Update();
    //!描画処理
    void Render(bool shadow);
    //!終了処理
    void Exit();
};
