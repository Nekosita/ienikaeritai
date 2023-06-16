#include"TriggerArea.h"
#include"../src/Game/Object/Model/Player.h"


//コンストラクタ
//--------------------------------------
TriggerArea::TriggerArea(size_t _num)
{
	trap_num = int(_num);

	//位置パラメータの用意
	constexpr VECTOR TRIGGER_START_POS[] = {
		#include"TriggerAreaStartPos.inc"
	};
    constexpr VECTOR TRIGGER_END_POS[] = {
		#include"TriggerAreaEndPos.inc"
	};

    SetTrigger(false);
	//座標設定
    SetBlockStartPos(TRIGGER_START_POS[trap_num]);
    SetBlockEndPos(TRIGGER_END_POS[trap_num]);

	sayu_block_handle = MV1LoadModel("Data/Stage/sayu_block.mqoz");

}
//デストラクタ
//-------------------------------------
TriggerArea::~TriggerArea()
{

}
void TriggerArea::Update()
{

	const VECTOR& player_gravity_line_start = player_ptr->GetGravityLineStart();
	const VECTOR& player_gravity_line_end = player_ptr->GetGravityLineEnd();

	//ブロックの当たり判定
	//プレイヤーが上板を当たったら上にいます
    block_t1  = HitCheck_Line_Triangle(player_gravity_line_start,
                                       player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopB(), GetBlock1TopC());
    block_t2 = HitCheck_Line_Triangle(player_gravity_line_start,
                                       player_gravity_line_end,
		GetBlock1TopA(), GetBlock1TopD(), GetBlock1TopC());

}

void TriggerArea::Render()
{	
	SetUseLighting(FALSE);//光を消す
	SetUseZBufferFlag(TRUE);//ブロックを正しく描画
	SetWriteZBufferFlag(TRUE);
	DrawCube3D(start_pos, end_pos, GetColor(150, 0, 0), GetColor(150, 0, 0), TRUE);

	VECTOR block_pos = {(start_pos.x + end_pos.x) / 2, start_pos.y + (4.0f / 2), (start_pos.z + end_pos.z) / 2};
    VECTOR block_rate;

    block_rate.x = (end_pos.x - start_pos.x);
    block_rate.y = 6.0f;
    block_rate.z = (end_pos.z - start_pos.z);

    //ブロックの外見
    MV1SetScale(sayu_block_handle, VGet(block_rate.x, block_rate.y, block_rate.z));   //モデルの大きさを変えます
    MV1SetRotationXYZ(sayu_block_handle, VGet(0.0f, TO_RADIAN(90.0f), 0.0f));
    MV1SetPosition(sayu_block_handle, VGet(block_pos.x, block_pos.y, block_pos.z));
    MV1DrawModel(sayu_block_handle);


	SetUseLighting(TRUE);//立方体描画終了後にライティングを再びON
}
void TriggerArea::Exit()
{

}
