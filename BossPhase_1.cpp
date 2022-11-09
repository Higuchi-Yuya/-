#include "BossPhase_1.h"

void BossPhase_1::Initialize(Model* model)
{
	for (int i = 0; i < 27; i++) {
		model_[i] = model;
	}
	// 親
	worldTransform_[0].translation_ = { 0,0,0 };

	// 子
	for (int i = 1; i < 27; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}

	// 子の座標設定
	worldTransform_[1].translation_ = { -2, 0,+2 };
	worldTransform_[2].translation_ = { -2, 0, 0 };
	worldTransform_[3].translation_ = { -2, 0,-2 };
	worldTransform_[4].translation_ = {  0, 0,-2 };
	worldTransform_[5].translation_ = { -2, 0,+2 };
	worldTransform_[6].translation_ = {  0, 0,+2 };
	worldTransform_[6].translation_ = { +2, 0,+2 };
	worldTransform_[7].translation_ = { +2, 0, 0 };
}

void BossPhase_1::Update()
{

}

void BossPhase_1::Draw()
{
	
}
