#include "Npc_Sheltter.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/System/Vector3.h"
#include "DxLib.h"

//コンストラクタ
//-------------------------------
Npc_Sheltter::Npc_Sheltter()
{
    SetBlockStartPos(VGet(200.0f * 200.0f, 4150.f, -20.0f * 200.0f));
    SetBlockEndPos(VGet(220 * 200.0f, 4800.f, -10.0f * 200.0f));

    sheltterpos = {(start_pos.x + end_pos.x) / 2, 4500.f, (start_pos.z + end_pos.z) / 2};

    Sheltter_model = MV1LoadModel("Data/Character/Trash Container/Rubbish_Bin_By_Berken fbx.mv1");
}
//デストラクタ
//------------------------------------
Npc_Sheltter::~Npc_Sheltter()
{
    Exit();
}
void Npc_Sheltter::Update()
{
}
//描画用の関数
//---------------------------------------
void Npc_Sheltter::Render()
{
    SetUseLighting(FALSE);   //光を消す
    //ブロックを正しく描画
    SetUseZBufferFlag(TRUE);
    SetWriteZBufferFlag(TRUE);
    //DrawCube3D(start_pos, end_pos, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);

    //VECTOR trashcanpos = {
    //    (start_pos.x + end_pos.x) / 2,
    //    (start_pos.y + end_pos.y) / 2,
    //    (start_pos.z + end_pos.z) / 2,
    //};
    MV1SetScale(Sheltter_model, VGet(5.f, 5.f, 5.f)); 
    MV1SetRotationXYZ(Sheltter_model, VGet(rot.x, TO_RADIAN(180.0f), rot.z));
    MV1SetPosition(Sheltter_model, VGet(sheltterpos.x, sheltterpos.y-500.0f, sheltterpos.z));
    MV1DrawModel(Sheltter_model);

    SetUseLighting(TRUE);   //光を戻す
}
void Npc_Sheltter::Exit()
{
}
