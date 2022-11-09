#include "BossPhase_1.h"

void BossPhase_1::Initialize(Model* model)
{
	assert(model);

	input_ = Input::GetInstance();

	model_ = Model::Create();

	for (int i = 0; i < 27; i++) {

		worldTransform_[i].Initialize();
		// フラグ
		RespawnFlag[i] = false;
	}

	

	// 親
	worldTransform_[0].translation_ = { 0,0,0 };



	// 子の座標設定

	// 真ん中の段
	worldTransform_[1].translation_ = { +2, 0,-2 };
	worldTransform_[2].translation_ = { 0, 0,-2 };
	worldTransform_[3].translation_ = { -2, 0,-2 };
	worldTransform_[4].translation_ = { -2, 0, 0 };
	worldTransform_[5].translation_ = { -2, 0,+2 };
	worldTransform_[6].translation_ = { 0, 0,+2 };
	worldTransform_[7].translation_ = { +2, 0,+2 };
	worldTransform_[8].translation_ = { +2, 0, 0 };
	// 上の段
	worldTransform_[9].translation_ = { 0,+2, 0 };
	worldTransform_[10].translation_ = { +2,+2,-2 };
	worldTransform_[11].translation_ = { 0,+2,-2 };
	worldTransform_[12].translation_ = { -2,+2,-2 };
	worldTransform_[13].translation_ = { -2,+2, 0 };
	worldTransform_[14].translation_ = { -2,+2,+2 };
	worldTransform_[15].translation_ = { 0,+2,+2 };
	worldTransform_[16].translation_ = { +2,+2,+2 };
	worldTransform_[17].translation_ = { +2,+2, 0 };
	// 下の段
	worldTransform_[18].translation_ = { 0,-2, 0 };
	worldTransform_[19].translation_ = { +2,-2,-2 };
	worldTransform_[20].translation_ = { 0,-2,-2 };
	worldTransform_[21].translation_ = { -2,-2,-2 };
	worldTransform_[22].translation_ = { -2,-2, 0 };
	worldTransform_[23].translation_ = { -2,-2,+2 };
	worldTransform_[24].translation_ = { 0,-2,+2 };
	worldTransform_[25].translation_ = { +2,-2,+2 };
	worldTransform_[26].translation_ = { +2,-2, 0 };

	// 子
	for (int i = 1; i < 27; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}
}

void BossPhase_1::Update()
{
	// キューブの移動
	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT)) {
		if (input_->PushKey(DIK_UP)) { worldTransform_[0].translation_.y += 1.0f; }
		else if (input_->PushKey(DIK_DOWN)) { worldTransform_[0].translation_.y -= 1.0f; }
		if (input_->PushKey(DIK_RIGHT)) { worldTransform_[0].translation_.x += 1.0f; }
		else if (input_->PushKey(DIK_LEFT)) { worldTransform_[0].translation_.x -= 1.0f; }
	}

	FlyBlocks({ 0,0,15 });

	// 行列の更新と転送
	for (int i = 0; i < 27; i++) {
		affine::makeAffine(worldTransform_[i]);
	}
	for (int i = 1; i < 27; i++) {
		worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
	}
	for (int i = 0; i < 27; i++) {
		worldTransform_[i].TransferMatrix();
	}
}

void BossPhase_1::Draw(ViewProjection viewprojection)
{
	for (int i = 0; i < 27; i++) {
		if (RespawnFlag[i] == false)
		{
			model_->Draw(worldTransform_[i], viewprojection);
		}
	}
}

void BossPhase_1::FlyBlocks(Vector3 playerPos)
{
	FloatRandomBlocks();

	// プラスフラグ
	if (FloatBlockFlagP == true) {
		worldTransform_[randomBlock].translation_.x += 0.1f;

		if (worldTransform_[randomBlock].translation_.x >= oldPos.x + 10.0f) {
			FloatBlockFlagP = false;
			//お試しリスポーン
			randomRespawnFlag = true;
			worldTransform_[randomBlock].translation_.x = oldPos.x + 10.0f;
			RespawnFlag[randomBlock] = true;
		}
	}


	// マイナスフラグ
	if (FloatBlockFlagM == true) {
		worldTransform_[randomBlock].translation_.x -= 0.1f;

		if (worldTransform_[randomBlock].translation_.x <= oldPos.x - 10.0f) {
			FloatBlockFlagM = false;
			//お試しリスポーン
			randomRespawnFlag = true;
			worldTransform_[randomBlock].translation_.x = oldPos.x - 10.0f;
			RespawnFlag[randomBlock] = true;
		}
	}
	


}

void BossPhase_1::FloatRandomBlocks()
{
	// 拒否してるブロック以外だったら再抽選を終わらせる
	while (randomBlock == 0 || randomBlock == 2 || randomBlock == 11 || randomBlock == 20 || randomBlock == 6 || randomBlock == 15 || randomBlock == 24) {
		randomBlock = rand() % 26 + 1;
		oldPos = worldTransform_[randomBlock].translation_;
	}

	// 再抽選が終わったら
	if (worldTransform_[randomBlock].translation_.x > worldTransform_[0].translation_.x) {
		FloatBlockFlagP = true;
	}
	else if (worldTransform_[randomBlock].translation_.x < worldTransform_[0].translation_.x) {
		FloatBlockFlagM = true;
	}

}
