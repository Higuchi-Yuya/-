#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "affine.h"
#include "DebugText.h"

class BossBullet
{
public:
	// 初期化
	void Initialize(Model* model, const WorldTransform worldTransform,const Vector3& OyaPos, const Vector3& rotaAngle);

	// 更新処理
	void Update(const Vector3& playerPos);

	// 描画処理
	void Draw(const ViewProjection& viewProjection);

	// デスフラグのゲッター
	bool IsDead() const { return isDead_; }

	// ワールド弾座標を取得
	Vector3 GetBulletWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// ブロックを飛ばす処理
	void FlyBlocks(Vector3 playerPos);

	// プレイヤーの位置に行った時のフラグのゲッター
	bool GetToPlayerFlag() const { return ToPlayerFlag_; }

	void SetToPlayerFlag(bool ToPlayerFlag);

	inline void SetPos(const Vector3& pos) {worldTransform_.translation_ = pos;}

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 3;

	//デスタイマー
	int32_t deathTimer_ = 0;

	//デスフラグ
	bool isDead_ = false;

	// ブロックを浮かし終わるまでのフラグ
	bool FloatBlockFlagM = false; // 座標をマイナス
	bool FloatBlockFlagP = false; // 座標をプラス
	bool FloatXRimitFlag = false; // 横に出すときの制限フラグ]


	// 飛ばしたブロックプレイヤーに向かって回転するフラグ
	bool flyToPlayerFlag = false;

	// ブロックを動かす前の位置
	Vector3 oldPos;
	Vector3 oyaPos;
	Vector3 playerPos;

	DebugText* debugText_ = nullptr;

	// プレイヤーの所に至った時のフラグ
	bool ToPlayerFlag_ = false;
};

