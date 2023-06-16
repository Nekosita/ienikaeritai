#include"../src/Game/Object/BlockBase.h"
#pragma once

class Player;

//! ステージのクラス
//===============================================
class StageBlock : public BlockBase
{
private:

	int stg_block_num;//!<作成するブロック用の番号
	int block_handle ;//!<ブロック用のモデル

	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して
	
public:

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	StageBlock(size_t block_num);
	~StageBlock();


	void Update_player();
    //void Update_npc();

	void Render();
	
	void Exit();

};
