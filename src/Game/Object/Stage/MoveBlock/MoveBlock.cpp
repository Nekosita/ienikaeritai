#include "MoveBlock.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Model/Npc.h"
#include "../src/Game/Object/Model/RedGoblin.h"
#include "../src/Game/Object/Model/Goblin.h"

#define SPEED            100.0f
#define INTERVAL         60 * 3
#define MOVESPEED        -20.0f
#define BLOCKSPEED       15.0f
#define COUNT_MAX        90
#define Z_BLOCKCOUNT_MAX 360

MoveBlock::MoveBlock(size_t block_num)
{
    move_block_num = int(block_num);
    //位置パラメータの用意
    constexpr VECTOR MOVEBLOCK_START_POS[] = {
#include "MoveBlockStartPos.inc"
    };
    constexpr VECTOR MOVEBLOCK_END_POS[] = {
#include "MoveBlockEndPos.inc"
    };

    //座標設定
    SetBlockStartPos(MOVEBLOCK_START_POS[move_block_num]);
    SetBlockEndPos(MOVEBLOCK_END_POS[move_block_num]);

    move_block_handle = MV1LoadModel("Data/Stage/0.005stone.mqoz");

    block_count     = 12 * INTERVAL;
    move_count      = 0;
    move_count_long = 0;

    move_vector_block2 = {MOVESPEED, 0.0f, 0.0f};
    move_vector_block3 = {-MOVESPEED, 0.0f, 0.0f};
    move_vector_block4 = {0.0f, 0.0f, MOVESPEED*2};
}
MoveBlock::~MoveBlock()
{
    Exit();
}
//------------------------
//移動するブロックの設定用
//----------------------
void MoveBlock::Update_player()
{
    //プレイヤーの当たり判定を渡す
    if(player_ptr == nullptr)
        return;
    const VECTOR& player_top_line_start     = player_ptr->GetTopLineStart();
    const VECTOR& player_top_line_end       = player_ptr->GetTopLineEnd();
    const VECTOR& player_gravity_line_start = player_ptr->GetGravityLineStart();
    const VECTOR& player_gravity_line_end   = player_ptr->GetGravityLineEnd();

    //プレイヤーが下の板をぶつかったら強制出来に落ちます
    block_b1 = HitCheck_Line_Triangle(player_top_line_start,
                                      player_top_line_end,
                                      GetBlock1BottomA(),
                                      GetBlock1BottomB(),
                                      GetBlock1BottomC());

    block_b2 = HitCheck_Line_Triangle(player_top_line_start,
                                      player_top_line_end,
                                      GetBlock1BottomA(),
                                      GetBlock1BottomD(),
                                      GetBlock1BottomC());

    //プレイヤーが上板を当たったら上にいます
    block_t1 = HitCheck_Line_Triangle(player_gravity_line_start,
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
void MoveBlock::domino_move_down()
{
    move_vector = {0.0f, MOVESPEED, 0.0f};
    start_pos.y += move_vector.y;
    end_pos.y += move_vector.y;
}
void MoveBlock::Move(bool T1, bool T2, bool G1, bool M1)
{
    if(M1 || (block_count < 12 * INTERVAL))
        block_count--;

    move_count++;
    if(move_count > COUNT_MAX) {
        move_count = 0;
    }
    //移動するブロックに移動用の変数を追加します
    switch(move_block_num) {
    case 0:
        if(T1) {
            //カウンターに合わせて移動する
            move_vector = {0.0f, -32.0f, 0.0f};
            start_pos.y += move_vector.y;
            end_pos.y += move_vector.y;
            if(start_pos.y < 1600) {
                move_vector = {0.0f, 0.0f, 0.0f};
                start_pos.y = 1600;
                end_pos.y   = 1600.0f + STAGE_HIGHT;
            }
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 1:
        if(G1) {
            move_vector = {0.0f, 0.0f, -SPEED};
            start_pos.z += move_vector.z;
            end_pos.z += move_vector.z;
            if(start_pos.z < -20.0f * 200.0f) {
                move_vector = {0.0f, 0.0f, 0.0f};
                start_pos.z = -20.0f * 200.0f;
                end_pos.z   = 40.0f * 200.0f;
            }
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 2:
        if((start_pos.x < 120.f * 200.0f) || (end_pos.x > 155.f * 200.0f)) {
            move_vector_block2.x *= (-1);
        }
        move_vector = move_vector_block2;
        start_pos.x += move_vector_block2.x;
        end_pos.x += move_vector_block2.x;
        break;
    case 3:
        if((start_pos.x < 115.f * 200.0f) || (end_pos.x > 150.f * 200.0f)) {
            move_vector_block3.x *= (-1);
        }
        move_vector = move_vector_block3;
        start_pos.x += move_vector.x;
        end_pos.x += move_vector.x;
        break;
    case 4:
        if((start_pos.z < 10.f * 200.0f) || (end_pos.z > 41.f * 200.0f)) {
            move_vector_block4.z *= (-1);
        }
        move_vector = move_vector_block4;
        start_pos.z += move_vector.z;
        end_pos.z += move_vector.z;
        break;
    case 5:
        //カウンターに合わせて移動する
        if(T2) {
            //カウンターに合わせて移動する
            move_vector = {0.0f, 32.0f, 0.0f};
            start_pos.y += move_vector.y;
            end_pos.y += move_vector.y;
            if(start_pos.y > 4000) {
                move_vector = {0.0f, 0.0f, 0.0f};
                start_pos.y = 4000;
                end_pos.y   = 4000.0f + STAGE_HIGHT;
            }
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    //ここからドミノエリア
    case 6:
        if(M1) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 7:
        if(block_count < 11 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 8:
        if(block_count < 10 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 9:
        if(block_count < 9 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 10:
        if(block_count < 8 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 11:
        if(block_count < 7 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 12:
        if(block_count < 6 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 13:
        if(block_count < 5 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 14:
        if(block_count < 4 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 15:
        if(block_count < 3 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 16:
        if(block_count < 2 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 17:
        if(block_count < 1 * INTERVAL) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    case 18:
        if(block_count < 0) {
            domino_move_down();
        }
        else {
            move_vector = {0.0f, 0.0f, 0.0f};
        }
        break;
    }
}
VECTOR MoveBlock::GetMoveVector() const
{
    return move_vector;
}
void MoveBlock::Render()
{
    SetUseLighting(TRUE);   //光を設定します

    //ブロックを正しく描画
    SetUseZBufferFlag(TRUE);
    SetWriteZBufferFlag(TRUE);
    DrawCube3D(start_pos, end_pos, GetColor(255, 217, 105), GetColor(255, 217, 105), FALSE);

    //テクスチャーの座標設定
    VECTOR block_pos;

    block_pos.x = (end_pos.x + start_pos.x) / 2;
    block_pos.y = start_pos.y + STAGE_HIGHT / 2;
    block_pos.z = (end_pos.z + start_pos.z) / 2;

    //テクスチャーの倍率設定
    VECTOR block_rate;

    block_rate.x = (end_pos.x - start_pos.x);
    block_rate.y = 150.0f;
    block_rate.z = (end_pos.z - start_pos.z);

    //ブロックの外見
    MV1SetScale(move_block_handle, VGet(block_rate.x, block_rate.y, block_rate.z));   //モデルの大きさを変えます
    MV1SetRotationXYZ(move_block_handle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(move_block_handle, VGet(block_pos.x, block_pos.y, block_pos.z));
    MV1DrawModel(move_block_handle);
}

void MoveBlock::Exit()
{
    MV1DeleteModel(move_block_handle);
}
