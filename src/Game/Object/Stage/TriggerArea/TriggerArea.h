#include"../src/Game/Object/BlockBase.h"
#pragma once

class Player;


//! トラップクラス
//===============================================
class TriggerArea : public BlockBase
{
private:

	int trap_num;//!<番号設定
	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して

	int sayu_block_handle;

public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	TriggerArea(size_t _num);
    ~TriggerArea();

	void Render();
	void Update();
	void Exit();

};

