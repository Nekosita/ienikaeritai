#pragma once
#include "ModelBase.h"
#include "../src/System/Effect.h"

class Player;
class Boss;
class Npc;

//! コンストラクタ
//-----------------------------------------------
class ShokeWave : public ModelBase
{
private:
    enum MOVE_PATTERN
    {
        STAY,
        ATTACK,
    };


    //----------------------------------------------------------
    //!	@name	モデル関係設定
    //----------------------------------------------------------
    //!@{
    MOVE_PATTERN wave_action;     //!攻撃パターン
    int          now_attacking;   //攻撃する時使ったカウンター
    //!@}
    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    const Player* player_ptr = nullptr;   //!<プレイヤーを宣言して
    const Boss*   boss_ptr   = nullptr;   //!<プレイヤーを宣言して
    //!@}

    //追加未整理

    float wave_radius;

public:

    //! コンストラクタ
    ShokeWave();
    //! デストラクタ
    ~ShokeWave();


    //! @name　他に使うクラス設定
    //!@{
    //プレイヤーを宣言して
    void SetPlayer(Player* p) { player_ptr = p; }
    void SetBoss(Boss* p) { boss_ptr = p; }
    //!@}
   
    //! @name　その他設定
    //!@{
    //!NPCのステータスリセット
    void ShokeWaveReset();
    //!@}
   
   float Get_wave() const { return wave_radius; }
    

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
