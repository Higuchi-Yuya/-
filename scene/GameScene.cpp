#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"affine.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	model_ = new Model;

	model_ = Model::CreateFromOBJ("stage");

	worldTransform.Initialize();

	worldTransform.scale_ = { 52,50,52 };

	worldTransform.translation_ = Vector3(0, -2.1f, 0);

	affine::makeAffine(worldTransform);

	worldTransform.TransferMatrix();

	bossPhase_1 = std::make_unique<BossPhase_1>();
	bossPhase_1->Initialize();
	bossPhase_2 = std::make_unique<BossPhase_2>();
	bossPhase_2->Initialize();
	bossPhase_3 = std::make_unique<BossPhase_3>();
	bossPhase_3->Initialize();


	player_ = std::make_unique<player>();
	railCamera_ = std::make_unique<RailCamera>();
	sky_ = std::make_unique<sky>();

	sky_->Initialize();
	player_->Initialize();
	railCamera_->Initialize(Vector3(0, 5, -50), Vector3(0, 0, 0), player_->GetWorldTransform());
}

void GameScene::Update() {
	player_->Update();
	railCamera_->Update();

	// ボスのフェーズ1の更新
	/*bossPhase_1->TitleUpdate();*/
	bossPhase_1->Update(player_->GetworldPosition());

	// ボスのフェーズ2の更新
	bossPhase_2->Update(player_->GetworldPosition());

	// ボスのフェーズ3の更新
	bossPhase_3->Update();

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	model_->Draw(worldTransform, railCamera_->GetViewProjection());
	player_->Draw(railCamera_->GetViewProjection());

	//// ボスフェーズ1の描画
	bossPhase_1->Draw(railCamera_->GetViewProjection());

	// ボスフェーズ2の描画
	bossPhase_2->Draw(railCamera_->GetViewProjection());

	//// ボスフェーズ3の描画
	//bossPhase_3->Draw(railCamera_->GetViewProjection());

	sky_->Draw(railCamera_->GetViewProjection());
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::TitleUpdate()
{


}

void GameScene::CheckAllCollisions()
{
	WorldTransform boss1, boss2;
	Vector3 playerPos;
	Vector3 posB;

	playerPos = player_->GetworldPosition();
	const std::list<std::unique_ptr<playerBullet>>& playerBullets_ = player_->GetBullets();
	const std::unique_ptr<BossBullet>& bossBullet_ = bossPhase_1->GetBullet();
	boss1 = bossPhase_1->GetPos();
	boss2 = bossPhase_2->GetPos();

#pragma region プレイヤー弾とボス

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<playerBullet>& bullet : playerBullets_) {
		//敵弾の座標
		posB = bullet->GetworldPosition();

		const float boss1R = boss1.scale_.x * 3;
		const float boss2R = boss2.scale_.x * 3;
		const float bulletR = 1;
		{
			float A = pow((posB.x - boss1.translation_.x), 2) + pow((posB.y - boss1.translation_.y), 2) + pow((posB.z - boss1.translation_.z), 2);
			float B = pow((boss1R + bulletR), 2);

			if (A <= B)
			{
				//ボス1の衝突時コールバックを呼び出す
				bossPhase_1->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
		{
			float A = pow((posB.x - boss2.translation_.x), 2) + pow((posB.y - boss2.translation_.y), 2) + pow((posB.z - boss2.translation_.z), 2);
			float B = pow((boss2R + bulletR), 2);

			if (A <= B)
			{
				//ボス1の衝突時コールバックを呼び出す
				bossPhase_2->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region プレイヤーとボス1の弾
	//敵弾の座標
	posB = bossBullet_->GetBulletWorldPosition();
	float playerR = 1;
	float bulletR = 1;
	float A = pow((posB.x - playerPos.x), 2) + pow((posB.y - playerPos.y), 2) + pow((posB.z - playerPos.z), 2);
	float B = pow((playerR + bulletR), 2);

	if (A <= B)
	{
		//ボス1の衝突時コールバックを呼び出す
		player_->OnCollision();
		//敵弾の衝突時コールバックを呼び出す
		bossBullet_->OnCollision();
	}

#pragma endregion

#pragma region プレイヤーとボス2のビーム
	if (bossPhase_2->GetBeamFrag()==true)
	{
		WorldTransform BeamTransform = bossPhase_2->GetBeamTransform();
		Vector3 def = affine::MatVector(BeamTransform.matWorld_, { 0,0,-1 });

		if (calcRaySphere({ BeamTransform.matWorld_.m[3][0], BeamTransform.matWorld_.m[3][1] , BeamTransform.matWorld_.m[3][2] }, def, playerPos, playerR))
		{
			//ボス1の衝突時コールバックを呼び出す
			player_->OnCollision();
		}
	}
#pragma endregion

#pragma region プレイヤーとボス2の上ブーメラン
	if (bossPhase_2->GetBoomerangflg(1) == true)
	{
		//敵弾の座標
		posB = bossPhase_2->GetUpBoomerangPos();
		playerR = 1;
		bulletR = boss2.scale_.x * 3;
		A = pow((posB.x - playerPos.x), 2) + pow((posB.z - playerPos.z), 2);
		B = pow((playerR + bulletR), 2);

		if (A <= B)
		{
			//ボス1の衝突時コールバックを呼び出す
			player_->OnCollision();
		}
	}
#pragma endregion

#pragma region プレイヤーとボス2の突進
	//敵弾の座標
	playerR = 1;
	bulletR = boss2.scale_.x * 3;
	A = pow((boss2.translation_.x - playerPos.x), 2) + pow((boss2.translation_.z - playerPos.z), 2);
	B = pow((playerR + bulletR), 2);

	if (A <= B)
	{
		//ボス1の衝突時コールバックを呼び出す
		player_->OnCollision();
	}
#pragma endregion

#pragma region プレイヤーとボス2の下ブーメラン
	if (bossPhase_2->GetBoomerangflg(0) == true)
	{
		//敵弾の座標
		posB = bossPhase_2->GetDownBoomerangPos();
		playerR = 1;
		bulletR = boss2.scale_.x * 3;
		A = pow((posB.x - playerPos.x), 2) + pow((posB.z - playerPos.z), 2);
		B = pow((playerR + bulletR), 2);

		if (A <= B)
		{
			if (playerPos.y - playerR < boss2.translation_.y + boss2.scale_.y &&
				playerPos.y + playerR < boss2.translation_.y - boss2.scale_.y)
			{
				//ボス1の衝突時コールバックを呼び出す
				player_->OnCollision();
			}
		}
	}
#pragma endregion
}
bool GameScene::calcRaySphere(
	Vector3 l,
	Vector3 v,
	Vector3 p,
	float r
) {
	p.x = p.x - l.x;
	p.y = p.y - l.y;
	p.z = p.z - l.z;

	float A = v.x * v.x + v.y * v.y + v.z * v.z;
	float B = v.x * p.x + v.y * p.y + v.z * p.z;
	float C = p.x * p.x + p.y * p.y + p.z * p.z - r * r;

	if (A == 0.0f)
		return false; // レイの長さが0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // 衝突していない

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // レイの反対で衝突

	return true;
}
