#include"ModelBase.h"
#pragma once


//! Itemクラス
//===============================================
class Item :public ModelBase
{
private:
// メンバ変数
//===============================================

	int vertex_shader;	//!<シェーダー設定	
	int pixel_shader;	//!<ピクセル設定
	float move_count;	//!<ふわふわ演出用
	bool is_picked;		//!<拾うようの変数	
	float radius;		//!<拾う用半径

public:
// メンバ関数
//===============================================

	//!コンストラクタ
	Item(size_t item_num);
	//!デストラクタ
	~Item();			  

	//!拾う変数の設定
	void SetIsPicked(bool now_picked);
	//!拾う変数の獲得
	bool GetIsPicked() const;		  
	//!更新処理
	void Update();		
	//!描画処理
	void Render();		
	//!終了処理
	void Exit();		
};

