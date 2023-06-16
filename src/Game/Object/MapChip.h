#pragma once

//===============================================
//! マップクラス
//===============================================
class MapChip
{
private:

	static constexpr int SWAMP_COUNT = 120;

	int water_handle;//!<モデル実装
	int count;
	float swamp_x;	//!<流れ用

public:

	//!コンストラクタ
	MapChip();	   
	//!コンストラクタ
	~MapChip();	   

	void BuildMap();
	void Update();
	//!地図作成
	void DrawMap();				 
};
namespace
{

}	 // namespace
