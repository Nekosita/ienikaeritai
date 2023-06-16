#pragma once
#include "ModelBase.h"
#include "../src/System/Effect.h"

class Player;
class Boss;
class Npc;

//! コンストラクタ
//-----------------------------------------------
class TrapBall : public ModelBase
{
private:
    enum MOVE_PATTERN
    {
        STAY,
        ATTACK,
    };

    //----------------------------------------------------------
    //!	@name	個体番号確認
    //----------------------------------------------------------
    //!@{
    int ball_num;
    //!@}
    //----------------------------------------------------------
    //!	@name	モデル関係設定
    //----------------------------------------------------------
    //!@{
    MOVE_PATTERN ball_action;     //!攻撃パターン
    VECTOR       speed;           //!ボールのスピード
    int          now_attacking;   //攻撃する時使ったカウンター
    //!@}
    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    const Player* player_ptr = nullptr;   //!<プレイヤーを宣言して
    const Boss*   boss_ptr   = nullptr;   //!<プレイヤーを宣言して
    //!@}

public:

    //! コンストラクタ
    TrapBall(size_t ball_num);
    //! デストラクタ
    ~TrapBall();

    //! @name　他キャラとの当たり判定
    //!@{
    bool CheckHitWithPlayer();
    //!@}

    //! @name　他に使うクラス設定
    //!@{
    //プレイヤーを宣言して
    void SetPlayer(Player* p) { player_ptr = p; }
    void SetBoss(Boss* p) { boss_ptr = p; }
    //!@}

    
    //! @name　その他設定
    //!@{
    //!NPCのステータスリセット
    void Trap_BallReset();
    //!@}
   

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
