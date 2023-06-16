#include"BlockBase.h"
#pragma once

class Player;
class Npc;
class Boss;

//===============================================
//! ゴールクラス
//===============================================
class GoalArea :public BlockBase
{
private:

	static constexpr float stage_hight = 150.0f;

	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して
	const Npc* npc_ptr = nullptr;   //!<Npcーを宣言して       
	const Boss* boss_ptr = nullptr;   //!<Bossーを宣言して

    //----------------------------------------------------------
    //!	@name	CLEAR演出用
    //----------------------------------------------------------
    //!@{
    int    clear_arrow;   //クリアポイントの所
    VECTOR clear_area_start;
    VECTOR clear_area_end;
    VECTOR clear_arrow_vector;
    int    clear_arrow_count;

    //!@}

public:

	void SetPlayer(Player* p) {player_ptr = p;}
	void SetNpc(Npc* n) {npc_ptr = n;}
    void SetBoss(Boss* b) {boss_ptr = b;}


	//!△の当たり判定用end_posを弄って最も上のposを探す
	float GetGoalAreaHight() const;

	GoalArea();
	~GoalArea();

    //VECTOR GetGoalPos() const; 
	//まだ整理していない
    VECTOR GetClearAreaStart() const { return clear_area_start; }
    VECTOR GetClearAreaEnd() const { return clear_area_end; }


	void clear_arror_update();
    void Render_clear_arror();

    //void Render();
	//void Update();
	void Exit();

};

