﻿#pragma once
#include "Matrix4.h"
/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 {
  public:
	float x; // x成分
	float y; // y成分
	float z; // z成分

  public:

	// コンストラクタ
	Vector3();                          // 零ベクトルとする
	Vector3(float x, float y, float z); // x成分, y成分, z成分 を指定しての生成
	  
	//メンバ関数
	float length() const;
	Vector3& normalize();

	// 単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
	Vector3& operator*(float s);
	Vector3& operator+(float s);
	Vector3& operator*(const Matrix4& m);

	const Vector3 lerp(const Vector3& start, const Vector3& end, const float Timer);

};
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(float s, const Vector3& v1);