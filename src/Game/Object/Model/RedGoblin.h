#include "ModelBase.h"
#include <vector>
#include <array>
#include <stdio.h>

class MoveBlock;
class BlockBase;
class Player;
class Npc;
class Vector3;
class Camera;

//! キャラクター
//===============================================
class RedGoblin : public ModelBase
{
private:
    enum MOVE_PATTERN
    {
        STAY,
        MOVE,
        ATTACK,
        HURT,
        DEAD,
    };

    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/RedGoblin/Anim_Attack1.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Neutral.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Walk.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Run.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Damage.mv1"),
        MV1LoadModel("Data/Character/RedGoblin/Anim_Down_Loop.mv1"),
    };

   
    //----------------------------------------------------------
    //!	@name	個体番号確認
    //----------------------------------------------------------
    //!@{
    int redgoblin_num;   //!<作成されるムーヴブロックのナンバー
    //!@}
    //----------------------------------------------------------
    //!	@name	モデル移動関係設定
    //----------------------------------------------------------
    //!@{
    MOVE_PATTERN goblin_action;       //!<ゴブリンの移動パターン
    int          move_count;          //!<固定の秒数の後で移動;
    float        fix_move_distance;   //!<固定で移動する距離;
    VECTOR       pos_before_move;     //!<逃げる前の位置
    bool         is_move;             //!<動いでいがどうが
    bool         is_live;             //!<生きるがどうかの確認
    bool         now_move;            //!<個体番号8から10用の1回性トリガー
    //!@}

    //----------------------------------------------------------
    //!	@name	攻撃時の設定
    //----------------------------------------------------------
    //!@{

    float  count = 0;
    VECTOR framePos_R;
    float  right_hand_radius = 30.f;
    bool   is_attacking;           //攻撃中
    bool   haved_hurt_by_player;   //既にプレーヤーに攻撃された
    bool   in_sight = false;       //視界の範囲内
    float  attack_rot;
    float  roty;   //!NPCーに向いて
    //!@}

    //----------------------------------------------------------
    //!	@name	重力関係設定
    //----------------------------------------------------------
    //!@{
    float gravity;      //!<重力
    bool  is_stepped;   //!<踏み台の上に踏んでいるのか
    bool  on_move_block;
    //!@}
    //----------------------------------------------------------
    //!	@name	生死判定
    //----------------------------------------------------------
    //!@{

    float Hp;        //!<モンスターのHP
    bool  is_dead;   //!<死亡しているがしていないが
    //!@}
    //----------------------------------------------------------
    //!	@name	ダメージ関係設定
    //----------------------------------------------------------
    //!@{
    bool  is_hurt;
    float hurt_rot;
    float transparency;   //!<ダメージ用透明度
    int   hurt_count;     //!<殴られた時に起動するとカウンター
    //!@}
    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    const Player* player_ptr = nullptr;   //!<プレイヤーを宣言して
    const Camera* camera_ptr = nullptr;   //!<カメラーを宣言して
    const Npc*    npc_ptr    = nullptr;   //!<NPCーを宣言して
    //!@}


    //追加未整理
   const char* text;   //セリフ用テキスト

public:
    //! コンストラクタ
    RedGoblin(size_t num);
    //! デストラクタ
    virtual ~RedGoblin();

    //! @name   アニメの設定
    //!@{
    //!アニメーションを再生処理を行う用の関数
    void PlayAnimation();
    //!@}

    //! @name　他関数
    //!@{
    //!固定座標を設定
    void Fixpos(VECTOR& pos);
    //!@}

    //! @name 生死判定
    //!@{
    //!殺された時の座標設定
    bool Check_Is_Dead();
    //! is_dead設定します。
    void Set_is_dead(bool now_is_dead);
    //! is_dead取得します。
    //! @return 現在のis_dead
    bool Get_is_dead() const { return is_dead; }

    ////!@}

    //! @name   攻撃時用
    //!@{
    void   Reload();   //!弾丸がリロードする時
    bool   Get_is_attacking() const { return is_attacking; }
    VECTOR GetframePos_R() const { return framePos_R; }
    float  Getright_hand_radius() const { return right_hand_radius; }
    //! attack_rot取得します。
    //! @return 現在のattack_rot
    float GetAttachRot() const { return attack_rot; }
    //!@}

    //! @name ブロックとの当たり判定
    //!@{
    void ResetBeforeCheck();                          //!チェック前に他の変数をリセットして直す
    void GravityLine();                               //!<上下の当たりラインの処理
    bool HitWithStageBlock(const BlockBase* block);   //!<ステージブロックの当たり判定
    bool HitWithMoveBlock(const MoveBlock* block);    //!<ムーヴブロックの当たり判定
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

    //!@name　ダメージ関係設定
    //!@{
    //! transparency設定します。
    void Set_Transparency(float now_transparency);
    //! is_hurt設定します。
    //void Set_is_hurt(bool now_is_hurt);
    //! Hp設定します。
    void Set_Hp(bool now_HP);
    void Reset_tomove();                                //!<攻撃終わった後STAYに戻る
    void SetPos_HurtByPlayerBullet(float bullet_rot);   //!< プレーヤー弾丸に攻撃された時の設定
    //! transparencyを取得します。
    //! @return 現在のtransparency
    float Get_Transparency() const { return transparency; }
    //! is_hurtを取得します。
    //! @return 現在のis_hurt
    bool Get_is_hurt() const { return is_hurt; }
    //! Hpを取得します。
    //! @return 現在のHp
    bool Get_Hp() const { return Hp; }
    //!@}

    //! @name　他に使うクラス設定
    //!@{
    //プレイヤーを宣言して
    void SetPlayer(Player* p) { player_ptr = p; }
    void SetCamera(Camera* c) { camera_ptr = c; }
    void SetNpc(Npc* n) { npc_ptr = n; }
    //!@}

    //! @name  殺された時設定
    //!@{
    //! now_liveを設定します。
    //! @param  [in]    now_live  現在のnow_live
    void SetIsLive(bool now_live);
    //! is_liveを取得します。
    //! @return 現在のis_live
    bool GetIsLive() const;
    //!@}

    //! @name　その他設定
    //!@{
    //ステージに合わせて正常稼働中
    bool MovieORDead();
    //!@}

    //!角度計算用
    void CalculateAngle_red_goblin_to_player(const VECTOR& pos_1,
                                             const VECTOR& pos_2,
                                             const VECTOR& vector1,
                                             const VECTOR& vector2);

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
