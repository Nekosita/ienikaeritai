#pragma once

// ==================================================
//	!GAME_SCREEN::GAME_RULEで使う変数・関数
// ==================================================

void InitGameRule();
GAME_SCREEN UpdateGameRule();
void RenderGameRule();
//Pos1 は左上の開始座標、Pos_2は右下の終わり座標、Mouse_x,Mouse_yはマウス座標
bool CheckInSquare(VECTOR Pos_1, VECTOR Pos_2, int Mouse_x, int Mouse_y);