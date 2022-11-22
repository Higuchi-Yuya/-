#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"affine.h"
#include<random>
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete titleSprite;
	delete titleUISprite;
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
	railCamera_->Update();
	viewProjection = &titleCamera;

	//カメラ初期座標の初期化
	cameraPos[Title] = bossPhase_1->GetWorldTransformP().translation_;
	cameraPos[Title].z -= 35.0f;
	cameraPos[Title].y -= 5.5f;
	cameraPos[GameStart] = railCamera_->GetViewProjection().eye;
	//cameraPos[GameStart].z -= 50.0f;
	cameraPos[GameBossTrans] = bossPhase_1->GetWorldTransformP().translation_;
	cameraPos[GameBossTrans].z -= 50.0f;
	//cameraPos[GameBossDeath];

	titleCamera.Initialize();
	titleCamera.eye = cameraPos[Title];
	titleCamera.target = Vector3(cameraPos[Title].x, cameraPos[Title].y, cameraPos[Title].z + 50);
	titleCamera.UpdateMatrix();
	animeTimer = 0;

	animetionPhase = Phase::TitleToGame;

	//タイトル用テクスチャ読み込み
	titleTexture = TextureManager::Load("Title.png");
	titleUITexture = TextureManager::Load("PressAStart.png");
	int texturePlayGuido = TextureManager::Load("Sousa.png");
	int texturerRisultUI = TextureManager::Load("PressATitle.png");
	int gameOverTexture = TextureManager::Load("GameOver.png");
	int resultTexture = TextureManager::Load("GameCLEAR.png");

	//スプライト生成
	titleSprite = Sprite::Create(titleTexture, { WinApp::kWindowWidth / 2,256 });
	titleUISprite = Sprite::Create(titleUITexture, { WinApp::kWindowWidth / 2,WinApp::kWindowHeight - 64 });
	playGuideSprite = Sprite::Create(texturePlayGuido,{30,490},{1,1,1,1},{0,0});
	resultUISprite = Sprite::Create(texturerRisultUI,{ WinApp::kWindowWidth / 2,WinApp::kWindowHeight - 64 },{1,1,1,1},{0.5,1});
	GameOverSprite = Sprite::Create(gameOverTexture,{ WinApp::kWindowWidth / 2,180 },{1,1,1,1}, Vector2(0.5f, 0.3f));
	resultSprite = Sprite::Create(resultTexture, { WinApp::kWindowWidth / 2,180 }, { 1,1,1,1 }, Vector2(0.5f, 0.3f));

	titleSprite->SetAnchorPoint(Vector2(0.5f, 0.3f));
	titleUISprite->SetAnchorPoint(Vector2(0.5f, 1));
	titleUISprite->SetSize(Vector2(784 * 3 / 10, 288 * 3 / 10));
	titleSprite->SetSize(Vector2(688 * 9 / 10, 336 * 9 / 10));
	playGuideSprite->SetSize({212,212});
	resultUISprite->SetSize(Vector2(784 * 3 / 10, 288 * 3 / 10));
	GameOverSprite->SetSize(Vector2(688 * 9 / 10, 336 * 9 / 10));
	resultSprite->SetSize(Vector2(688 * 9 / 10, 336 * 9 / 10));
}

void GameScene::Update() {

	if (input_->TriggerKey(DIK_0))
	{
		viewProjection = &railCamera_->GetViewProjection();
	}
	if (input_->TriggerKey(DIK_5))
	{
		viewProjection = &titleCamera;
	}

	XINPUT_STATE gamePad;
	XINPUT_STATE oldGamePad;
	input_->GetInstance()->GetJoystickStatePrevious(0, oldGamePad);
	input_->GetInstance()->GetJoystickState(0, gamePad);
	int A = (gamePad.Gamepad.wButtons & XINPUT_GAMEPAD_A);
	int oldA = (oldGamePad.Gamepad.wButtons & XINPUT_GAMEPAD_A);
	switch (gameLoop)
	{
	case GameLoop::Title:
		bossPhase_1->TitleUpdate();
		if (A == 4096 &&oldA== 0)
		{
			gameLoop = GameLoop::Game;
				player_->TransformRset(false);
				railCamera_->Update();
		}
		break;
	case GameLoop::Game:
		switch (bossTrans)
		{
		case BossTrans::TitleToGame:
			bossPhase_1->TitleUpdate();
			AnimationCameraUpdate();
			if (animeTimer >= 1)
			{
				viewProjection = &railCamera_->GetViewProjection();
				bossTrans = BossTrans::Boss1;
				railCamera_->Update();
			}
			break;
		case BossTrans::Boss1:
			bossPhase_1->Update(player_->GetworldPosition());
			player_->Update();
			railCamera_->Update();
			if (bossPhase_1->GetHP() <= 0)
			{
				player_->TransformRset(false);
				railCamera_->Update();
				player_->AllBulletDelete();
				animeTimer = 0;
				animetionPhase = Phase::Boss1To2;
				cameraShakeCount = 49;
				viewProjection = &titleCamera;
				bossTrans = BossTrans::Boss1To2;
			}
			if (player_->GetHP() <= 0)
			{
				player_->AllBulletDelete();
				gameLoop = GameLoop::GameOver;
			}
			break;
		case BossTrans::Boss1To2:
			//// ボスフェーズ1の描画
			if (animeTimer < 50)
			{
				bossPhase_1->TitleUpdate();
			}
			else
			{
				bossPhase_2->TitleUpdate();
			}
			AnimationCameraUpdate();
			if (animeTimer - 52 >= 0)
			{
				viewProjection = &railCamera_->GetViewProjection();
				bossTrans = BossTrans::Boss2;
			}
			break;
		case BossTrans::Boss2:
			player_->Update();
			bossPhase_2->Update(player_->GetworldPosition());
			railCamera_->Update();
			if (bossPhase_2->GetHP() <= 0)
			{
				player_->TransformRset(false);
				player_->AllBulletDelete();
				animeTimer = 0;
				animetionPhase = Phase::GameToResult;
				player_->SetEndMoveRotation(bossPhase_2->GetPos().translation_);
				player_->Update();

				//ボスを死亡
				bossPhase_2->SetIsDead(true);
				boss2Mat = bossPhase_2->GetPos().matWorld_;
				viewProjection = &titleCamera;
				bossTrans = BossTrans::GameToResult;
			}
			if (player_->GetHP() <= 0)
			{
				player_->AllBulletDelete();
				gameLoop = GameLoop::GameOver;
			}
			break;
		case BossTrans::GameToResult:
			AnimationCameraUpdate();
			bossPhase_2->Update(player_->GetworldPosition());
			if (bossPhase_2->GetMedamaWTTransformY() <= -10)
			{

				gameLoop = GameLoop::Result;
			}
			break;
		}
		break;
	case GameLoop::GameOver:
		if (bossTrans == BossTrans::Boss1)
		{
			bossPhase_1->Update(player_->GetworldPosition());
		}
		else
		{
			bossPhase_2->Update(player_->GetworldPosition());
		}
		if (A == 4096 && oldA == 0)
		{
			animeTimer = 0;
			animetionPhase = TitleToGame;
			player_->Rset();
			bossPhase_2->Rset();
			bossPhase_1->Rset();
			gameLoop = GameLoop::Title;
			bossTrans = BossTrans::TitleToGame;
			viewProjection = &titleCamera;
			titleCamera.eye = cameraPos[Title];
			titleCamera.target = Vector3(cameraPos[Title].x, cameraPos[Title].y, cameraPos[Title].z + 50);
			titleCamera.UpdateMatrix();

		}
		break;
	case GameLoop::Result:
		if (A == 4096 && oldA == 0)
		{
			animeTimer = 0;
			animetionPhase = TitleToGame;
			player_->Rset();
			bossPhase_2->Rset();
			bossPhase_1->Rset();
			gameLoop = GameLoop::Title;
			bossTrans = BossTrans::TitleToGame;
			titleCamera.eye = cameraPos[Title];
			titleCamera.target = Vector3(cameraPos[Title].x, cameraPos[Title].y, cameraPos[Title].z + 50);
			titleCamera.UpdateMatrix();
		}
		break;
	}
	/*cameraPos[GameStart] = railCamera_->GetViewProjection().eye;*/

	// ボスのフェーズ2の更新
	//bossPhase_2->Update(player_->GetworldPosition());

	CheckAllCollisions();
	/*debugText_->SetPos(10, 10);
	debugText_->Printf("boss1:%d", bossPhase_1->GetHP());
	debugText_->SetPos(10, 30);
	debugText_->Printf("boss2:%d", bossPhase_2->GetHP());
	debugText_->SetPos(10, 50);
	debugText_->Printf("player:%d", player_->GetHP());
	debugText_->SetPos(10, 70);
	debugText_->Printf("%d",A);
	debugText_->SetPos(10, 90);
	debugText_->Printf("%d",oldA);*/
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
	model_->Draw(worldTransform, *viewProjection);
	player_->Draw(*viewProjection);
	switch (gameLoop)
	{
	case GameLoop::Title:
		//// ボスフェーズ1の描画
		bossPhase_1->Draw(*viewProjection);

		break;
	case GameLoop::Game:
		switch (bossTrans)
		{
		case BossTrans::TitleToGame:
			//// ボスフェーズ1の描画
			bossPhase_1->Draw(*viewProjection);
			break;
		case BossTrans::Boss1:
			//// ボスフェーズ1の描画
			bossPhase_1->Draw(*viewProjection);
			break;
		case BossTrans::Boss1To2:
			//// ボスフェーズ1の描画
			if (animeTimer < 50)
			{
				bossPhase_1->Draw(*viewProjection);
			}
			if (animeTimer >= 50)
			{
				bossPhase_2->Draw(*viewProjection);
			}
			break;
		case BossTrans::Boss2:
			bossPhase_2->Draw(*viewProjection);
			break;
		case BossTrans::GameToResult:
			bossPhase_2->Draw(*viewProjection);
			break;
		}
		break;
	case GameLoop::GameOver:
		//// ボスフェーズ1の描画
		if (bossTrans == BossTrans::Boss1)
		{
			bossPhase_1->Draw(*viewProjection);
		}
		else
		{
			bossPhase_2->Draw(*viewProjection);
		}
		break;
	case GameLoop::Result:
		break;
	}

	sky_->Draw(*viewProjection);
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
	switch (gameLoop)
	{
	case GameLoop::Title:
		titleSprite->Draw();
		titleUISprite->Draw();
		break;
	case GameLoop::Game:
		switch (bossTrans)
		{
		case BossTrans::TitleToGame:
			break;
		case BossTrans::Boss1:
			playGuideSprite->Draw();
			bossPhase_1->DrawUI();
			player_->DrawUI();
			break;
		case BossTrans::Boss1To2:
			break;
		case BossTrans::Boss2:
			playGuideSprite->Draw();
			bossPhase_2->DrawUI();
			player_->DrawUI();
			break;
		case BossTrans::GameToResult:
			break;
		default:
			break;
		}
		break;
	case GameLoop::GameOver:
		GameOverSprite->Draw();
		resultUISprite->Draw();
		break;
	case GameLoop::Result:
		resultSprite->Draw();
		resultUISprite->Draw();
		break;
	}


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
	if (bossPhase_2->GetBeamFrag() == true)
	{
		WorldTransform BeamTransform = bossPhase_2->GetBeamTransform();
		Vector3 def = affine::MatVector(BeamTransform.matWorld_, { 0,0,-1 });

		if (calcRaySphere({ BeamTransform.matWorld_.m[3][0], BeamTransform.matWorld_.m[3][1] , BeamTransform.matWorld_.m[3][2] }, def, playerPos, playerR))
		{
			if (BeamTransform.scale_.z > 40)
			{
				//ボス1の衝突時コールバックを呼び出す
				player_->OnCollision();
			}
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


	}




	if (A <= B)
	{
		//ボス1の衝突時コールバックを呼び出す
		player_->OnCollision();
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

void GameScene::AnimationCameraUpdate()
{
	if (input_->PushKey(DIK_8)) {
		animeTimer = 0;
		animetionPhase = TitleToGame;
	}

	if (input_->PushKey(DIK_9)) {
		animeTimer = 0;
		animetionPhase = Phase::Boss1To2;
		cameraShakeCount = 49;
	}

	if (input_->PushKey(DIK_7)) {
		animeTimer = 0;
		animetionPhase = Phase::GameToResult;
		player_->SetEndMoveRotation(bossPhase_2->GetPos().translation_);
		//ボスを死亡
		bossPhase_2->SetIsDead(true);
		boss2Mat = bossPhase_2->GetPos().matWorld_;
	}


	if (animetionPhase == TitleToGame) {

		animeTimer += 0.025f;
		if (animeTimer >= 1) {
			animeTimer = 1;
		}
		Vector3 pos, target;
		pos = pos.lerp(cameraPos[Title], cameraPos[GameStart], animeTimer);
		target = target.lerp(Vector3(cameraPos[Title].x, cameraPos[Title].y, cameraPos[Title].z + 5.0f), railCamera_->GetViewProjection().target, animeTimer);
		titleCamera.eye = pos;
		titleCamera.target = target;

	}
	else if (animetionPhase == Boss1To2) {
		if (animeTimer < 50.0f) {
			animeTimer++;
			titleCamera.eye = Shake(cameraPos[GameBossTrans], cameraShakeCount);
			titleCamera.target = bossPhase_1->GetWorldTransformP().translation_;
		}
		else if (animeTimer >= 50.0f) {
			animeTimer += 0.025f;
			if (animeTimer >= 53) {
				animeTimer = 53;
			}
			Vector3 pos;
			float easeTime = animeTimer - 52;
			if (easeTime >= 0) {
				pos = pos.lerp(cameraPos[GameBossTrans], cameraPos[GameStart], easeTime);
				titleCamera.eye = pos;
			}
		}
		titleCamera.target = Vector3(titleCamera.eye.x, titleCamera.eye.y, titleCamera.eye.z + 5.0f);
	}
	else if (animetionPhase == Phase::GameToResult) {
		//プレイヤーを敵に向かせる
		float playerRota = bossPhase_2->GetPos().rotation_.y + 180.0f;

		//プレイヤーのY軸回転を設定する
		//player_->

		if (animeTimer < 425) {
			animeTimer++;
			//注視点からカメラの距離
			float cameraDistance = 40.0f;
			//カメラ回転角
			float cameraRotation = 30.0f;
			Vector3 eye{}, target;
			//注視点はボス2に
			target = bossPhase_2->GetPos().translation_;
			//目を45度回転
			eye.z -= cos(MathUtility::PI / 180 * cameraRotation) * cameraDistance;
			eye.x -= sin(MathUtility::PI / 180 * cameraRotation) * cameraDistance;
			//ボス2の座標を使ってワールド座標返還
			eye = MathUtility::Vector3Transform(eye, boss2Mat);
			//使うカメラに代入
			titleCamera.eye = eye;
			titleCamera.target = bossPhase_2->GetPos().translation_;
			//後の線形補間で使う用の変数に値を入れる
			//1つめの座標はいま使っている座標
			cameraPos[GameBossDeath] = eye;
			//2つめの座標(補間の終点)はプレイヤー座標参照
			float rota2 = 75.0f;
			Vector3 nextEye{
				-cos(MathUtility::PI / 180 * rota2) * cameraDistance / 2 ,
				0,
				-sin(MathUtility::PI / 180 * rota2) * cameraDistance / 2 };
			cameraPos[GameBossDeath2] = MathUtility::Vector3Transform(nextEye, player_->GetWorldTransform()->matWorld_);
		}
		else {
			animeTimer += 0.0125f;
			float easeTime = animeTimer - 425;
			if (animeTimer >= 426) {
				animeTimer = 426;
			}
			Vector3 eye, target;
			eye = eye.lerp(cameraPos[GameBossDeath], cameraPos[GameBossDeath2], easeTime);
			Vector3 pos1, pos2;
			pos1 = Vector3(boss2Mat.m[3][0], boss2Mat.m[3][1], boss2Mat.m[3][2]);
			pos2 = player_->GetWorldTransform()->translation_;
			target = target.lerp(pos1, pos2, easeTime / 2);

			titleCamera.eye = eye;
			titleCamera.target = target;
		}
	}

	titleCamera.UpdateMatrix();

	/*debugText_->SetPos(50, 150);
	debugText_->Printf("bossPosition:%1.4f,%1.4f,%1.4f", bossPhase_2->GetPos().translation_.x, bossPhase_2->GetPos().translation_.y, bossPhase_2->GetPos().translation_.z);
	debugText_->SetPos(50, 170);
	debugText_->Printf("cameraTarget:%1.4f,%1.4f,%1.4f", titleCamera.target.x, titleCamera.target.y, titleCamera.target.z);
	debugText_->SetPos(50, 185);
	debugText_->Printf("animeTimer:%1.5f", animeTimer);*/
}

Vector3 GameScene::Shake(const Vector3& firstPos, int& shakeCount)
{
	//シェイク回数が0なら初期座標に戻す
	if (shakeCount <= 0) {
		return firstPos;
	}

	random_device seed_gem;
	mt19937_64 engine(seed_gem());
	uniform_real_distribution<float> rand(-2, 2);


	//それ以外なら初期座標をランダムに増減して返す
	Vector3 pos = firstPos;
	pos.x += rand(engine);
	pos.y += rand(engine);
	pos.z += rand(engine);

	shakeCount--;

	return pos;
}