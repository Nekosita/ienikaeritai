#include "HealItem.h"
#include "../Base.h"
#include "Player.h"
#include "../src/System/AppendFunction.h"
#include "DxLib.h"

// コンストラクタ
//----------------------------------------------------
HealItem::HealItem(size_t item_num)
{
    model_handle  = MV1LoadModel("Data/Item/uploads_files_3773231_pill+bottle.mv1");   //モデル実装
    vertex_shader = LoadVertexShader("shader/SampleVS.vso");
    pixel_shader  = LoadPixelShader("shader/SamplePS.pso");

    //位置パラメータの用意
    constexpr VECTOR ITEM_POS_LIST[] = {
        { 55.0f * 200.0f, 1850.0f,  35.0f * 200.0f},
        {145.0f * 200.0f, 1850.0f, -10.0f * 200.0f},
        {210.0f * 200.0f, 4350.0f,  10.0f * 200.0f},
    };

    pos           = ITEM_POS_LIST[item_num];   //座標設定
    now_heal_item = NORMAL;
    SetIsPicked(false);   //拾ってない
    SetRadius(225.0f); 
    rot.y = 180.0f;       //初期角度

    move_count = TO_RADIAN(90.0f);
}
//デストラクタ
//-----------------------------------------------
HealItem::~HealItem()
{
    Exit();
}
void HealItem::SetIsPicked(bool now_picked)
{
    is_picked = now_picked;
}
bool HealItem::GetIsPicked() const
{
    return is_picked;
}
//アップデート
//-----------------------------------------------
void HealItem::Update()
{

    if(is_picked) {
        now_heal_item = PICKED;
    }

    switch(now_heal_item) {
    case HealItem::NORMAL:
        rot.y += TO_RADIAN(1.0f);
        move_count += TO_RADIAN(3.0f);
        pos.y += sinf(move_count) * 1.5f;

        break;
    case HealItem::PICKED:
        pos = {0.0f, -12000.0f, 0.0f};
        break;
    }
}
//描画
//-----------------------------------------------
void HealItem::Render()
{
    //シェーダを使うー
    //MV1SetUseOrigShader(TRUE);
    //使用する頂点シェーダーのセット
    //SetUseVertexShader(vertex_shader);
    ////使用するピクセルシェーダーをセット
    //SetUsePixelShader(pixel_shader);
    ////モデルの大きさを設定
    MV1SetScale(model_handle, VGet(30.0f, 30.0f, 30.0f));

    //モデルの描画
    MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
    MV1SetPosition(model_handle, pos);
    MV1DrawModel(model_handle);

    //シェーダを閉じる
    MV1SetUseOrigShader(FALSE);
}
void HealItem::Exit()
{
    //読み込んだ頂点シェーダーの削除
    DeleteShader(vertex_shader);
    //読み込んだピクセルシェーダーの削除
    DeleteShader(pixel_shader);
    MV1DeleteModel(model_handle);
}
