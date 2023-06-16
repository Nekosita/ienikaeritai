#include "MapChip.h"
#include "DxLib.h"


//コンストラクタ
//---------------------------------
MapChip::MapChip()
	:count(0)
    , water_handle(0)
	, swamp_x(0.0)
{

}
void MapChip::BuildMap()
{

	water_handle = MV1LoadModel("Data/Stage/water.mqoz");

	count = 0;
}
void MapChip::Update()
{

}
//デストラクタ
//---------------------------------
MapChip::~MapChip() 
{
}
//地図作成
//---------------------------------
void MapChip::DrawMap()
{

	SetBackgroundColor(150, 200, 255);

	SetUseLighting(TRUE);//光を照らす
	SetUseZBufferFlag(TRUE);	//ブロックを正しく描画
	SetWriteZBufferFlag(TRUE);

	//沼を描画
    MV1SetScale(water_handle, VGet(5000.0f, 3.f, 5000.0f));   //モデルの大きさを変えます
    MV1SetRotationXYZ(water_handle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(water_handle, VGet(0.0f * 200.0f, -10.0f, 0.0f * 200.0f));
    MV1DrawModel(water_handle);

	SetUseLighting(TRUE);//立方体描画終了後にライティングを再びON
}
