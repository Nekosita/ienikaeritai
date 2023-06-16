#include"../src/System/Vector3.h"
#pragma once

class Vector3;
class Camera;
class Player;
class Boss;
class Npc;

//! ユーザーインターフェースクラス
//-----------------------------------------------
class UserInterface
{
private:

	int time_count;//!<タイム
	int time_count_flame;//!<タイム計測
	int num;//!<現在のアイテム取得数	
	bool item_complete;	//!<アイテム全て揃った時
	int item_complete_counter;//!<アイテム全て揃った時用
	bool hint;//!<プレイヤーがアイテム集め終わって無い時

	int mute_handle;//!<ミュート画像
	int item_num;

	int text_count=0;//!<開幕用のテキスト
	
	const Camera* camera_ptr = nullptr;
    const Player* player_ptr = nullptr; 
    const Npc* npc_ptr = nullptr; 
	const Boss* boss_ptr   = nullptr;

	int keybord_picture_inpause;//!<パスの中の画像

	//Title関係の変数
    int    mouseX, mouseY;
    int    title_handle;
    int    title_handle_2;
    VECTOR title_centerpos;
    int    title_weight;
    int    title_height;
    VECTOR title_startpos;
    VECTOR title_endpos;
	
	bool check_title;
    
    //Skip関係の変数
    int    skip_handle;
    int    skip_handle_2;
    VECTOR skip_centerpos;
    int    skip_weight;
    int    skip_height;
    VECTOR skip_startpos;
    VECTOR skip_endpos;

    	
	bool check_skip;

public:

    //Pos1 は左上の開始座標、Pos_2は右下の終わり座標、Mouse_x,Mouse_yはマウス座標
    bool CheckInSquare(VECTOR Pos_1, VECTOR Pos_2, int Mouse_x, int Mouse_y)
    {
        if((Mouse_x > Pos_1.x) && (Mouse_x < Pos_2.x) && (Mouse_y > Pos_1.y) && (Mouse_y < Pos_2.y)) {
            return true;
        }
        return false;
    }

	void SetCamera(Camera* p) { camera_ptr = p; }
    void SetPlayer(Player* p) { player_ptr = p; }
    void SetNpc(Npc* p) { npc_ptr = p; }
    void SetBoss(Boss* b) { boss_ptr = b; }

	void SetNum(int now_num);
	int Getnum() const;
	void AddNum();


	bool Getcheck() const { return check_title; }
	void SetTimeCount(int now_count);
	int GetTimeCount() const;
	void MinueTimeCount();
	void TimeCountFlame();
	
	void Init();
    void Update();
    void Update_In_end_anime();
    void DrawUi(bool zonbi_dead);
	void DrawGamepauseText(bool music_on, int sound);
    void DrawGameClearText();
	void DrawGameoverText();
};

