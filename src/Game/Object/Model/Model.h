#pragma once
#include "ModelBase.h"
#include <vector>

//! Modelクラス
//===============================================
class Model : public ModelBase
{
private:

    // メンバ変数
    //===============================================

    //!キャラクターのモデル設定
    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/mouse/Falling Into Pool.mv1"),
        MV1LoadModel("Data/Character/mouse/Joyful Jump.mv1"),
        MV1LoadModel("Data/Character/mouse/Falling.mv1"),
        MV1LoadModel("Data/Character/mouse/Running.mv1"),
        MV1LoadModel("Data/Character/mouse/Waving.mv1"),
        MV1LoadModel("Data/Character/mouse/Sitting.mv1"),
        MV1LoadModel("Data/Character/mouse/Sad Idle.mv1"),
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

    bool already_clear;

    int GameOver_Count;
    Model_PATTERN model;   //!<使ってるモデル番号
    float play_time_start;   //!<動画の開始時間
    VECTOR player_model_rot;   //!<プレイヤーのモデル用の角度
    int    scene_num;          //!<今上演している番号

public:

    //!コンストラクタ
    Model();
    //!デストラクタ
    ~Model();

    //----------------------------------------------------------
    //!	@name	アニメの設定
    //----------------------------------------------------------
    //!@{
    void SetAnimation(const std::vector<int>& handle, int new_anim_no);
    void PlayAnimation();
    //!@}
      
    //----------------------------------------------------------
    //!	@name	上演シーン設定
    //----------------------------------------------------------
    //!@{
    void SetNowScene(int now_scene);
    int  GetNowScene() const;
    //!@}

    

    //!更新処理
    void Update(int now_model, bool is_clear);
    //!描画処理
    void Render(int count);
    //!終了処理
    void Exit();
};
