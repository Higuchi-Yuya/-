#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>

class BossPhase_2
{

public:// サブクラス
	enum class Motion {
		standby,
		beam,
		boomerang,
		rush,
		reflector,
	};
public:// メンバ関数
	// 初期化関数
	void Initialize();

	// 更新処理
	void Update(Vector3 playerPos);

	// 描画処理
	void Draw(ViewProjection viewprojection);

private:
	void beamUpdate(Vector3 playerPos);

	void boomerangUpdate(Vector3 playerPos);

	// 行列の転送と更新
	void TransferMat();
private:// メンバ変数
	//ワールド変換データ
	WorldTransform worldTransform_[19];

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	// インプット
	Input* input_ = nullptr;

	//モデル
	Model* model_ = nullptr;


};

