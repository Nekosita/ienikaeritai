#include"Item.h"
#include"../Base.h"
#include"DxLib.h"


// コンストラクタ
//----------------------------------------------------
Item::Item(size_t item_num)
{

	model_handle = MV1LoadModel("Data/Item/Item.mqoz");      //モデル実装
	vertex_shader = LoadVertexShader("shader/SampleVS.vso"); 
	pixel_shader = LoadPixelShader("shader/SamplePS.pso");	

	//位置パラメータの用意
	constexpr VECTOR ITEM_POS_LIST[] = {
		{ -15.0f * 200.0f, 1200.0f,   6.0f * 200.0f },
		{ -18.0f * 200.0f, 4700.0f, -18.0f * 200.0f },
		{ -17.0f * 200.0f, 5500.0f,  28.0f * 200.0f },
		{  16.0f * 200.0f, 5300.0f,   6.0f * 200.0f },
		{ -10.0f * 200.0f, 5600.0f, -10.0f * 200.0f },
		{ -13.0f * 200.0f, 5500.0f,  17.0f * 200.0f },
		{  -5.0f * 200.0f, 1200.0f,    -10 * 200.0f },
		{  28.0f * 200.0f, 8200.0f,  10.0f * 200.0f },
		{  15.0f * 200.0f, 9700.0f,   3.0f * 200.0f },
		{   1.0f * 200.0f, 11000.0f, 10.0f * 200.0f }
	};

	//item_numが配列外の場合、0にする
	if (item_num < 0 || item_num >= 10){
		item_num = 0;
	}

	pos = ITEM_POS_LIST[item_num]; //座標設定

	SetIsPicked(false); //拾ってない
	SetRadius(200.0f);  //ロットY
	rot.y = 180.0f;     //初期角度

	move_count = TO_RADIAN(90.0f);
}
//デストラクタ
//-----------------------------------------------
Item::~Item()
{
	Exit();
}
void Item::SetIsPicked(bool now_picked)
{
	is_picked = now_picked;
}
bool Item::GetIsPicked() const
{
	return is_picked;
}
//アップデート
//-----------------------------------------------
void Item::Update()
{
	rot.y += TO_RADIAN(1.0f);

	move_count += TO_RADIAN(3.0f);

	pos.y += sinf(move_count)*1.5f;
}
//描画
//-----------------------------------------------
void Item::Render()
{

	//シェーダを使うー
	MV1SetUseOrigShader(TRUE);
	//使用する頂点シェーダーのセット
	SetUseVertexShader(vertex_shader);
	//使用するピクセルシェーダーをセット
	SetUsePixelShader(pixel_shader);
	//モデルの大きさを設定
	MV1SetScale(model_handle, VGet(30.0f, 30.0f, 30.0f));

	//モデルの描画
	MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
	MV1SetPosition(model_handle, pos);
	MV1DrawModel(model_handle);

	//シェーダを閉じる
	MV1SetUseOrigShader(FALSE);

}
void Item::Exit()
{
	//読み込んだ頂点シェーダーの削除
	DeleteShader(vertex_shader);
	//読み込んだピクセルシェーダーの削除
	DeleteShader(pixel_shader);
	MV1DeleteModel(model_handle);
}
