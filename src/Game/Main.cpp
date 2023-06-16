#include"DxLib.h"
#include"../src/Game/Object/MapChip.h"
#include"../src/Game/Object/Model/Player.h"
#include"../System/KeyInput.h"
#include"Game.h"
#include"../System/Effect.h"
#include"Main.h"
#include"../src/Game/Object/Base.h"
#include"../src/Game/Object/Model/Player.h"
#include<vector>
#include<array>
#include<string>
#define _CRT_SECURE_NO_WARNINGS


char KeyBuffer[256];
int	 MouseLeftFrame;
int	 MouseRightFrame;

int NowMouseX;
int NowMouseY;
int BeforeMouseX;
int BeforeMouseY;

int StageModelHandle;
int CharaModelHandle;
int ShadowMapHandle;

//clang-format off
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR	 lpCmdLine, int	nCmdShow)
{

	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetMainWindowText("家に帰りたい！");
	SetBackgroundColor(0, 0, 0);
	SetDoubleStartValidFlag(TRUE);
	SetAlwaysRunFlag(TRUE);

	SetGraphMode(SCREEN_W, SCREEN_H, 32);
	SetBackgroundColor(255, 255, 255);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
		
	if (DxLib_Init() < -1)
		return -1;

	//描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);
	//Zバッファを有効に設定
	SetUseZBufferFlag(TRUE);
	//Zバッファへの書き込みを有効に設定
	SetWriteZBufferFlag(TRUE);
	//ライティングの計算をしないように設定を変更
	SetUseLighting(FALSE);

	//画像透化用
	//白と設定すれば背景が白のがは実装しても白の所は自動的に透明になる
	SetTransColor(255, 255, 255);
	srand(GetNowCount() % RAND_MAX);

	EffectSystem::Init(1920, 1080);
	
	Game game;
	game.Init();

	//描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);

	//シャドウマップハンドルの作成
	ShadowMapHandle = MakeShadowMap(8192, 8192);

	//ライトの方向を設定
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	//シャドウマップが想定するライトの方向もセット
	VECTOR	shadowDirection = VNorm(VGet(0.5f, -1.0f, 0.5f));
	SetShadowMapLightDirection(ShadowMapHandle, shadowDirection);
	//シャドウマップに描画する範囲を設定
	SetShadowMapAdjustDepth(ShadowMapHandle, 0.0005f);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		float fps = GetFPS();

		// 画面をクリアする
		ClearDrawScreen();
		KeyInputUpdate();

		EffectSystem::Update();

		game.Update();

		// プレイヤーが1人しかない時だげ使われるわコード
		Player* player = Player::GetPlayer();
		VECTOR position = player->GetPos();
        SetShadowMapDrawArea(ShadowMapHandle,
                             VGet(position.x - 5000.f, position.y - 200, position.z - 5000.f),
                             VGet(position.x + 20000.f, position.y + 200, position.z +20000.f));

		// シャドウマップへの描画の準備
		ShadowMap_DrawSetup(ShadowMapHandle);
		game.Render(true);
		// シャドウマップへの描画を終了
		ShadowMap_DrawEnd();
		//描画に使用するシャドウマップを設定
		SetUseShadowMap(0, ShadowMapHandle);

		game.Render(false);

		// 描画に使用するシャドウマップの設定を解除
		SetUseShadowMap(0, -1);
		EffectSystem::Draw();
		ScreenFlip();
	}

	// シャドウマップの削除
	DeleteShadowMap(ShadowMapHandle);
	
	EffectSystem::Exit();
	game.Exit();

	DxLib_End();
	return 0;
}
#if 0 
//clang-format on
//---------------------------------------------------------------------------------
//	マウスが押されているかを取得する
//---------------------------------------------------------------------------------
bool CheckMouseInput(int button)
{
	if (GetMouseInput() & button) {
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	マウスが押された瞬間を取得する
//---------------------------------------------------------------------------------
bool PushMouseInput(int button)
{
	if (button & MOUSE_INPUT_LEFT) {
		if (MouseLeftFrame == 1) {
			return true;
		}
	}
	if (button & MOUSE_INPUT_RIGHT) {
		if (MouseRightFrame == 1) {
			return true;
		}
	}
	return false;
}
#endif //if 0
//---------------------------------------------------------------------------------
//マウスの座標を取得する
//---------------------------------------------------------------------------------
int GetMouseX()
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_x;
}
int GetMouseY()
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_y;
}
//---------------------------------------------------------------------------------
//マウスの移動量を取得する
//---------------------------------------------------------------------------------
int GetMouseMoveX()
{
	return NowMouseX - BeforeMouseX;
}
int GetMouseMoveY()
{
	return NowMouseY - BeforeMouseY;
}
//---------------------------------------------------------------------------------
//度をラジアンに変換する関数
//---------------------------------------------------------------------------------
float TO_RADIAN(float degree)
{
	return degree * 3.14159265f / 180.0f;
}
//---------------------------------------------------------------------------------
//ラジアンを度に変換する関数
//---------------------------------------------------------------------------------
float TO_DEGREE(float radian)
{
	return radian * 180.0f / 3.14159265f;
}
