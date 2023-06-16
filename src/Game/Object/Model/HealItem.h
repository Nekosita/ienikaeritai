#include "ModelBase.h"
#pragma once

class Player;

//! Itemクラス
//===============================================
class HealItem : public ModelBase
{
private:

    enum ITEN_PATTERN
    {
        NORMAL,
        PICKED,
    };

    // メンバ変数
    //===============================================

    ITEN_PATTERN now_heal_item;   //!<今のアイテムの状態
    int          vertex_shader;   //!<シェーダー設定
    int          pixel_shader;    //!<ピクセル設定
    float        move_count;      //!<ふわふわ演出用
    bool         is_picked;       //!<拾うようの変数
    float        radius;          //!<拾う用半径


    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    const Player* player_ptr = nullptr;   //!<プレイヤーを宣言して
    //!@}

public:
    // メンバ関数
    //===============================================

    //!コンストラクタ
    HealItem(size_t item_num);
    //!デストラクタ
    ~HealItem();

    //!拾う変数の設定
    void SetIsPicked(bool now_picked);
    //!拾う変数の獲得
    bool GetIsPicked() const;

    //! @name　他キャラとの当たり判定
    //!@{
    void SetPlayer(Player* p) { player_ptr = p; }
    //bool CheckHitWithPlayer();
    //!@}

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
