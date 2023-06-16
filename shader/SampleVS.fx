#pragma pack_matrix(row_major)

#include "dxlib_vs.h"

// [DxLib] MV1モデルの頂点シェーダー入力
struct VS_INPUT_MV1
{
	float3 position_      : POSITION;		// 座標(ローカル空間)
	float3 normal_        : NORMAL;			// 法線(ローカル空間)
	float4 diffuse_       : COLOR0;			// ディフューズカラー
	float4 specular_      : COLOR1;			// スペキュラカラー
	float4 uv0_           : TEXCOORD0;		// テクスチャ座標
	float4 uv1_           : TEXCOORD1;		// サブテクスチャ座標
};

//---------------------------------------------------------------------------------
//	頂点シェーダー
//---------------------------------------------------------------------------------
/*
//	受け取る各頂点の情報
struct VS_INPUT
{
	float4 LPos		: POSITION;		//	ローカル座標
	float3 LNormal	: NORMAL;		//	ローカル法線
	float4 Color	: COLOR;		//	色
};
*/


//	ピクセルシェーダーに渡す情報
struct VS_OUTPUT
{
	float4 SPos		: SV_Position;		//	画面座標
	float4 Color	: COLOR;		//	色

	//ピクセルシェーダーで明るさ計算をしたいので
	float3 WPos		: TEXCOORD1;//ワールド座標
	float3 WNormal	: TEXCOORD2;//ワールド法線
};
/*
float4x4 World		: register(c94);	//	ワールド変換行列
float4x4 View		: register(c6);	//	ビュー変換行列
float4x4 Projection	: register(c2);	//	プロジェクション変換行列
*/
//---------------------------------------------------------------------------------
//！各頂点の処理
//---------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT_MV1 In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float3	worldPosition = mul(DxLib_Base.WorldMatrix, float4(In.position_, 1.0));	// スキニング計算。ワールド空間へ変換
	float3	viewPosition = mul(DxLib_Base.ViewMatrix, float4(worldPosition, 1.0));	// ビュー空間へ変換
	Out.SPos = mul(DxLib_Base.ProjectionMatrix, float4(viewPosition, 1.0));	// スクリーン空間へ変換

	//	ワールド法線を計算
//	float3 N = normalize(mul(DxLib_Base.WorldMatrix, float4(In.normal_, 0.0)));
	float3 N = In.normal_;
	
	//色を渡す
	Out.Color = float4(1.0f, 0.5f, 0.5f, 0.8f);
	//Out.Color = In.Color;

	//ワールド座標をピクセルシェーダーに渡す
	Out.WPos = worldPosition;

	//ワールド法線をピクセルシェーダーに渡す
	Out.WNormal = N;

	return Out;
}
