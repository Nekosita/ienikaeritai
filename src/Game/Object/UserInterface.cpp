#include "UserInterface.h"
#include "../src/Game/Main.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Model/Npc.h"
#include "../src/Game/Object/Model/Boss.h"
#include "../src/Game/Object/Camera.h"
#include "../src/System/Vector3.h"
#include "../src/System/AppendFunction.h"
#include "DxLib.h"

void UserInterface::SetNum(int now_num)
{
    now_num = num;
}
void UserInterface::AddNum()
{
    num++;
}
int UserInterface::Getnum() const
{
    return num;
}
void UserInterface::SetTimeCount(int now_count)
{
    time_count = now_count;
}
int UserInterface::GetTimeCount() const
{
    return time_count;
}
void UserInterface::MinueTimeCount()
{
    time_count--;
}
void UserInterface::Init()
{
    SetTimeCount(300);
    SetNum(0);   //アイテムを拾った数
    time_count_flame        = 0;
    mute_handle             = LoadGraph("Data/mute.png");
    keybord_picture_inpause = LoadGraph("Data/Directions.png");

    title_handle      = LoadGraph("Data/Icon/title.png");
    title_handle_2    = LoadGraph("Data/Icon/title_2.png");
    title_centerpos.x = SCREEN_W / 2;
    title_centerpos.y = SCREEN_H - 175.0f;

    skip_handle     = LoadGraph("Data/Icon/skip.png");
    skip_handle_2     = LoadGraph("Data/Icon/skip_2.png");
    skip_centerpos.x = SCREEN_W / 2 + 500;
    skip_centerpos.y  = SCREEN_H / 2 + 350;
}

void UserInterface::Update()
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

    check_title = CheckInSquare(title_startpos, title_endpos, mouseX, mouseY);
}

void UserInterface::Update_In_end_anime()
{
    //GAME_RULEの左上座標
    skip_startpos.x  = skip_centerpos.x - skip_weight / 2;
    skip_startpos.y = skip_centerpos.y - skip_height / 2;
    //GAME_RULEの右下座標
    skip_endpos.x  = skip_centerpos.x + skip_weight / 2;
    skip_endpos.y = skip_centerpos.y + skip_height / 2;

    //マウス座標と画像座標用の変数を入れること
    GetMousePoint(&mouseX, &mouseY);
    GetGraphSize(skip_handle, &skip_weight, &skip_height);

    check_skip = CheckInSquare(skip_startpos, skip_endpos, mouseX, mouseY);
}

//残り時間をの計算関数
//----------------------------------------------
void UserInterface::TimeCountFlame()
{
    //タイムカウンター
    if(camera_ptr->GetOpenAnimeCount() < 0) {
        time_count_flame++;
        if(time_count_flame == 60) {
            time_count_flame = 0;
            MinueTimeCount();
        }
    }
}
//HPとカウンター描画
// ----------------------------------------------
void UserInterface::DrawUi(bool zonbi_dead)
{
    //開幕の右上のヒントバー（プレイヤーに指示を出す
    SetFontSize(25);

    if((camera_ptr->GetOpenAnimeCount() < 0)) {
        DrawBox(SCREEN_W - 250, 0, SCREEN_W, 150, GetColor(0, 0, 0), FALSE);
        DrawString(SCREEN_W - 245, 10, "MISSION:", GetColor(255, 0, 0));
        DrawLine(SCREEN_W - 250, 40, SCREEN_W, 40, GetColor(0, 0, 0), 2);

        //ゾンビが倒されないなら
        if(!zonbi_dead && !boss_ptr->Get_is_dead()) {
            DrawString(SCREEN_W - 250, 60, "ゾンビを全て", GetColor(255, 0, 0));
            DrawString(SCREEN_W - 250, 90, "倒しましょう！", GetColor(255, 0, 0));
        }
        //ゾンビ全部倒したら
        if(zonbi_dead && !boss_ptr->Get_is_dead()) {
            //NPCがシェルターに居たらこのセリフ
            if(npc_ptr->Getin_the_sheltter()) {
                //BOSSの所まで進んだら
                DrawString(SCREEN_W - 250, 90, "BOSSを倒そう！", GetColor(255, 0, 0));
            }
            else {
                //シェルターに居て無いなら
                DrawString(SCREEN_W - 250, 60, "BOSSの所まで", GetColor(255, 0, 0));
                DrawString(SCREEN_W - 250, 90, "目指しましょう！", GetColor(255, 0, 0));
            }
        }

        if(boss_ptr->Get_is_dead()) {
            //ボスが死んだ時のセリフ
            DrawString(SCREEN_W - 250, 60, " ⇒の所まで", GetColor(255, 0, 0));
            DrawString(SCREEN_W - 250, 90, "目指しましょう！", GetColor(255, 0, 0));
            //
        }
    }

    //時間
    char time[256];
    SetFontSize(40);
    int minute = time_count / 60;
    int second = time_count % 60;
    sprintf_s(time, "残り時間:%01d分%02d秒", minute, second);
    DrawString(0, 10, time, GetColor(0, 0, 0));

    //操作関係
    DrawString(50, SCREEN_H - 100, "Enter:設定", GetColor(0, 0, 0));
    SetFontSize(60);
    if((camera_ptr->GetOpenAnimeCount() > 0) || (camera_ptr->GetEndAnimeCount() < 90)) {
        if(CheckInSquare(skip_startpos, skip_endpos, mouseX, mouseY)) {
            DrawRotaGraph(skip_centerpos.x, skip_centerpos.y, 1.0f, 0.0, skip_handle, TRUE, FALSE);
        }
        else {
            DrawRotaGraph(skip_centerpos.x, skip_centerpos.y, 1.0f, 0.0, skip_handle_2, TRUE, FALSE);
        }
    }
    SetFontSize(40);
}

//GAME_PAUSEのテキスト描画
// ----------------------------------------------
void UserInterface::DrawGamepauseText(bool music_on, int sound)
{
    SetFontSize(40);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);   //半透明設定
    DrawBox((SCREEN_W / 2) - 100, 100, SCREEN_W - 100, SCREEN_H - 100, GetColor(255, 255, 255), 1);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);   //半透明の設定を戻っに戻す
    DrawString((SCREEN_W / 2), 140, "音量:", GetColor(0, 0, 0));
    //音量確認
    if(music_on) {
        char text_volume[256];
        sprintf_s(text_volume, "%3d", sound);
        DrawString((SCREEN_W / 2) + 100, 140, text_volume, GetColor(0, 0, 0));
        float box_x = 200;
        DrawFillBox((SCREEN_W / 2) + 255, 140, (SCREEN_W / 2) + 510, 180, GetColor(0, 0, 0));
        DrawFillBox((SCREEN_W / 2) + 255, 140, (SCREEN_W / 2) + 255 + (int)(sound), 180, GetColor(100, 100, 100));
        DrawLineBox((SCREEN_W / 2) + 255, 140, (SCREEN_W / 2) + 510, 180, GetColor(160, 160, 160));
    }
    if(!music_on) {
        DrawRotaGraph((SCREEN_W / 2) + 300, 155, 0.2f, 0, mute_handle, TRUE, FALSE);
    }
    SetFontSize(40);
    SetFontSize(30);
    DrawString((SCREEN_W / 2), int(210), "右キー・左キーでBGMの音量調整", GetColor(0, 0, 0), TRUE);
    DrawString((SCREEN_W / 2), int(240), "１キーでONとOFF切り替え", GetColor(0, 0, 0), TRUE);
    SetFontSize(40);
    //HINT_BAR描画
    SetFontSize(50);
    DrawString((SCREEN_W / 2), SCREEN_H / 2, "操作 :", GetColor(0, 0, 0), TRUE);
    SetFontSize(30);
    DrawRotaGraph((SCREEN_W / 2) + 300, SCREEN_H / 2 + 180, 0.5f, 0, keybord_picture_inpause, TRUE, FALSE);
    SetFontSize(30);
    SetFontSize(40);
}
//GAME_SCREEN::GAME_OVERで使う変数・関数
// --------------------------------------------------
void UserInterface::DrawGameoverText()
{
    SetFontSize(60);
    DrawString(int(SCREEN_W / 2 - 150.0f), int(SCREEN_H / 2 - 350.0f), "GAME OVER", GetColor(237, 28, 36), TRUE);
    SetFontSize(60);
    if(CheckInSquare(title_startpos, title_endpos, mouseX, mouseY)) {
        DrawRotaGraph(title_centerpos.x, title_centerpos.y, 1.0f, 0.0, title_handle, TRUE, FALSE);
    }
    else {
        DrawRotaGraph(title_centerpos.x, title_centerpos.y, 1.0f, 0.0, title_handle_2, TRUE, FALSE);
    }
}
//GAME_SCREEN::GAME_CLEARで使う変数・関数
// --------------------------------------------------
void UserInterface::DrawGameClearText()
{
    SetFontSize(60);
    DrawString(int(SCREEN_W / 2 - 150.0f), int(SCREEN_H / 2 - 350.0f), "GAME CLEAR", GetColor(237, 28, 36), TRUE);
    SetFontSize(60);
    if(CheckInSquare(title_startpos, title_endpos, mouseX, mouseY)) {
        DrawRotaGraph(title_centerpos.x, title_centerpos.y, 1.0f, 0.0, title_handle, TRUE, FALSE);
    }
    else {
        DrawRotaGraph(title_centerpos.x, title_centerpos.y, 1.0f, 0.0, title_handle_2, TRUE, FALSE);
    }
}