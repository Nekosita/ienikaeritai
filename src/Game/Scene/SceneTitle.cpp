// ==================================================
// SceneTitle.cpp
//	タイトル画面用のファイル
// ==================================================
#include "../Game.h"
#include "../Main.h"
#include "../src/System/KeyInput.h"
#include "SceneTitle.h"

namespace
{
int seclect_num;   //!<選択用の変数
int num_X;         //!<選択肢の初期座標X
int num_Y;         //!<選択肢の初期座標Y

int mouseX, mouseY;

//GAME_START画像判定用
int    game_start_handle;
int    game_start_handle_2;
VECTOR game_start_centerpos;
int    game_start_weight;
int    game_start_height;
VECTOR game_start_startpos;
VECTOR game_start_endpos;

//GAME_RULE判定用
int    game_rule_handle;
int    game_rule_handle_2;
VECTOR game_rule_centerpos;
int    game_rule_weight;
int    game_rule_height;
VECTOR game_rule_startpos;
VECTOR game_rule_endpos;

}   // namespace

//初期化処理
//--------------------------------
void InitGameTitle()
{
    //ゲームスタート画面の選択用
    seclect_num = 0;
    num_X       = 280;
    num_Y       = 650;

    game_start_centerpos.x = SCREEN_W / 2 - 450;
    game_start_centerpos.y = SCREEN_H / 2 + 100;
    game_start_handle      = LoadGraph("Data/Icon/start.png");
    game_start_handle_2 = LoadGraph("Data/Icon/start_2.png");
    game_rule_centerpos.x = SCREEN_W / 2 - 450;
    game_rule_centerpos.y = SCREEN_H / 2 + 250;
    game_rule_handle      = LoadGraph("Data/Icon/rule.png");
    game_rule_handle_2     = LoadGraph("Data/Icon/rule_2.png");
}
//戻り値: 選択されたシーンの番号
//------------------------------------------
GAME_SCREEN UpdateGameTitle()
{
    //GAME_STARTの左上座標
    game_start_startpos.x = game_start_centerpos.x - game_start_weight / 2;
    game_start_startpos.y = game_start_centerpos.y - game_start_height / 2;
    //GAME_STARTの右下座標
    game_start_endpos.x = game_start_centerpos.x + game_start_weight / 2;
    game_start_endpos.y = game_start_centerpos.y + game_start_height / 2;
    //GAME_RULEの左上座標
    game_rule_startpos.x = game_rule_centerpos.x - game_rule_weight / 2;
    game_rule_startpos.y = game_rule_centerpos.y - game_rule_height / 2;
    //GAME_RULEの右下座標
    game_rule_endpos.x = game_rule_centerpos.x + game_rule_weight / 2;
    game_rule_endpos.y = game_rule_centerpos.y + game_rule_height / 2;


    //マウス座標と画像座標用の変数を入れること
    GetMousePoint(&mouseX, &mouseY);
    GetGraphSize(game_start_handle, &game_start_weight, &game_start_height);
    GetGraphSize(game_rule_handle, &game_rule_weight, &game_rule_height);

    //もしもGAME_START範囲の中であれば
    if(CheckInSquare(game_start_startpos, game_start_endpos, mouseX, mouseY)) {
        //マウスを押したらシーン変えます
        if(((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)) {
            if(CheckMusic() == 0) {
                PlaySoundFile("Data/music/魔王魂 効果音 システム49.mp3", DX_PLAYTYPE_BACK);
            }
            return GAME_SCREEN::GAME_NOWLOADING;
        }
    }

    //もしもGAME_RULE範囲の中であれば
    if(CheckInSquare(game_rule_startpos, game_rule_endpos, mouseX, mouseY)) {
        //マウスを押したらシーン変えます
        if(((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)) {
            if(CheckMusic() == 0) {
                PlaySoundFile("Data/music/魔王魂 効果音 システム49.mp3", DX_PLAYTYPE_BACK);
            }
            return GAME_SCREEN::GAME_RULE;
        }
    }
    return GAME_SCREEN::GAME_TITLE;
}
//描画処理
//---------------------------------------------
void RenderGameTitle()
{
  
    SetFontSize(80);
    DrawString(330, 70, "家に帰りたい！", GetColor(50, 255, 130), TRUE);


    //マウスが判定用区域居た時、いない時
    if(CheckInSquare(game_start_startpos, game_start_endpos, mouseX, mouseY)) {
        DrawRotaGraph(game_start_centerpos.x, game_start_centerpos.y, 1.0f, 0.0, game_start_handle, TRUE, FALSE);
    }
    else {
        DrawRotaGraph(game_start_centerpos.x, game_start_centerpos.y, 1.0f, 0.0, game_start_handle_2, TRUE, FALSE);
    }

    //マウスが判定用区域居た時、いない時
    if(CheckInSquare(game_rule_startpos, game_rule_endpos, mouseX, mouseY)) {
        DrawRotaGraph(game_rule_centerpos.x, game_rule_centerpos.y, 1.0f, 0.0, game_rule_handle, TRUE, FALSE);
    }
    else {
        DrawRotaGraph(game_rule_centerpos.x, game_rule_centerpos.y, 1.0f, 0.0, game_rule_handle_2, TRUE, FALSE);
    }

}

bool CheckInSquare(VECTOR Pos_1, VECTOR Pos_2, int Mouse_x, int Mouse_y)
{
    if((Mouse_x > Pos_1.x) && (Mouse_x < Pos_2.x) && (Mouse_y > Pos_1.y) && (Mouse_y < Pos_2.y)) {
        return true;
    }
    return false;
}
