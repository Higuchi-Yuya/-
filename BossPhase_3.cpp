#include "BossPhase_3.h"

void BossPhase_3::Initialize()
{

	model_ = Model::Create();

	debugText_ = DebugText::GetInstance();

	for (int i = 0; i < 19; i++)
	{
		worldTransform_[i].Initialize();
		if (i % 2 != 0)
		{
			if (i % 4 == 1)
			{
				worldTransform_[i].translation_ = Vector3(1.0f, 0.0f, 0.0f + i * 2);
			}
			else if (i % 4 == 3)
			{
				worldTransform_[i].translation_ = Vector3(-1.0f, 0.0f, 0.0f + i * 2);
			}
		}
		else
		{
			worldTransform_[i].translation_ = Vector3(0.0f, 0.0f, 0.0f + i * 2);
		}
		worldTransform_[i].rotation_.y = 180 * affine::Deg2Rad;
	}
}
void BossPhase_3::Update()
{

	WorldTransform oldworldTransform[19];

	for (int i = 0; i < 19; i++)
	{
		oldworldTransform[i].translation_.x = worldTransform_[i].translation_.x;
	}
	for (int i = 1; i < 19; i++)
	{
		worldTransform_[i].translation_.x = oldworldTransform[i-1].translation_.x;
	}

	//自機のワールド座標から移動ベクトルを計算
	Vector3 vectorX = { 1.0,0,0 };
	vectorX = affine::MatVector(worldTransform_[0].matWorld_, vectorX);
	Vector3 vectorZ = { 0,0,1.0 };
	vectorZ = affine::MatVector(worldTransform_[0].matWorld_, vectorZ);

	Vector3 move = { 0,0,0 };

	move.x = speedX * vectorX.x;
	move.z = speedX * vectorX.z;

	worldTransform_[0].translation_.x += move.x;
	worldTransform_[0].translation_.z += move.z;

	totalMoveX += speedX;

	if (totalMoveX >= 2)
	{
		speedX = -0.5;
		totalMoveX = 0;
	}
	if (totalMoveX <= -2)
	{
		speedX = 0.5;
		totalMoveX = 0;
	}

	for (int i = 0; i < 19; i++)
	{
		affine::makeAffine(worldTransform_[i]);
		worldTransform_[i].TransferMatrix();
	}
	/*debugText_->SetPos(10, 50);
	debugText_->Printf("%d,%f", totalMoveX, speedX);*/
}

void BossPhase_3::Draw(ViewProjection viewprojection)
{
	for (int i = 0; i < 19; i++)
	{
		model_->Draw(worldTransform_[i], viewprojection);
	}
}