#include "../Base.h"
#include <vector>
#pragma once

//! ModelBaseクラス
//===============================================
class ModelBase : public Base
{
protected:
    // メンバ定数
    //===============================================
    static constexpr float JUMP_SPEED        = 70.0f;   //!<飛んでるスピード
    static constexpr float ADD_GRAVITY       = 5.0f;    //!<重力加速度設定
    static constexpr float PLAYER_ROT_SPEED  = 2.5f;    //!<プレイヤーの回転スピード設定
    static constexpr float PLAYER_MOVE_SPEED = 45.0f;   //!<プレイヤーの移動速度設定
    static constexpr float BULLET_SPEED      = 90.0f;   //!< 飛ばす弾丸スピード
    // メンバ変数(ゴブリン
    //===============================================
    static constexpr float GOBLIN_NORMAL_SPEED = 2.0f;    //!< ゴブリンの移動速度
    static constexpr float GOBLIN_DUSH_SPEED   = 20.0f;   //!< ゴブリンの走る速度
    static constexpr float GOBLIN_ROT_SPEED    = 1.0f;    //!< ゴブリンの回転する速度

    // メンバ変数
    //===============================================
    int   model;          //!<モデル用変数
    int   model_handle;   //!<モデルのデータを入れる用の変数
    float target_rot;     //!<目的地に向けた角度

    //アニメ関係の変数
    float playtime = 0.0f;
    int   anim_no;       //!<今のアニメ番号
    float totaltime;     //!<トータル放送時間
    int   attachindex;   //!<今のアニメ番号

    //----------------------------------------------------------
    //!	@name	ブロックと当たり判定用ライン設定
    //----------------------------------------------------------
    //!@{
    VECTOR gravity_line_start;   //!<プレイヤーの重力ラインの上
    VECTOR gravity_line_end;     //!<プレイヤーの重力ラインの下
    VECTOR top_line_start;       //!<プレイヤーの頭上ラインの下
    VECTOR top_line_end;         //!<プレイヤーの頭上ラインの上
    //!@}

    //----------------------------------------------------------
    //!	@name	ブロックと当たり判定用ライン設定
    //----------------------------------------------------------
    //!@{
    //!
    //!ブロックの上との当たり判定
    HITRESULT_LINE block_t1;
    HITRESULT_LINE block_t2;
    //!ブロックの下との当たり判定
    HITRESULT_LINE block_b1;
    HITRESULT_LINE block_b2;
    //!@}

    int    root_node = -1;   //!<モデルのルート用変数
    MATRIX identity;
    float  attackradius;   //!<攻撃される半径の設定

public:
    //!コンストラクタ
    ModelBase();
    //!デストラクタ
    virtual ~ModelBase();

    //! @name  重力ラーイン関係設定
    //!@{
    void   SetGravityLineStart(VECTOR& now_gravity_start, float charatorheight_adjeat);
    VECTOR GetGravityLineStart() const;
    void   SetGravityLineEnd(VECTOR& now_gravity_end, float charatorheight_adjeat);
    VECTOR GetGravityLineEnd() const;
    //!@}

    //! @name  重力ラーイン関係設定
    //!@{

    void   SetTopLineStart(VECTOR& now_top_start, float charatorheight_adjeat);
    VECTOR GetTopLineStart() const;
    void   SetTopLineEnd(VECTOR& now_top_end, float charatorheight_adjeat);
    VECTOR GetTopLineEnd() const;

    //!@}

    //! @name  当たり判定用の関係設定
    //!@{

    HITRESULT_LINE GetBlockT1() const;
    HITRESULT_LINE GetBlockT2() const;

    HITRESULT_LINE GetBlockB1() const;
    HITRESULT_LINE GetBlockB2() const;

    //!上層の床用の判定取得
    bool CheckHitStageFloor() const { return (GetBlockT1().HitFlag == 1) || (GetBlockT2().HitFlag == 1); }
    //!下層の床用
    bool CheckHitStageBottom() const { return (GetBlockB1().HitFlag == 1) || (GetBlockB2().HitFlag == 1); }

    //！当たり判定の点を取ってそれを判定させる
    void CheckIfHitWithBlock(const VECTOR& TA,
                             const VECTOR& TB,
                             const VECTOR& TC,
                             const VECTOR& TD,
                             const VECTOR& BA,
                             const VECTOR& BB,
                             const VECTOR& BC,
                             const VECTOR& BD);
    //!@}

    //! @name  アニメ関係設定
    //!@{
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    //!@}

    //----------------------------------------------------------
    //!	@name	攻撃される半径の設定
    //----------------------------------------------------------
    //!@{
    void  SetAttackRadius(float now_radious);
    float GetAttackRadius() const;
    //!@}
};
