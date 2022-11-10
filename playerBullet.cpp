#include "playerBullet.h"
#include"playerBullet.h"
#include<cassert>
#include"affine.h"

void playerBullet::Initialize( const Vector3& Position,Vector3 Rot, const Vector3& velocity)
{

	model_=Model::Create();
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = Position;

	worldTransform_.rotation_ = Rot;

	velocity_ = velocity;
}
void playerBullet::Update()
{
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	affine::makeMatIdentity(worldTransform_.matWorld_);
	affine::makeMatRot(worldTransform_.matWorld_, worldTransform_.rotation_);
	affine::makeMatTrans(worldTransform_.matWorld_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}
void playerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
void playerBullet::OnCollision()
{
	isDead_ = true;
}
Vector3 playerBullet::GetworldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldpos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldpos.x = worldTransform_.translation_.x;
	worldpos.y = worldTransform_.translation_.y;
	worldpos.z = worldTransform_.translation_.z;

	return worldpos;
}