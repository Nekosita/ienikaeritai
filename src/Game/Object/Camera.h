#include "Base.h"

class Player;
class Boss;
#pragma once

//! カメラのベースクラス
//===============================================
class Camera : public Base
{
    //!カメラのパターンの設定
    enum CAMERA_PATTERN
    {
        OPENING_ANIME,
        GAME_TITLE,
        TPV,
        GAME_CLEAR,
        GAME_CLEAR_MOVIE,
        GAME_OVER,
        NOTHING
    };

    int            camera_type;   //!<今使っていたカメラの番号
    CAMERA_PATTERN now_camera;    //!<今使われたカメラのパターンの設定

    VECTOR lookatpos;             //カメラが見る座標
    VECTOR OpeningAnimationDir_lookatpos;   //!lookpos用のVECTOR
    VECTOR OpeningAnimationDir_camerapos;   //!camerapos用のVECTOR
    float scale = 1.0f;   //!<カメラの遠近調整

    int           yobi_count;             //!<予備カウンター
    int           oa_count;               //!<OPENING_ANIME用カメラカウンター設定
    int           ea_count;               //!<ENDING_ANIME用カメラカウンター設定
    int           shaking_count;          //!<カメラ揺らす時用のカウンター
    const Player* player_ptr = nullptr;   //!<プレイヤーを宣言して
    const Boss*   boss_ptr   = nullptr;   //!<ボスキャラを宣言して

    //カメラの角度
    float CameraVAngle = 0.0f;
    int Rot = 0;//マウスの移動量
    VECTOR GOAL_POS_IN_OPENING_MOVIE = {-20.0f * 200.0f, 2500.f, 8.5f * 200.0f};
 


public:
    //!カメラのコンストラクタ
    Camera();
    //!カメラのデストラクタ
    ~Camera();

    void SetPlayer(Player* p) { player_ptr = p; }
    void SetBoss(Boss* b) { boss_ptr = b; }

    float GetCameraVAngle() const { return CameraVAngle; }

    //! @name 上演シーン設定
    //!@{
    void SetNowCamera(int now_camera_nom);
    int  GetNowCamera() const;
    //!@}

    //! @name oa_count設定
    //!@{
    void SetOpenAnimeCount(int now_oa_count);
    int  GetOpenAnimeCount() const;
    //!@}

    int Getyobi_count() const;

    //! @name ea_count設定
    //!@{
    void SetEndAnimeCount(int now_ea_count);
    int  GetEndAnimeCount() const;
    //!@}

    //追加未整理

    void Reset_count();

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
