#include "BossPhase_2.h"

void BossPhase_2::Initialize()
{
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();

	for (int i = 0; i < 19; i++) {

		worldTransform_[i].Initialize();
		
	}

	// 親
	worldTransform_[0].translation_ = { 50,10,50 };
	
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
	worldTransform_[10].translation_ = { 0,+2,-2 };
	worldTransform_[11].translation_ = { -2,+2, 0 };
	worldTransform_[12].translation_ = { 0,+2,+2 };
	worldTransform_[13].translation_ = { +2,+2, 0 };

	// 下の段
	worldTransform_[14].translation_ = { 0,-2, 0 };
	worldTransform_[15].translation_ = { 0,-2,-2 };
	worldTransform_[16].translation_ = { -2,-2, 0 };
	worldTransform_[17].translation_ = { 0,-2,+2 };
	worldTransform_[18].translation_ = { +2,-2, 0 };

	// 子
	for (int i = 1; i < 19; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}
}

void BossPhase_2::Update(Vector3 playerPos)
{
	TransferMat();
}

void BossPhase_2::Draw(ViewProjection viewprojection)
{
	for (int i = 0; i < 19; i++) {
		model_->Draw(worldTransform_[i], viewprojection);
	}
}

void BossPhase_2::beamUpdate(Vector3 playerPos)
{

}

void BossPhase_2::boomerangUpdate(Vector3 playerPos)
{
}

void BossPhase_2::TransferMat()
{
	for (int i = 0; i < 19; i++) {
		affine::makeAffine(worldTransform_[i]);
		if (i != 0) {
			worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
		}
		worldTransform_[i].TransferMatrix();
	}
}
