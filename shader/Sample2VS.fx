
//---------------------------------------------------------------------------------
//	頂点シェーダー
//---------------------------------------------------------------------------------

//	受け取る各頂点の情報
struct VS_INPUT
{
	float4 LPos		: POSITION;		//	ローカル座標
	float3 LNormal	: NORMAL;		//	ローカル法線
	float4 Color	: COLOR;		//	色
};

//	ピクセルシェーダーに渡す情報
struct VS_OUTPUT
{
	float4 SPos		: POSITION;		//	画面座標
	float4 Color	: COLOR;		//	色
};

float4x4 World		: register(c94);	//	ワールド変換行列
float4x4 View		: register(c6);	//	ビュー変換行列
float4x4 Projection	: register(c2);	//	プロジェクション変換行列

//---------------------------------------------------------------------------------
//	各頂点の処理
//---------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//大きなモデルにして描画したい
	//ローカル座標を変更します
	float4 LPos = In.LPos;

	//法線方向に頂点を移動させる
	LPos.xyz += In.LNormal * 0.03f;

	//座標変換
	float4 WPos = mul(LPos, World);
	float4 CPos = mul(WPos, View);
	Out.SPos = mul(CPos, Projection);


	//Out.Color = In.Color;
	Out.Color = float4(0.2f, 1.0f, 0.2f, 0.5f);

	return Out;

}

