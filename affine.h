#pragma once

#include "WorldTransform.h"
//#include"PrimitiveDrawer.h"

namespace affine
{
	//度からラジアンに変換する定数
	constexpr float Deg2Rad = 3.1415926535897931f / 180.0f;
	constexpr float PIHalf = 3.1415926535897931f / 2.0f;

	//単位行列
	void makeMatIdentity(Matrix4& matrix);

	//アフィン変換拡縮
	void makeMatScale(Matrix4& matrix, Vector3 scale);

	//アフィン変換回転
	void makeMatRot(Matrix4& matrix, Vector3 rot);

	//アフィン変換平行移動
	void makeMatTrans(Matrix4& matrix, Vector3 Trans);

	void makeAffine(WorldTransform& worldTransform);

	//ベクトルと行列の掛け算
	Vector3 MatVector(Matrix4 matrix, Vector3 vector);

	Vector3 Mat3D(Matrix4 matrix, Vector3 vector);
	//ワールド座標を返す
	Vector3 GetWorldTrans(Matrix4 matrix);

	Vector3 wdivision(Matrix4 matrix, Vector3 vector);
}
