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
	worldTransform_[18].translation_ = { 0,-2, 0 };
	worldTransform_[19].translation_ = { +2,-2,-2 };
	worldTransform_[20].translation_ = { 0,-2,-2 };
	worldTransform_[21].translation_ = { -2,-2,-2 };
	worldTransform_[22].translation_ = { -2,-2, 0 };
	worldTransform_[23].translation_ = { -2,-2,+2 };
	worldTransform_[24].translation_ = { 0,-2,+2 };
	worldTransform_[25].translation_ = { +2,-2,+2 };
	worldTransform_[26].translation_ = { +2,-2, 0 };
}

void BossPhase_2::Update(Vector3 playerPos)
{
}

void BossPhase_2::Draw(ViewProjection viewprojection)
{
}

void BossPhase_2::beamUpdate(Vector3 playerPos)
{
}

void BossPhase_2::boomerangUpdate(Vector3 playerPos)
{
}
