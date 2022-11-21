#include "BossPhase_1.h"


void BossPhase_1::Initialize()
{

	input_ = Input::GetInstance();

	model_ = Model::CreateFromOBJ("BossCube");

	for (int i = 0; i < 27; i++) {

		worldTransform_[i].Initialize();
		// フラグ
		AnnihilationFlag[i] = false;
	}

	// 親
	worldTransform_[0].translation_ = { 0,10,50 };
	debugText_ = DebugText::GetInstance();

	// ボスバレットの初期化
	bullet = std::make_unique<BossBullet>();
	bullet->SetPos(worldTransform_[0].translation_);

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

void BossPhase_1::Update(Vector3 playerPos)
{
	// キューブの移動
	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT)) {
		if (input_->PushKey(DIK_UP)) { worldTransform_[0].translation_.y += 1.0f; }
		else if (input_->PushKey(DIK_DOWN)) { worldTransform_[0].translation_.y -= 1.0f; }
		if (input_->PushKey(DIK_RIGHT)) { worldTransform_[0].translation_.x += 1.0f; }
		else if (input_->PushKey(DIK_LEFT)) { worldTransform_[0].translation_.x -= 1.0f; }
	}

	TurnBodyToPlayer(playerPos);

	rotaAngle = worldTransform_[0].rotation_;


	// 行列の更新と転送
	TransferMat();

	// ランダムに選ばれたブロックが消えたら弾の処理を開始
	FlyBlocks(playerPos);

	// 行列の更新と転送
	TransferMat();
}

void BossPhase_1::TitleUpdate()
{
	//横に回転しながら浮遊
	titleRadian += 0.75f;
	if (titleRadian >= 360.0f) {
		titleRadian -= 360.0f;
	}

	worldTransform_[0].rotation_.y =  MathUtility::PI / 180* titleRadian;
	worldTransform_[0].translation_.y = (sin(MathUtility::PI / 180 * titleRadian) * 2.0f) + 12;

	debugText_->SetPos(50, 90);
	debugText_->Printf("boss1:translation:%1.4f,%1.4f,%1.4f", worldTransform_[0].translation_.x, worldTransform_[0].translation_.y, worldTransform_[0].translation_.z);

	TransferMat();
}

void BossPhase_1::Draw(ViewProjection viewprojection)
{
	for (int i = 0; i < 27; i++) {
		if (AnnihilationFlag[i] == false)
		{
			model_->Draw(worldTransform_[i], viewprojection);
		}
	}

	// 発射開始したら弾の描画をする
	if (AnnihilationFlag[randomBlock] == true && bullet->GetToPlayerFlag() == false) {
		bullet->Draw(viewprojection);
	}
}

void BossPhase_1::OnCollision()
{

}

void BossPhase_1::FlyBlocks(Vector3 playerPos)
{
	// ランダムに抽選を行う
	FloatRandomBlocks();

	// 発射開始したら弾の更新を行う
	if (AnnihilationFlag[randomBlock] == true) {
		bullet->Update(playerPos);
		// スケールを小さくする
		worldTransform_[randomBlock].scale_ = { 0.0f,0.0f,0.0f };
	}

	
	// 弾がプレイヤーの位置に行ったら
	if (bullet->GetToPlayerFlag() == true) {
		// 徐々に元のものを拡大して再生させる
		expansionScale += expansionScaleSpeed;

		worldTransform_[randomBlock].scale_.x = min(expansionScale.x, 1);
		worldTransform_[randomBlock].scale_.y = min(expansionScale.y, 1);
		worldTransform_[randomBlock].scale_.z = min(expansionScale.z, 1);

		AnnihilationFlag[randomBlock] = false;
		bullet->SetPos(worldTransform_[0].translation_ + oldPos);

	}
	else
	{
		expansionScale = { 0,0,0 };
	}

	// 元のものがスケール元に戻ったらリセットをする
	if (worldTransform_[randomBlock].scale_.x >= 1.0f) {
		ResetFlyBlocks();
	}
	

}

void BossPhase_1::FloatRandomBlocks()
{
	// 拒否してるブロック以外だったら再抽選を終わらせる
	while (randomBlock == 0 || randomBlock == 2 || randomBlock == 11 || randomBlock == 20 || randomBlock == 6 || randomBlock == 15 || randomBlock == 24) {
		randomBlock = rand() % 26 + 1;
		oldPos = worldTransform_[randomBlock].translation_;
		// 弾の登録
		bullet->Initialize(model_, worldTransform_[randomBlock], worldTransform_[0].translation_,rotaAngle);
	}

	// 再抽選が終わったら
	if (worldTransform_[randomBlock].translation_.x > worldTransform_[0].translation_.x ||
		worldTransform_[randomBlock].translation_.x < worldTransform_[0].translation_.x) 
	{
		AnnihilationFlag[randomBlock] = true;
	}

}

// 浮くブロックに関する初期化の処理
void BossPhase_1::ResetFlyBlocks()
{
	randomBlock = 0;
	FloatBlockFlagM = false;
	FloatBlockFlagP = false;
	for (int i = 0; i < 27; i++) {
		// 元になるものを消すフラグのリセット
		AnnihilationFlag[i] = false;
	}
}

void BossPhase_1::TransferMat()
{
	for (int i = 0; i < 27; i++) {
		affine::makeAffine(worldTransform_[i]);
		if (i != 0) {
			worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
		}
		worldTransform_[i].TransferMatrix();
	}
}

void BossPhase_1::TurnBodyToPlayer(Vector3 playerPos)
{
	// 打つ方向に向けてオブジェクトを回転させる
	Vector3 velocity = playerPos - worldTransform_[0].translation_;
	velocity.normalize();
	// Y軸周り角度(θy)
	worldTransform_[0].rotation_.y = std::atan2(velocity.x, velocity.z);
	// Y軸周りに-θy回す回転行列を計算
	Matrix4 RotY;
	affine::makeMatRotY(RotY, -worldTransform_[0].rotation_.y);
	// velosity_に回転行列を掛け算してvelosityZを求める
	Vector3 velosityZ = velocity;
	velosityZ = affine::MatVector(RotY, velosityZ);

	// X軸周り角度(θx)
	worldTransform_[0].rotation_.x = std::atan2(-velosityZ.y, velosityZ.z);
}
	
