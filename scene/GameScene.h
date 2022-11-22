#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"player.h"
#include"playerBullet.h"
#include"railCamera.h"
#include"sky.h"
#include "BossPhase_1.h"
#include "BossPhase_2.h"
#include "BossPhase_3.h"
#include<sstream>

enum PositionIndex {
	Title,	//タイトル
	GameStart,//ゲーム開始
	GameBossTrans,//ボス変身
	GameBossDeath,//ボス死亡
	GameBossDeath2,//ボス死亡
	
	PositionIndexCount,//インデックス総数
};

enum Phase {
	TitleToGame,//タイトルからゲームへ
	Boss1To2,//ボス１から２
	GameToResult,//ゲームからリザルト(クリア)

	PhaseIndex,//フェーズの総数
};

enum class GameLoop
{
	Title,
	Game,
	GameOver,
	Result,
};
enum class BossTrans
{
	TitleToGame,//タイトルからゲームへ
	Boss1,
	Boss1To2,//ボス１から２
	Boss2,
	GameToResult,//ゲームからリザルト(クリア)
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// タイトル更新処理
	/// </summary>
	void TitleUpdate();

	///<summary>
	///衝突判定と応答
	///</summary>
	void CheckAllCollisions();

	bool calcRaySphere(Vector3 l,Vector3 v,	Vector3 p,float r);

	void AnimationCameraUpdate();

	//シェイク関数
	Vector3 Shake(const Vector3& firstPos, int& shakeCount);

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	std::unique_ptr<player> player_;
	std::unique_ptr<RailCamera> railCamera_ = nullptr;
	std::unique_ptr<sky> sky_=nullptr;

	Model* model_ = nullptr;
	WorldTransform worldTransform;
	std::unique_ptr<BossPhase_1> bossPhase_1;
	std::unique_ptr<BossPhase_2> bossPhase_2;
	std::unique_ptr<BossPhase_3> bossPhase_3;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//カメラ座標
	Vector3 cameraPos[PositionIndexCount];
	ViewProjection titleCamera;
	Matrix4 boss2Mat;
	float animeTimer = 0;

	int animetionPhase = 0;
	int cameraShakeCount = 0;

	uint32_t titleTexture = 0;
	uint32_t titleUITexture = 0;
	Sprite* titleSprite = nullptr;
	Sprite* titleUISprite = nullptr;

	ViewProjection *viewProjection=nullptr;


	GameLoop gameLoop=GameLoop::Title;
	BossTrans bossTrans = BossTrans::TitleToGame;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
