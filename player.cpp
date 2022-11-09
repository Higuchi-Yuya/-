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

	//自機のワールド座標から3Dレティクルのワールド座標を計算
		//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 1.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { kDistancePlayerTo3DReticle,0,0 };
	//自機のワールド行列の回転を反映
	offset = affine::MatVector(worldTransform_.matWorld_, offset);

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
	if (input_->PushKey(DIK_LEFT))
	{
		move.x = -0.1 * offset.x;
		move.z = -0.1 * offset.z;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x = 0.1 * offset.x;
		move.z = 0.1 * offset.z;
	}
	if (input_->PushKey(DIK_UP))
	{
		move.y = 0.1;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		move.y = -0.1;
	}
	if (input_->PushKey(DIK_A))
	{
		rot.y = 0.001;
	}
	if (input_->PushKey(DIK_D))
	{
		rot.y = -0.001;
	}

	worldTransform_.translation_ += move;
	worldTransform_.rotation_.y += rot.y * (180 / PI);

	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	////範囲を超えない処理
	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);




	affine::makeMatIdentity(worldTransform_.matWorld_);
	affine::makeMatRot(worldTransform_.matWorld_, worldTransform_.rotation_);
	affine::makeMatTrans(worldTransform_.matWorld_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	debugText_->SetPos(10, 10);
	debugText_->Printf("%f,%f,%f", offset.x, offset.y, offset.z);
}

void player::Attack() {
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = affine::MatVector(worldTransform_.parent_->matWorld_, velocity);
		velocity = affine::MatVector(worldTransform_.matWorld_, velocity);
		velocity = MathUtility::operator-(affine::GetWorldTrans(worldTransform3DReticle_.matWorld_), affine::GetWorldTrans(worldTransform_.matWorld_));
		float len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		if (len != 0)
		{
			velocity /= len;
		}
		velocity *= kBulletSpeed;

		//弾の生成し、初期化
		std::unique_ptr<playerBullet> newBullet = std::make_unique<playerBullet>();
		Vector3 playerWolrdPos = worldTransform_.parent_->translation_;
		playerWolrdPos += worldTransform_.translation_;
		Vector3 playerWolrdRot = worldTransform_.parent_->rotation_;
		playerWolrdRot += worldTransform_.rotation_;
		newBullet->Initialize(GetworldPosition(), velocity);

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
