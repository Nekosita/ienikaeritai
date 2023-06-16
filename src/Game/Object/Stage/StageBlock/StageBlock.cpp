#include "StageBlock.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Model/Npc.h"
#include "../src/Game/Object/Model/RedGoblin.h"
#include "../src/Game/Object/Model/Goblin.h"


//コンストラクタ
//-----------------------------------------------
StageBlock::StageBlock(size_t block_num)
{
    stg_block_num = int(block_num);
    block_handle  = MV1LoadModel("Data/Stage/0.005stone.mqoz");

    //当たり判定用の位置パラメータの用意
    constexpr VECTOR STAGEBLOCK_START_POS[] = {
        #include "StageStartPos.inc"
    };

    constexpr VECTOR STAGEBLOCK_END_POS[] = {
        #include "StageEndPos.inc"
    };

    //座標設定
    SetBlockStartPos(STAGEBLOCK_START_POS[stg_block_num]);
    SetBlockEndPos(STAGEBLOCK_END_POS[stg_block_num]);

}
//デストラクタ
//-----------------------------------------------
StageBlock::~StageBlock()
{
    Exit();
}
void StageBlock::Update_player()
{
    //もしもnullならその先の処理をしない(プレイヤーのデータがないなら前に行っても意味ない
    if(player_ptr == nullptr)
        return;

    const VECTOR& player_top_line_start     = player_ptr->GetTopLineStart();
    const VECTOR& player_top_line_end       = player_ptr->GetTopLineEnd();
    const VECTOR& player_gravity_line_start = player_ptr->GetGravityLineStart();
    const VECTOR& player_gravity_line_end   = player_ptr->GetGravityLineEnd();

    //プレイヤーが下の板をぶつかったら強制出来に落ちます
    block_b1  = HitCheck_Line_Triangle(player_top_line_start,
                                       player_top_line_end,
                                       GetBlock1BottomA(),
                                       GetBlock1BottomB(),
                                       GetBlock1BottomC());
    block_b2 = HitCheck_Line_Triangle(player_top_line_start,
                                       player_top_line_end,
                                       GetBlock1BottomA(),
                                       GetBlock1BottomD(),
                                       GetBlock1BottomC());

    //ブロックの当たり判定
    //プレイヤーが上板を当たったら上にいます
    block_t1  = HitCheck_Line_Triangle(player_gravity_line_start,
                                       player_gravity_line_end,
                                       GetBlock1TopA(),
                                       GetBlock1TopB(),
                                       GetBlock1TopC());
    block_t2 = HitCheck_Line_Triangle(player_gravity_line_start,
                                       player_gravity_line_end,
                                       GetBlock1TopA(),
                                       GetBlock1TopD(),
                                       GetBlock1TopC());

}
void StageBlock::Render()
{
    SetUseLighting(TRUE);   //光を設定します

    //ブロックを正しく描画
    SetUseZBufferFlag(TRUE);
    SetWriteZBufferFlag(TRUE);

    //当たり判定用の位置パラメータに合わせて外見の座標設定
    VECTOR block_pos;
    block_pos.x = (end_pos.x + start_pos.x) / 2;
    block_pos.y = start_pos.y + STAGE_HIGHT / 2;
    block_pos.z = (end_pos.z + start_pos.z) / 2;

    //当たり判定用の位置パラメータに合わせて外見の倍率設定
    VECTOR block_rate;
    block_rate.x = (end_pos.x - start_pos.x);
    block_rate.y = 150.05f;
    block_rate.z = (end_pos.z - start_pos.z);

    //ブロックの外見
    MV1SetScale(block_handle, VGet(block_rate.x, block_rate.y, block_rate.z));   //モデルの大きさを変えます
    MV1SetRotationXYZ(block_handle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(block_handle, VGet(block_pos.x, block_pos.y, block_pos.z));
    MV1DrawModel(block_handle);
    MV1SetMaterialDrawBlendModeAll(block_handle, DX_BLENDMODE_NOBLEND);
}
void StageBlock::Exit()
{
    MV1DeleteModel(block_handle);
}
