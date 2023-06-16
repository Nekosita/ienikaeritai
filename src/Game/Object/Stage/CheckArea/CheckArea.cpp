#include "CheckArea.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/System/Vector3.h"
#include "DxLib.h"

//コンストラクタ
//-------------------------------
CkeckArea::CkeckArea()
{
    SetBlockStartPos(VGet(200.0f * 200.0f, 4150.f, -20.0f * 200.0f));
    SetBlockEndPos(VGet(220 * 200.0f, 4800.f, 40.0f * 200.0f));

    player_is_in_area = false;//!判定式

}
//デストラクタ
//------------------------------------
CkeckArea::~CkeckArea()
{
    Exit();
}
void CkeckArea::Update()
{
    //プレイヤーの当たり判定を渡す
    if(player_ptr == nullptr)
        return;
    const VECTOR& player_pos     = player_ptr->GetPos();

    //playerの座標判定と判定式取ります　playerがここに入ったら
     if((player_pos.x < end_pos.x) && (start_pos.x < player_pos.x) && (player_pos.z < end_pos.z) &&
       (start_pos.z < player_pos.z)){
        player_is_in_area = true;   //1回しか使わない
    }

    //clsDx();
    //printfDx("toriger:%d \n", player_is_in_area);
}
//描画用の関数
//---------------------------------------
void CkeckArea::Render()
{
    SetUseLighting(FALSE);   //光を消す
    //ブロックを正しく描画
    SetUseZBufferFlag(TRUE);
    SetWriteZBufferFlag(TRUE);

    DrawCube3D(start_pos, end_pos, GetColor(0, 0, 150), GetColor(0, 0, 0), FALSE);
    SetUseLighting(TRUE);   //光を戻す
}
void CkeckArea::Exit()
{
}
