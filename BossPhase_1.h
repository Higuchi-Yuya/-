#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
#include "BossBullet.h"

class BossPhase_1
{

public:// メンバ関数
	// 初期化関数
	void Initialize();

	// 更新処理
	void Update(Vector3 playerPos);

	/// <summary>
	/// タイトルシーン～ゲームシーンへ遷移するようの更新処理
	/// </summary>
	void TitleUpdate();

	// 描画処理
	void Draw(ViewProjection viewprojection);

	WorldTransform GetPos() { return worldTransform_[0]; }

	//衝突を検出したら呼び出されたるコールバック関数
	void OnCollision();

	std::unique_ptr<BossBullet>& GetBullet() { return bullet; }

	WorldTransform GetWorldTransformP() const { return worldTransform_[0]; }

private:// 静的メンバ関数

	// ブロックを飛ばす処理
	void FlyBlocks(Vector3 playerPos);

	// ブロックをランダムに抽選して浮かせる処理
	void FloatRandomBlocks();

	// 弾を打つ関連の初期化
	void ResetFlyBlocks();

	// 行列の転送と更新
	void TransferMat();

	void TurnBodyToPlayer(Vector3 playerPos);



private:// メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_[27];

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	// インプット
	Input* input_ = nullptr;

	//モデル
	Model* model_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	// 消すときのフラグ
	bool AnnihilationFlag[27];

	// 弾だったものをもう一度出すフラグ
	bool RespawnFlag[27];

	// ランダムにとるための変数
	int randomBlock = 0;

	// ブロックを浮かし終わるまでのフラグ
	bool FloatBlockFlagM = false; // 座標をマイナス
	bool FloatBlockFlagP = false; // 座標をプラス
	
	// 押し出すときの角度
	Vector3 rotaAngle;

	// ブロックを動かす前の位置
	Vector3 oldPos;

	// ボスバレット
	std::unique_ptr<BossBullet> bullet;

	// 徐々に拡大する時のスケール
	Vector3 expansionScale;

	// 徐々に拡大するときのスピード
	Vector3 expansionScaleSpeed = { 0.05f,0.05f,0.05f };

	//タイトル挙動用回転角
	float titleRadian = 0;
};

