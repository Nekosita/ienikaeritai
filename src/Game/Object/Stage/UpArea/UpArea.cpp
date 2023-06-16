#include "UpArea.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/System/Vector3.h"
#include "DxLib.h"

//コンストラクタ
//-------------------------------
UpArea::UpArea(size_t _num)
{
    SetBlockStartPos(VGet(185.0f * 200.0f, 1752.0f, 0.0f * 200.0f));
    SetBlockEndPos(VGet(195.0f * 200.0f, 1753.0f, 10.0f * 200.0f));

    up_block_handle = MV1LoadModel("Data/Stage/up_block.mqoz");
}
//デストラクタ
//------------------------------------
UpArea::~UpArea()
{
    Exit();
}
void UpArea::Update(bool player_npc_is_on)
{
    if(player_npc_is_on) {
        //カウンターに合わせて移動する
        move_vector = {0.0f, 32.0f, 0.0f};
        start_pos.y += move_vector.y;
        end_pos.y += move_vector.y;
        if(start_pos.y > 4150.f) {
            move_vector = {0.0f, 0.0f, 0.0f};
            start_pos.y = 4150.f;
            end_pos.y   = start_pos.y + 4.0f;
        }
    }
}
//描画用の関数
//---------------------------------------
void UpArea::Render()
{
    SetUseLighting(FALSE);   //光を消す

    //ブロックを正しく描画
    SetUseZBufferFlag(TRUE);
    SetWriteZBufferFlag(TRUE);

    DrawCube3D(start_pos, end_pos, GetColor(0, 0, 150), GetColor(0, 0, 0), TRUE);

    VECTOR block_pos = {(start_pos.x + end_pos.x) / 2, start_pos.y +(4.0f/2), (start_pos.z + end_pos.z) / 2};
    VECTOR block_rate;

    block_rate.x = (end_pos.x - start_pos.x);
    block_rate.y = 6.0f;
    block_rate.z = (end_pos.z - start_pos.z);

    //ブロックの外見
    MV1SetScale(up_block_handle, VGet(block_rate.x, block_rate.y, block_rate.z));   //モデルの大きさを変えます
    MV1SetRotationXYZ(up_block_handle, VGet(0.0f,TO_RADIAN(90.0f), 0.0f));
    MV1SetPosition(up_block_handle, VGet(block_pos.x, block_pos.y, block_pos.z));
    MV1DrawModel(up_block_handle);

    SetUseLighting(TRUE);   //光を戻す
}
void UpArea::Exit()
{
    MV1DeleteModel(up_block_handle);
}
