#include "player.h"
#include"affine.h"
void player::Initialize() {
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	model_ = Model::Create();

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0.0f, 0.0f, 0.0f);
	textureHandle_ = TextureManager::Load("uvChecker.png");

	//3Dレティクルのワールドトランスフォームの初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("2D.png");

	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2{ 640,360 }, Vector4{ 1,1,1,1 }, Vector2(0.5, 0.5)));
}

void player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<playerBullet>& bullet) {
		return bullet->IsDead();
		});

	//自機のワールド座標から移動ベクトルを計算
	Vector3 vectorX = { 1.0,0,0 };
	vectorX = affine::MatVector(worldTransform_.matWorld_, vectorX);
	Vector3 vectorZ = { 0,0,1.0 };
	vectorZ = affine::MatVector(worldTransform_.matWorld_, vectorZ);

	Vector3 move = { 0,0,0 };
	Vector3 rot = { 0,0,0 };
	//ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;
	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 0.1;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 0.1;
	}
	////プレイヤー移動処理
	if (input_->PushKey(DIK_A))
	{
		move.x = -1 * vectorX.x;
		move.z = -1 * vectorX.z;
	}
	if (input_->PushKey(DIK_D))
	{
		move.x = 1 * vectorX.x;
		move.z = 1 * vectorX.z;
	}
	if (input_->PushKey(DIK_W))
	{
		move.x = 1 * vectorZ.x;
		move.z = 1 * vectorZ.z;
	}
	if (input_->PushKey(DIK_S))
	{
		move.x = -1 * vectorZ.x;
		move.z = -1 * vectorZ.z;
	}
	if (input_->PushKey(DIK_Q))
	{
		rot.y = -1;
	}
	if (input_->PushKey(DIK_E))
	{
		rot.y = 1;
	}
	if (input_->PushKey(DIK_Z))
	{
		rot.x = -1;
	}
	if (input_->PushKey(DIK_X))
	{
		rot.x = 1;
	}

	float AR;
	float BR;

	AR = pow(( worldTransform_.translation_.x+move.x)-0, 2) + pow((0 +worldTransform_.translation_.z+move.z), 2);
	BR = pow((100 - worldTransform_.scale_.x*2), 2);

	if (AR <= BR)
	{
		worldTransform_.translation_ += move;
	}

	/*worldTransform_.translation_ += move;*/
	worldTransform_.rotation_.y += rot.y *affine::Deg2Rad;
	worldTransform_.rotation_.x += rot.x * affine::Deg2Rad;
	//移動限界座標
	const float kRotLimit = 30.0 * affine::Deg2Rad;

	//範囲を超えない処理
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x, -kRotLimit);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x, +kRotLimit);

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && jumpFlag == 0)
	{
		jumpFlag = 1;
	}

	jump();

	affine::makeAffine(worldTransform_);
	worldTransform_.TransferMatrix();

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	debugText_->SetPos(10, 10);
	debugText_->Printf("%f,%f",AR,BR);

	debugText_->SetPos(10, 30);
	debugText_->Printf("%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z);
}

void player::Attack() {
	/*XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		return;
	}*/
	if (input_->TriggerKey(DIK_R)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = affine::MatVector(worldTransform_.matWorld_, velocity);
		float len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		if (len != 0)
		{
			velocity /= len;
		}
		velocity *= kBulletSpeed;

		//弾の生成し、初期化
		std::unique_ptr<playerBullet> newBullet = std::make_unique<playerBullet>();
		newBullet->Initialize(worldTransform_.translation_,worldTransform_.rotation_, velocity);

		//弾の登録する
		bullets_.push_back(std::move(newBullet));
	}
}

void player::Draw(ViewProjection& viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	/*model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);*/
	//弾描画
	for (std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}
void player::DrawUI() {
	sprite2DReticle_->Draw();
}
Vector3 player::GetworldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldpos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];
	return worldpos;
}
void player::OnCollision()
{

}
void player::jump()
{
	if (jumpFlag == 1)
	{
		worldTransform_.translation_.y += gravitySpeed;
		gravitySpeed -= 0.06f;

		if (gravitySpeed <= 0)
		{
 			jumpFlag = 2;
		}
	}

	//下がる
	if (jumpFlag == 2)
	{
		worldTransform_.translation_.y -= gravitySpeed;
		gravitySpeed += 0.06f;

		if (gravitySpeed >= defGravitySpeed)
		{
			jumpFlag = 0;
			gravitySpeed = defGravitySpeed;
			worldTransform_.translation_.y = 0.0f;
		}
	}
}
