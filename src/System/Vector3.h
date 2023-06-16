#include"../src/Game/Main.h"

#pragma once

//---------------------------------------------------------------------------------
//!Vector3 クラス（ x y z があるクラス）
//---------------------------------------------------------------------------------
class Vector3
{
public:

	float	x;
	float	y;
	float	z;

	Vector3();									//!<コンストラクタ
	Vector3(float x, float y, float z);
	Vector3(VECTOR v);

	void clear();								//!<ゼロ初期化
	void set(float x, float y, float z);		//!<値のセット
	void set(Vector3& v);

	float GetLength();							//!<長さ取得
	void normalize();							//!<正規化
	void SetLength(float length);				//!<長さ設定

	Vector3& operator = (const Vector3& v);	//!< = 演算子のオーバーロード

	Vector3& operator += (const Vector3& v);	//!< += 演算子のオーバーロード
	Vector3& operator -= (const Vector3& v);	//!< -= 演算子のオーバーロード
	Vector3& operator *= (const float f);		//!< *= 演算子のオーバーロード
	Vector3& operator /= (const float f);		//!< /= 演算子のオーバーロード

	VECTOR VGet();								//!< DXライブラリで使う VECTOR を返す
	void VSet(VECTOR v);						//!< DXライブラリで使う VECTOR の値を取得して設定
	FLOAT4 GetFLOAT4();							//!< DXライブラリで使う FLOAT4 を返す
};

//!<  + 演算子のオーバーロード
Vector3 operator + (const Vector3& v0, const Vector3& v1);
//!<  - 演算子のオーバーロード
Vector3 operator - (const Vector3& v0, const Vector3& v1);
//!<  * 演算子のオーバーロード
Vector3 operator * (const Vector3& v, const float f);
//!< / 演算子のオーバーロード
Vector3 operator / (const Vector3& v, const float f);

//!< ２つの Vector2 の距離を求める
float GetVector3Distance(Vector3& p0, Vector3& p1);
//!< ２つの Vector3 の内積を求める
float GetVector3Dot(Vector3& v0, Vector3& v1);
//!< ２つの Vector3 の外積を求める
Vector3 GetVector3Cross(Vector3& v0, Vector3& v1);
//!< 反射ベクトルの取得
Vector3 GetVector3Reflect(Vector3& base, Vector3& normal);
