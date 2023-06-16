#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include<windows.h>
#include<time.h>
#include<math.h>

#pragma warning (disable : 4819)
#include<d3d9.h>
#include<DxLib.h>

#define SCREEN_W 1500
#define SCREEN_H 850

bool CheckMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
bool PushMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
int GetMouseX();
int GetMouseY();
int GetMouseMoveX();
int GetMouseMoveY();
float TO_RADIAN(float degree);
float TO_DEGREE(float radian);
