#pragma pack_matrix(row_major)

//---------------------------------------------------------------------------------
//	ピクセルシェーダー
//---------------------------------------------------------------------------------

//受け取る各ピクセルの情報
struct PS_INPUT
{
	float4 SPos		: SV_Position;		//	画面座標
	float4 Color	: COLOR;		//	色

	//ピクセルシェーダーで明るさ計算をしたいので
	float3 WPos		: TEXCOORD1;//ワールド座標
	float3 WNormal	: TEXCOORD2;//ワールド法線
};

//計算した結果、画面に表示される色
struct PS_OUTPUT
{
	float4 Color	: SV_Target0;		//	色
};

//---------------------------------------------------------------------------------
//各ピクセルの処理
//---------------------------------------------------------------------------------
PS_OUTPUT main( PS_INPUT In )
{

	//ライト座標
	float4 LightPos	= { -2.0f, 5.0f, 0.0f, 0.0f };

	PS_OUTPUT Out;

	Out.Color = In.Color;

	//各ピクセルからライトまでのベクトル
	float3 dir = LightPos.xyz - In.WPos;

	//そのベクトルのお正規化
	float3 L = normalize( dir );

	//ワールド法線も正規化
	float3 N = normalize( In.WNormal );

	//そのピクセルがどのぐらいライトの方を向いているか
	float power = max( 0.0f, dot( N, L ) );

	//上で求めるた強さは0.0ｆ～1.0ｆなのですか
	//これを3段階
	//に分けてみます
	if (power < 0.2f) {
		power = 0.2f;
	}else if (power < 0.7f) {
		power = 0.5f;
	}else {
		power = 0.8f;
	}

	//その強さの明るさにします
	Out.Color = In.Color * power*50;

	//Out.Color.g = 1.0f;
	Out.Color.a = 1.0f;

	Out.Color = float4(N, 1);

	//Out.Color = float4(power.xxx, 1);
	return Out;
}
