#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"affine.h"
#include<random>
using namespace std;

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

	model_ = Model::Create();

	worldTransform.Initialize();

	worldTransform.scale_ = { 100,1,100 };

	worldTransform.translation_ = Vector3(0, -2, 0);

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

	//カメラ初期座標の初期化
	cameraPos[Title] = bossPhase_1->GetWorldTransformP().translation_;
	cameraPos[Title].z -= 50.0f;
	cameraPos[GameStart] = railCamera_->GetViewProjection().eye;
	//cameraPos[GameStart].z -= 50.0f;
	cameraPos[GameBossTrans] = Vector3(0,5,50);
	cameraPos[GameBossTrans] = cameraPos[Title];
	cameraPos[GameBossDeath];

	titleCamera.Initialize();
	titleCamera.eye = cameraPos[Title];
	titleCamera.target = Vector3(cameraPos[Title].x, cameraPos[Title].y, cameraPos[Title].z + 50);
	titleCamera.UpdateMatrix();
	animeTimer = 0;

	animetionPhase = Phase::TitleToGame;
}

void GameScene::Update() {
	player_->Update();
	railCamera_->Update();

	if (input_->PushKey(DIK_8)) {
		animeTimer = 0;
		animetionPhase = TitleToGame;
	}

	if (input_->PushKey(DIK_9)) {
		animeTimer = 0;
		animetionPhase = Phase::Boss1To2;
		cameraShakeCount =49;
	}



	if (animetionPhase == TitleToGame) {

		animeTimer += 0.025f;

		if (animeTimer >= 1) {
			animeTimer = 1;
		}

		Vector3 pos;
		pos = pos.lerp(cameraPos[Title], cameraPos[GameStart], animeTimer);

		titleCamera.eye = pos;
		titleCamera.target = Vector3(pos.x, pos.y, pos.z + 5.0f);
		titleCamera.UpdateMatrix();

	}
	else if (animetionPhase == Boss1To2) {
		if (animeTimer < 50.0f) {
			animeTimer++;
			titleCamera.eye = Shake(cameraPos[GameBossTrans], cameraShakeCount);
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
		titleCamera.UpdateMatrix();
	}

	debugText_->SetPos(50, 190);
	debugText_->Printf("animeTimer:%1.5f", animeTimer);

	// ボスのフェーズ1の更新
	bossPhase_1->TitleUpdate();
	//bossPhase_1->Update(player_->GetworldPosition());

	// ボスのフェーズ2の更新
	bossPhase_2->Update(player_->GetworldPosition());

	// ボスのフェーズ3の更新
	//bossPhase_3->Update();
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
	model_->Draw(worldTransform, titleCamera);
	player_->Draw(titleCamera);

	//// ボスフェーズ1の描画
	bossPhase_1->Draw(titleCamera);

	// ボスフェーズ2の描画
	if (animetionPhase >= Phase::Boss1To2) {
		bossPhase_2->Draw(railCamera_->GetViewProjection());
	}

	//// ボスフェーズ3の描画
	//bossPhase_3->Draw(railCamera_->GetViewProjection());

	sky_->Draw(titleCamera);
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
