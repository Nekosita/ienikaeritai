#include"../Game.h"
#include"../Main.h"
#include"SceneNowLoading.h"
#include"../src/System/KeyInput.h"


int keybord_picture;   //!<説明画像用

void GameStartAnime() {
	keybord_picture = LoadGraph("Data/Directions.png");
}

GAME_SCREEN UpdateGameNowLoading()
{
    return GAME_SCREEN::GAME_STAGE;
}
//DrawGameNowLoadingTextのテキスト描画
//----------------------------------------------
void DrawGameNowLoadingText() {
	SetFontSize(60);//文字サイズ設定
	DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), 1);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);//半透明設定
	DrawBox(0, 150, SCREEN_W, SCREEN_H - 150, GetColor(255, 255, 255), 1);
	DrawRotaGraph((int)(800), (int)(450), 1.0f, 0, keybord_picture, TRUE, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//半透明の設定を戻っに戻す
	DrawString(60, 200, "TIP：", GetColor(10, 40, 200), TRUE);
	SetFontSize(60);//文字サイズ設定
	DrawString(SCREEN_W - 600, SCREEN_H - 150, "NOW LOADING.....", GetColor(255, 255, 255), TRUE);
	SetFontSize(40);
}