#include "../src/Game/Object/BlockBase.h"
#pragma once

class Player;

//! スローエリアクラス
//===============================================
class CkeckArea : public BlockBase
{
private:
    VECTOR move_vector = {0.0f, 0.0f, 0.0f};
    
  bool player_is_in_area;//!<1回用

  const Player* player_ptr = nullptr;   //!<プレイヤーを宣言して

public:

   void SetPlayer(Player* p) { player_ptr = p; }

    CkeckArea();
    ~CkeckArea();

    bool Get_player_is_in_area() const { return player_is_in_area; }


    void Update();
    void Render();
    void Exit();
};
