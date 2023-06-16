#pragma once
#include"../src/Game/Object/BlockBase.h"

//!プレイヤーはクラスで伝えます
class Player;

//===============================================
//! ムーヴクラス
//===============================================
class MoveBlock :public BlockBase
{
private:

	int move_block_num;//!<作成されるムーヴブロックのナンバー
	int move_block_handle;//!<移動ブロック用のモデル
	int  block_count;   //!<落ちた時のカウンター
	const Player* player_ptr = nullptr;//!<プレイヤーを宣言して
	VECTOR move_vector;

	int move_count;//前の二つブロック用のカウンター
    int move_count_long;   //後ろにいる遅い方用のカウンター

	//移動ブロック用の暫定変数
	VECTOR move_vector_block2;
    VECTOR move_vector_block3;
    VECTOR move_vector_block4;

public:

    MoveBlock(size_t block_num);
    virtual ~MoveBlock();

	void SetPlayer(Player* p) {
		player_ptr = p;
	}

	VECTOR GetMoveVector() const;

	void domino_move_down();
	
    //!更新処理
    void Update_player();

    void Move(bool T1, bool T2 ,bool G1, bool M1);
    //!終了処理
	void Render();
    void Exit();
};

