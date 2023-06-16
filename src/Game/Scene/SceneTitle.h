#pragma once
// ==================================================
// SceneTitle.h
//	タイトル画面用のファイル
// ==================================================

void InitGameTitle();
GAME_SCREEN UpdateGameTitle();
void RenderGameTitle();
//Pos1 は左上の開始座標、Pos_2は右下の終わり座標、Mouse_x,Mouse_yはマウス座標
bool CheckInSquare(VECTOR Pos_1, VECTOR Pos_2, int Mouse_x, int Mouse_y);
