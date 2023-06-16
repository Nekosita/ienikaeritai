#include "../src/Game/Object/BlockBase.h"
#pragma once

class Player;

//! スローエリアクラス
//===============================================
class UpArea : public BlockBase
{
private:
    VECTOR move_vector = {0.0f, 0.0f, 0.0f};
    
    
    int up_block_handle;

public:
    UpArea(size_t _num);
    ~UpArea();

    void Update(bool player_npc_is_on);
    void Render();
    void Exit();
};
