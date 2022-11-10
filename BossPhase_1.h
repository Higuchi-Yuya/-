#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
class BossPhase_1
{
public:// メンバ関数
	// 初期化関数
	void Initialize(Model* model);

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection viewprojection);

	// ブロックを飛ばす処理
	void FlyBlocks(Vector3 playerPos);

	// ブロックをランダムに抽選して浮かせる処理
	void FloatRandomBlocks();

private:// メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_[27];

	// 消すときのフラグ
	bool RespawnFlag[27];

	DebugText* debugText_ = nullptr;


	//モデル
	Model* model_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	// インプット
	Input* input_ = nullptr;

	// ランダムにとるための変数
	int randomBlock = 0;

	// 再抽選するフラグ
	bool ReLotteryFlag = true;

	// ブロックを浮かし終わるまでのフラグ
	bool FloatBlockFlagM = false; // 座標をマイナス
	bool FloatBlockFlagP = false; // 座標をプラス
	bool FloatXRimitFlag = false; // 横に出すときの制限フラグ

	// ブロックを動かす前の位置
	Vector3 oldPos;

	// 飛ばしたブロックのリスポーンフラグ
	bool randomRespawnFlag = false;

	// 飛ばしたブロックプレイヤーに向かって回転するフラグ
	bool flyToPlayerFlag = false;

	// ベクトル
	Vector3 velocity;
};

