#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


class BossPhase_1
{
public:// メンバ関数
	// 初期化関数
	void Initialize(Model* model);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:// メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_[27];

	//モデル
	Model* model_[27];

	//デスフラグ
	bool isDead_ = false;

};

