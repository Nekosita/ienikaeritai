#include "../Game.h"
#include "../Main.h"
#include "SceneRule.h"
#include "../src/System/KeyInput.h"

namespace
{
int game_rule_handle;   //!<選択用の変数

int mouseX, mouseY;

//TITLE判定用
int    title_handle;
int    title_handle_2;
VECTOR title_centerpos;
int    title_weight;
int    title_height;
VECTOR title_startpos;
VECTOR title_endpos;
}   // namespace

void InitGameRule()
{
    game_rule_handle = LoadGraph("Data/back.jpg");
    title_centerpos.x = 1400;
    title_centerpos.y = 800;
    title_handle      = LoadGraph("Data/Icon/title.png");
    title_handle_2    = LoadGraph("Data/Icon/title_2.png");
}

GAME_SCREEN UpdateGameRule()
{
    //GAME_RULEの左上座標
    title_startpos.x = title_centerpos.x - title_weight / 2;
    title_startpos.y = title_centerpos.y - title_height / 2;
    //GAME_RULEの右下座標
    title_endpos.x = title_centerpos.x + title_weight / 2;
    title_endpos.y = title_centerpos.y + title_height / 2;
 
    //マウス座標と画像座標用の変数を入れること
    GetMousePoint(&mouseX, &mouseY);
    GetGraphSize(title_handle, &title_weight, &title_height);


    //もしもGAME_RULE範囲の中であれば
    if(CheckInSquare(title_startpos, title_endpos, mouseX, mouseY)) {
        //マウスを押したらシーン変えます
        if(((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)) {
            if(CheckMusic() == 0) {
                PlaySoundFile("Data/music/魔王魂 効果音 システム49.mp3", DX_PLAYTYPE_BACK);
            }
            return GAME_SCREEN::GAME_TITLE;
        }
    }
    return GAME_SCREEN::GAME_RULE;
}

void RenderGameRule()
{
    DrawRotaGraph(SCREEN_W / 2, SCREEN_H / 2, 1.0f, 0, game_rule_handle, TRUE, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);   //半透明設定
    DrawBox(50, 30, SCREEN_W - 50, SCREEN_H - 30, GetColor(255, 255, 255), 1);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);   //半透明の設定を戻っに戻す

    SetFontSize(60);   //文字サイズ設定
    DrawString(60, 50, "GAME_RULE:", GetColor(10, 40, 200), TRUE);
    SetFontSize(40);
    DrawString(60, 150, "途中のモンスターを倒しつつ", GetColor(50, 50, 50), TRUE);
    DrawString(60, 200, "ネズミさんを家まで護衛しましょう！", GetColor(50, 50, 50), TRUE);
    SetFontSize(60);   //文字サイズ設定
    DrawString(60, 420, "操作説明：", GetColor(10, 40, 200), TRUE);
    SetFontSize(40);

    DrawString(60, 500, "AWSD：移動", GetColor(50, 50, 50), TRUE);
    DrawString(60, 550, "マウス左：弾丸発射", GetColor(50, 50, 50), TRUE);
    DrawString(60, 600, "マウス右：カメラの左右移動", GetColor(50, 50, 50), TRUE);
    DrawString(60, 650, "マウスホイール：カメラ距離", GetColor(50, 50, 50), TRUE);

    DrawString(800, 600, "SPACE：JUMP", GetColor(50, 50, 50), TRUE);
    DrawString(800, 650, "ENTER:BGM調整", GetColor(50, 50, 50), TRUE);
    SetFontSize(40);

    //マウスが判定用区域居た時、いない時
    if(CheckInSquare(title_startpos, title_endpos, mouseX, mouseY)) {
        DrawRotaGraph(title_centerpos.x, title_centerpos.y, 1.0f, 0.0, title_handle, TRUE, FALSE);
    }
    else {
        DrawRotaGraph(title_centerpos.x, title_centerpos.y, 1.0f, 0.0, title_handle_2, TRUE, FALSE);
    }
}