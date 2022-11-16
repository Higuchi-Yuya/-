#include "BossBullet.h"

void BossBullet::Initialize(Model* model, const WorldTransform worldTransform, const Vector3& OyaPos, const Vector3& rotaAngle)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	//引数で受け取ったものをメンバ変数に代入
	oyaPos = OyaPos;
	oldPos = worldTransform.translation_;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.x =worldTransform.matWorld_.m[3][0];
	worldTransform_.translation_.y =worldTransform.matWorld_.m[3][1];
	worldTransform_.translation_.z =worldTransform.matWorld_.m[3][2];

	worldTransform_.rotation_ = rotaAngle;

	debugText_ = DebugText::GetInstance();

	FloatBlockFlagM = false;
	FloatBlockFlagP = false;
	FloatXRimitFlag = false;

	flyToPlayerFlag = false;
	ToPlayerFlag_ = false;
}

void BossBullet::Update(const Vector3& playerPos)
{
	// 座標を移動させる(１フレーム分の移動量を足しこむ)
	FlyBlocks(playerPos);
	

	affine::makeAffine(worldTransform_);
	worldTransform_.TransferMatrix();
}

void BossBullet::Draw(const ViewProjection& viewProjection)
{
	if (ToPlayerFlag_ == false) {
		model_->Draw(worldTransform_, viewProjection);
	}
}

Vector3 BossBullet::GetBulletWorldPosition()
{
	return Vector3();
}

void BossBullet::OnCollision()
{
}

void BossBullet::FlyBlocks(Vector3 playerPos)
{
	if (oyaPos.x < worldTransform_.translation_.x) {
		FloatBlockFlagP = true;
	}
	else if (oyaPos.x > worldTransform_.translation_.x) {
		FloatBlockFlagM = true;
	}

	// プラスフラグ
	if (FloatBlockFlagP == true && FloatXRimitFlag == false) {
		worldTransform_.translation_.x += 0.1f;

		if (worldTransform_.translation_.x >= oldPos.x + 10.0f) {
			// 制限に移動したらフラグをオン
			FloatXRimitFlag = true;
			//お試しリスポーン
			flyToPlayerFlag = true;
			worldTransform_.translation_.x = oldPos.x + 10.0f;

			// プレイヤーに向かうベクトルの計算
			velocity_.x = playerPos.x - worldTransform_.translation_.x;
			velocity_.y = playerPos.y - worldTransform_.translation_.y;
			velocity_.z = playerPos.z - worldTransform_.translation_.z;

			velocity_.normalize();
			velocity_ *= 0.3f;
		}
	}

	// マイナスフラグ
	if (FloatBlockFlagM == true && FloatXRimitFlag == false) {
		worldTransform_.translation_.x -= 0.1f;

		if (worldTransform_.translation_.x <= oldPos.x - 10.0f) {
			// 制限に移動したらフラグをオン
			FloatXRimitFlag = true;
			//お試しリスポーン
			flyToPlayerFlag = true;
			worldTransform_.translation_.x = oldPos.x - 10.0f;

			// プレイヤーに向かうベクトルの計算
			velocity_.x = playerPos.x - worldTransform_.translation_.x;
			velocity_.y = playerPos.y - worldTransform_.translation_.y;
			velocity_.z = playerPos.z - worldTransform_.translation_.z;

			velocity_.normalize();
			velocity_ *= 0.3f;
		}
	}
	if (ToPlayerFlag_ == false) {
		// プレイヤーに向かって飛ぶ処理
		if (flyToPlayerFlag == true) {
			// 回転しながら飛んでいく
			worldTransform_.rotation_.z += 0.5f;

			// プレイヤーに向かって飛ぶ
			worldTransform_.translation_ += velocity_;

			// デスタイマーの加算
			deathTimer_++;

			if (worldTransform_.translation_.y <= 0.0f ||
				worldTransform_.translation_.y >= 30.0f||
				deathTimer_>=kLifeTime) {
				flyToPlayerFlag = false;
				ToPlayerFlag_ = true;
				deathTimer_ = 0;
			}
			
		}
	}
	
	/*debugText_->SetPos(20, 60);
	debugText_->Printf("飛ぶやつ:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);*/
}

void BossBullet::SetToPlayerFlag(bool ToPlayerFlag)
{
	ToPlayerFlag_ = ToPlayerFlag;
}


