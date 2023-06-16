#include "Camera.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Model/Boss.h"

// カメラの当たり判定サイズ
#define COLLISION_SIZE (15.0f)

// カメラの角度変更速度
#define ANGLE_SPEED (3.0f)

// カメラとプレイヤーの距離
#define PLAYER_DISTANCE (3500.0f)

// カメラが注視点とするプレイヤーの座標のＹ座標に足す値
#define PLAYER_OFFSET_Y (400.0f)

// カメラの画角
#define CAMERA_FOV (40.0f)

// カメラの座標を目標座標に近づける計算で除算に使用する値
#define MOVE_DIV_NUM (0.17f)

// カメラの回転速度
#define CAMERA_ANGLE_SPEED 0.05f

// カメラの手前クリップ距離
#define CAMERA_NEAR_Z (250.0f)

// カメラの奥クリップ距離
#define CAMERA_FAR_Z (400 * 200.0f)

//カメラのコンストラクタ設定
//---------------------------------------------------------------------------------
Camera::Camera()
{
    //カメラ座標の初期化
    lookatpos = VGet(8.0f * 200.0f, 5000.0f, 28.0f * 200.0f);     //カメラの見る座標
    pos       = {-200.0f * 200.0f, 20000.0f, -200.0f * 200.0f};   //カメラ座標

    now_camera = Camera::GAME_TITLE;   //一番最初のカメラのバージョン

    oa_count   = 605;
    yobi_count = 60;
    ea_count   = 95;

    // カメラの向きを初期化
    CameraVAngle = 0.0f;
}
//カメラのデストラクタ設定
//---------------------------------
Camera::~Camera()
{
    Exit();
}
//カメラタイプの設定と取得の関数
//---------------------------------
void Camera::SetNowCamera(int camera_type_num)
{
    camera_type = camera_type_num;
}
int Camera::GetNowCamera() const
{
    return camera_type;
}
//カメラカウンターの設定と取得の関数
//---------------------------------
void Camera::SetOpenAnimeCount(int now_oa_count)
{
    oa_count = now_oa_count;
}
int Camera::GetOpenAnimeCount() const
{
    return oa_count;
}
//更新処理
//-------------------------------
void Camera::Update()
{
    //もしもnullならその先の処理をしない(プレイヤーのデータがないなら前に行っても意味ない
    if(player_ptr == nullptr)
        return;
    const VECTOR& player_pos      = player_ptr->GetPos();
    const VECTOR& player_rot      = player_ptr->GetRot();
    int           now_camera_type = GetNowCamera();

    camera_type = now_camera_type;

    //プレイヤーの今まで状態に応じてカメラモードを変更する
    switch(camera_type) {
    case Camera::GAME_TITLE:
        lookatpos = VGet(0.0f * 200.0f, -10000.0f, 0.0f * 200.0f);
        pos       = {-40.0f * 200.0f, -8500.0f, -10.0f * 200.0f};
        break;
    case Camera::OPENING_ANIME:
        //前半４秒はステージの全体図
        if(oa_count > 0) {
            camera_type = OPENING_ANIME;
        }
        if(((GetMouseInput() & MOUSE_INPUT_LEFT) == 1) || (oa_count < 0)) {
            camera_type = TPV;
        }
        //最初はBOSSの所に設置
        if(oa_count > 599) {
            lookatpos = VGet(270.0f * 200.0f, 5000.0f, 0.0f * 200.0f);
            pos       = {230.0f * 200.0f, 8500.0f, -60.0f * 200.0f};
        }
        //途中はゴブリンと全体ステージ
        if((oa_count > 420) && (oa_count < 599)) {
            OpeningAnimationDir_lookatpos = {-200.0f, 0.0f, 0.0f};
            OpeningAnimationDir_camerapos = {-200.0f, 0.0f, 0.0f};
        }
        //ここで赤いゴブリンの所に切り替え
        if(oa_count == 420) {
            pos       = {40.0f * 200.0f, 3300.0f, 10.0f * 200.0f};
            lookatpos = VGet(70.0f * 200.0f, 3500.0f, 10.0f * 200.0f);
            OpeningAnimationDir_camerapos.x = (GOAL_POS_IN_OPENING_MOVIE.x - pos.x) /180;
            OpeningAnimationDir_camerapos.y = (GOAL_POS_IN_OPENING_MOVIE.y - pos.y) /180;
            OpeningAnimationDir_camerapos.z = (GOAL_POS_IN_OPENING_MOVIE.z - pos.z) /180;
            OpeningAnimationDir_lookatpos   = {0.0f, 0.0f, 0.0f};
        }
        //後半はプレイヤーとNPCが話す所
        if(oa_count < 240) {
            OpeningAnimationDir_lookatpos = {0.0f, 0.0f, 0.0f};
            OpeningAnimationDir_camerapos = {0.0f, 0.0f, 0.0f};
            lookatpos                     = VGet(-15. * 200.0f, 1500.0f, 8.5f * 200.0f);
            pos                           = {-30.0f * 200.0f, 2500.0f, 8.5f * 200.0f};
        }
        oa_count--;

        //ここでvectorに合わせて計算
        lookatpos = VAdd(OpeningAnimationDir_lookatpos, lookatpos);
        pos       = VAdd(OpeningAnimationDir_camerapos, pos);
        break;
    case Camera::TPV:
        oa_count = -99;

        //マウス移動量の確認
        Rot = GetMouseWheelRotVol();

        if(Rot > 0) {
            CameraVAngle += CAMERA_ANGLE_SPEED;
            if(CameraVAngle >= 1.0f) {
                CameraVAngle = 1.0f;
            }
        }
        if(Rot < 0) {
            CameraVAngle -= CAMERA_ANGLE_SPEED;
            if(CameraVAngle <= 0.0f) {
                CameraVAngle = 0.0f;
            }
        }

        VECTOR TempPosition2;

        lookatpos = player_pos;
        lookatpos.y += PLAYER_OFFSET_Y;

        TempPosition2.x = -(PLAYER_DISTANCE / 2 + sinf(CameraVAngle) * PLAYER_DISTANCE) *
                          sinf(player_rot.y + TO_RADIAN(180.0f));
        TempPosition2.y = sinf(CameraVAngle) * PLAYER_DISTANCE;
        TempPosition2.z = -(PLAYER_DISTANCE / 2 + sinf(CameraVAngle) * PLAYER_DISTANCE) *
                          cosf(player_rot.y + TO_RADIAN(180.0f));

        //もしもボスが死んだら揺ぎ始め
        if(boss_ptr->Get_is_dead() || (player_ptr->Getpart_count() > 0)) {
            shaking_count++;
            if((shaking_count % 31) < 20) {
                int x, y, z;
                x               = GetRand(5);
                y               = GetRand(5);
                z               = GetRand(5);
                TempPosition2.x = -(PLAYER_DISTANCE / 2 + sinf(CameraVAngle) * PLAYER_DISTANCE) *
                                  sinf(player_rot.y + TO_RADIAN(177.0f + x * 1.f));
                TempPosition2.y = sinf(CameraVAngle - 0.01f * y) * PLAYER_DISTANCE;
                TempPosition2.z = -(PLAYER_DISTANCE / 2 + sinf(CameraVAngle) * PLAYER_DISTANCE) *
                                  cosf(player_rot.y + TO_RADIAN(177.0f + z * 1.f));
            }
        }

       // 算出した座標に注視点の位置を加算したものがカメラの位置
        pos = VAdd(TempPosition2, lookatpos);

        break;
    case Camera::GAME_CLEAR_MOVIE:
        lookatpos = VGet(280.0f * 200.0f, 4200.0f, 10.0f * 200.0f);
        shaking_count++;
        if((shaking_count % 31) < 16) {
            int x, y, z;
            x   = GetRand(3);
            y   = GetRand(3);
            z   = GetRand(3);
            pos = VGet(298.0f * 200.0f + (x * 200), 4800.0f + (y * 100), 8.0f * 200.0f + (z * 200));
        }
        ea_count--;
        break;
    case Camera::GAME_CLEAR:
        lookatpos = VGet(0.0f, SCREEN_H / 2, 0.0f);
        pos       = {0.0f, SCREEN_H / 2 + 100.0f, -600.0f};
        break;
    case Camera::GAME_OVER:
        lookatpos = VGet(0.0f, SCREEN_H / 2, 0.0f);
        pos       = {0.0f, SCREEN_H / 2 + 100.0f, -600.0f};
        break;
    }
}

//カメラ描画
//-------------------------------------------
void Camera::Render()
{
    SetupCamera_Perspective(CAMERA_FOV * DX_PI_F / 180.0f);
    SetCameraNearFar(CAMERA_NEAR_Z, CAMERA_FAR_Z);
    SetCameraPositionAndTarget_UpVecY(pos, lookatpos);
}
int Camera::Getyobi_count() const
{
    return yobi_count;
}
void Camera::SetEndAnimeCount(int now_ea_count)
{
    ea_count = now_ea_count;
}
int Camera::GetEndAnimeCount() const
{
    return ea_count;
}
void Camera::Reset_count()
{
    oa_count    = 605;
    ea_count    = 95;
    camera_type = OPENING_ANIME;
    // カメラの向きを初期化
    CameraVAngle = 0.25f;
}
void Camera::Exit()
{
}
