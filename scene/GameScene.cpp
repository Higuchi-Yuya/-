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

	worldTransform.Initialize();

	worldTransform.translation_ = Vector3(0,0,0);

	affine::makeMatTrans(worldTransform.matWorld_,worldTransform.translation_);

	worldTransform.TransferMatrix();

	bossPhase_1 = std::make_unique<BossPhase_1>();
	bossPhase_1->Initialize();
	bossPhase_3 = std::make_unique<BossPhase_3>();
	bossPhase_3->Initialize();
	

	player_ = std::make_unique<player>();
	railCamera_ = std::make_unique<RailCamera>();
	sky_ = std::make_unique<sky>();

	sky_->Initialize();
	player_->Initialize();
	railCamera_->Initialize(Vector3(0,5,-50),Vector3(0,0,0), player_->GetWorldTransform());
}

void GameScene::Update() {
	player_->Update();
	railCamera_->Update();

	// ボスのフェーズ1の更新
	bossPhase_1->Update(player_->GetworldPosition());
	// ボスのフェーズ3の更新
	bossPhase_3->Update();
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
	model_->Draw(worldTransform,railCamera_->GetViewProjection());
	player_->Draw(railCamera_->GetViewProjection());

	// ボスフェーズ１の描画
	bossPhase_1->Draw(railCamera_->GetViewProjection());
	// ボスフェーズ3の描画
	bossPhase_3->Draw(railCamera_->GetViewProjection());

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
