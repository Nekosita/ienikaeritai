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
class Boss;
class ShokeWave;

//! Playerクラス
//===============================================
class Player : public ModelBase
{
private:
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
    enum PLAYER_STATUS
    {
        START_MOVIE,
        COMMON,
        HURT,
        GAME_CLEAR_MOVIE,
        CLEAR,
        SHOOT,
    };

    //!キャラクターのモデル設定
    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/Mousey/Idle.mv1"),
        MV1LoadModel("Data/Character/Mousey/Slow Run.mv1"),
        MV1LoadModel("Data/Character/Mousey/Running Backward.mv1"),
        MV1LoadModel("Data/Character/Mousey/Throw.mv1"),
        MV1LoadModel("Data/Character/Mousey/Jazz Dancing.mv1"),
        MV1LoadModel("Data/Character/Mousey/Waving.mv1"),
        MV1LoadModel("Data/Character/Mousey/Talking.mv1"),
        MV1LoadModel("Data/Character/Mousey/Falling.mv1"),
        MV1LoadModel("Data/Character/Mousey/Pistol Kneeling Idle.mv1"),
        MV1LoadModel("Data/Character/Mousey/Shoot Rifle.mv1"),
    };

    //----------------------------------------------------------
    //!	@name	モデル用設定
    //----------------------------------------------------------
    //!@{
    PLAYER_STATUS now_player;   //!<プレイヤの現状
    bool          player_is_boring;
    int           boring_count;
    VECTOR        player_model_rot;   //!<プレイヤーのモデル用の角度
    float         angle       = 0;
    VECTOR        move_vector = {0,0,0};
    //!@}
    //----------------------------------------------------------
    //!	@name	JUMP用設定とブロックの当たり判定
    //----------------------------------------------------------
    //!@{

    bool  is_jump;           //!<JUMPするがどうが確認の変数
    bool  on_ground;         //!<今は地面に接触しでいるがどうか
    bool  now_fall;          //!<落下
    float mov_y;             //!<JUMP距離用の変数
    float gravity;           //!<重力
    bool  is_run;            //!<走り用の変数
    float stand_by;          //!<待機用のロット
    bool  is_stepped;        //!<踏み台の上に踏んでいるのか
    bool  is_collided;       //!<踏み台の下にぶつかって行くがどうか
    bool  to_hint_bar;       //!<ヒントバーにあげる関数
    bool  is_goal_actived;   //!<ゴールバーの上に立っているがいないが
    bool  on_move_block;
    //!@}

    //----------------------------------------------------------
    //!	@name	プレイヤーがダメージを貰った時の設定
    //----------------------------------------------------------
    //!@{
    VECTOR knocked_off_variation;   //!<ぶつけられた時の変数
    int    move_count;              //!<移動用のカウンター
    int    flame_count;             //!<点滅カウンター
    int    jump_count;              //!<ジャンプカウンター
    int    hurt_count;              //!<弾丸に殴られた時
    float  hurt_rot;     //!<ゴブリンに攻撃された時の角度
    int    wave_count;   //衝撃波用
    int    ball_count;   //ボール用
    float  transparency;//!ダメージ貰った時の透明度
    //!@}

    //----------------------------------------------------------
    //!	@name	弾丸関係設定
    //----------------------------------------------------------
    //!@{

    int    count         = 0;
    float  bullet_radius = 50.0f;
    VECTOR bullet_pos;
    bool   is_shoot;//打っているがどうが
    float  bullet_rot;
    VECTOR bullet_dead_pos = {0.0f, 0.0f, 0.0f};
    int    rigor           = 0;   //！弾丸の硬直
    //!@}

    //----------------------------------------------------------
    //!	@name	開幕演出用
    //----------------------------------------------------------
    //!@{
    float openmovieupbar_y;     //映画用の横バー（上
    float openmoviedownbar_y;   //映画用の横バー（下
    //!@}
    //----------------------------------------------------------
    //!	@name	HPゲージ使う関係設定
    //----------------------------------------------------------
    //!@{
    int   Hp_handle;
    int   Hp_framework;
    int   Hp_handle_dying;
    float Hp;
    bool  is_dead=false;//死亡確認
    //!@}

    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    const Camera*   comera_ptr = nullptr;
    const GoalArea* goal_ptr   = nullptr;
    const Boss*     boss_ptr   = nullptr;
    const ShokeWave* shoke_wave_ptr = nullptr;
    //!@}

    //----------------------------------------------------------
    //!	@name　その他
    //----------------------------------------------------------
    //!@{
    bool npc_pause;   //NPCに指示を出す
    //!@}
    int part_count = -99;
    int zonbi_count=-99;
    //マウスの座標設定
    int click_mouse_x, click_mouse_y;   //押した時だけ貰った値
    int now_mouse_x, now_mouse_y;       //いつも更新される値

    float player_hight=125.0f;//プレイヤーの身長

    int count_after_bossdead = 150;

public:
    //! コンストラクタ
    Player();
    //! デストラクタ
    ~Player();

    //! @name  アニメとモデルの設定
    //!@{
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    //アニメーションを再生処理を行う用の関数
    void PlayAnimation();
    //!待機用模型のリセット
    void ResetPlayerIsBoring();
    //!プレイヤーの待機モーションを再生します
    void CheckPlayerIsBoring();
    //! player_model_rotを取得します。
    //! @return 現在のplayer_model_rot
    VECTOR GetPlayerModelRot() const { return player_model_rot; }
    //!@}



    //! @name   Jumpと重力関係処理
    //!@{
    //!全ての飛ぶ処理のまとめ
    void JumpProcess(bool now_is_collided);
    //!飛ぶ事ができるがを判断する関数
    void IsJump();
    void JumpReset();
    //!Jump処理_地面
    void JumpResetGround();
    //!頭がぶつかったときの動作
    void JumpHit();
    //!ブロック上のジャンプ
    void JumpOnTheBlock();
    //!地面からのジャンプ
    void JumpOnTheGround();
    //!ジャンプの時のプレイヤーの重力(抵抗力)処理
    void OnResistance();
    void GravityInJump();
    //!下から上にぶつかった時の高さ設定
    void SetTopWhenHit(float block_hight);
    //!上から下に落ちた時の下の設定
    void SetDownWhenHit(float block_hight);
    //!上昇中かどうがを判断
    bool IsJumpUp();
    //!上下の当たりラインの処理
    void GravityLine();
    //!プレイヤーのYの位置再設定
    void SetPosY(float fix_y);
    //!飛ぶ処理用
    bool GetIsJump() const { return is_jump; }
    //!ムーヴブロックの上に立つ時のベクター設定
    void SetPosOnMoveBlock(const Vector3& dir);
    //!プレイヤーとステージブロックの当たり判定
    bool HitWithStageBlock(const BlockBase* block);
    //!プレイヤーとスムーヴブロックの当たり判定
    bool HitWithMoveBlock(const MoveBlock* block);
    bool GetOnMoveBlock() const { return on_move_block; }
    //! now_is_steppedを設定します。
    //! @param  [in]    now_is_stepped  現在のnow_is_stepped
    void SetIsStepped(bool now_is_stepped);
    //! is_steppedを取得します。
    //! @return 現在のis_stepped
    bool GetIsStepped() const { return is_stepped; }
    //! now_is_collidedを設定します。
    //! @param  [in]    now_is_collided  現在のnow_is_collided
    void SetIsCollided(bool now_is_collided);
    //! is_collidedを取得します。
    //! @return 現在のis_collided
    bool GetIsCollided() const { return is_collided; }
    //!@}
    
    //! @name プレイヤーとゴールブロックの当たり判定
    //!@{
    void ResetBeforeCheck();
    //bool CheckHitGoalBar(bool is_on_goal, float hight);
    //bool GetGoalActived() const;
    void SetToHintBar(bool now);
    bool GetToHintBar() const { return to_hint_bar; }
    //!@}




    //! @name 移動処理全般
    //!@{
    void Move();
    //!プレイヤーが走る処理を入る前の設定
    void SetBeforeRun();
    //!右
    void SetRight();
    //!左
    void SetLeft();
    //!前
    void SetFront();
    //!後
    void SetBack();
    //!移動中でもない場合は角度に合わせてプレイヤーの角度をセット
    void ResetPlayerPosition();
    bool GetIsRun() const { return is_run; }
    //!@}


    //! @name   弾丸用
    //!@{
    void   ShootProcess();
    void   SetIsShoot(bool is_shoot);
    void   SetBulletPos(const VECTOR& now_pos);
    VECTOR GetBulletPos() const { return bullet_pos; }
    float  GetBulletRot() const { return bullet_rot; }
    float  GetBulletRadius() const { return bullet_radius; }
    bool   GetIsShoot() const { return is_shoot; }
    void   Reload();
    //!@}

     //! @name HP関係の設定
    //!@{
    float  Heal();
    bool  Check_Is_hurt();
    void  GetHurt();
    void ResetCount(float bullet_rot);
    bool CheckWave(float addjust_y);
    void GetHurt_wave();
    void GetHurt_ball();
    void SetPosWithWave(const VECTOR& now_pos, const Vector3& dir);
    void SetHurtByBall();
    float GetHp() const { return Hp; }
    bool  Get_is_dead() const { return is_dead; }
    //!@}
   
    //! @name　他に使うクラス設定
    //!@{
    //プレイヤーを宣言して
    void  SetCamera(Camera* c) { comera_ptr = c; }
    void  SetGoalArea(GoalArea* g) { goal_ptr = g; }
    void  SetBoss(Boss* b) { boss_ptr = b; }
    void  SetShokeWave(ShokeWave* w) { shoke_wave_ptr = w; }
    //!@}
   
    //! @name　その他設定
    //!@{
    //開幕演出あるいはクリア演出の中を確認
    bool Check_MovieNowShowing();
    //!プレイヤーを取得
    static Player* GetPlayer();
    //!プレイヤーのステータスリセット
    void PlayerReset();
    //!プレイヤを現地で止まさせること
    bool SetNpcPause();
    bool Getnpc_pause() const { return npc_pause; }
    //!@}

    //追加未整理
    bool CheckHitWithBossHands(float addjust_y);
    bool CheckHitWithBossRightHands(float addjust_y);
    void GetHurt_BossPart();
    int  Getpart_count() const { return part_count; }

    float Getplayer_hight() const { return player_hight; }
    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
