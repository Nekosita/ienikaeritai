#pragma once
#include "ModelBase.h"
#include <vector>

//! Modelクラス
//===============================================
class Model_2 : public ModelBase
{
private:

    // メンバ変数
    //===============================================

    //!キャラクターのモデル設定
    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/mouse/Sitting Talking.mv1"),
    };

    //!カメラ視点の設定
    enum Model_PATTERN
    {
        GAME_TITLE,
        GAME_STAGE,
        GAME_CLEAR,
        GAME_OVER,
        NOTHING,
    };

    //----------------------------------------------------------
    //!	@name	他モデル
    //----------------------------------------------------------
    //!@{
    int chair[2];
    int coffee[2];
    int table;
    VECTOR chair_pos[2];
    VECTOR coffee_pos[2];
    VECTOR table_pos;
    //!@}
    
    int Clear_count;

    Model_PATTERN model;   //!<使ってるモデル番号
    float play_time_start;   //!<動画の開始時間
    VECTOR player_model_rot;   //!<プレイヤーのモデル用の角度

public:

    //!コンストラクタ
    Model_2();
    //!デストラクタ
    ~Model_2();

    //----------------------------------------------------------
    //!	@name	アニメの設定
    //----------------------------------------------------------
    //!@{
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    void PlayAnimation();
    //!@}
      


    void Reset();

    //!更新処理
    void Update(int now_model);
    //!描画処理
    void Render(int count);
    //!終了処理
    void Exit();
};
