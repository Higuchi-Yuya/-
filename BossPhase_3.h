#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
class BossPhase_3
{
public:
	// 初期化関数
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection viewprojection);
private:// メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_[19];

	// 消すときのフラグ
	bool RespawnFlag[27];

	DebugText* debugText_ = nullptr;


	//モデル
	Model* model_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	// インプット
	Input* input_ = nullptr;

	float speedX=0.1;

	float totalMoveX = 1;
};

