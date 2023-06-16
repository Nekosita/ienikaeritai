#include "../src/Game/Object/BlockBase.h"
#pragma once


//! スローエリアクラス
//===============================================
class Npc_Sheltter : public BlockBase
{
private:

    int Sheltter_model;

    VECTOR sheltterpos;

public:


    Npc_Sheltter();
   ~Npc_Sheltter();

   VECTOR Getsheltterpos() const { return sheltterpos; }

    void Update();
    void Render();
    void Exit();
};
