#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>

class BossPhase_2
{
public:// サブクラス
	enum class Motion {
		standby,
		beam,
		boomerang,
		rush,
		reflector,
	};
	enum class Action
	{
		AttackInterval,
		AttackSelection,
		AttackInMotion
	};

public:// メンバ関数
	// 初期化関数
	void Initialize();

	// 更新処理
	void Update(Vector3 playerPos);

	/// <summary>
/// 第一形態～第二形態へ遷移するようの更新処理
/// </summary>
	void TitleUpdate();

	// 描画処理
	void Draw(ViewProjection viewprojection);

	//衝突を検出したら呼び出されたるコールバック関数
	void OnCollision();

	bool GetBeamFrag() { return beamOBJSetFlag; }

	bool GetBoomerangflg(bool UpOrDown);

	void SetIsDead(bool isDead);

	WorldTransform GetPos() { return worldTransform_[0]; }

	Vector3 GetUpBoomerangPos() { return upBoomerangWorldTransform->translation_; }

	Vector3 GetDownBoomerangPos() { return downBoomerangWorldTransform->translation_; }

	WorldTransform GetBeamTransform() { return beamWorldTransform_; }

	int GetHP() { return HP; }

	float GetMedamaWTTransformY() { return medamaWT.translation_.y; }

	void Rset();

private:
	void beamUpdate(Vector3 playerPos);

	void boomerangUpdate(Vector3 playerPos);

	void rushUpdate(Vector3 playerPos);

	// 行列の転送と更新
	void TransferMat();

	float Lerp(const float& startPos, const float& endPos, const float& timeRate);

	double easing_Out(double start, double end, double time, double max_time);

	double easing_In(double start, double end, double time, double max_time);
	

	float DegreeToRad(float num)
	{
		return num / 180 * MathUtility::PI;
	}

	float RadToDegree(float num)
	{
		return num / MathUtility::PI * 180;
	}

	// ビーム関連のリセット
	void beamReset();

	// 待機時のプレイヤーに向く処理
	void TurnBodyToPlayer(Vector3 playerPos);
	
	// 死んだときに一度だけプレイヤーに向く処理
	void TurnDeadToPlayer(Vector3 playerPos);

	// ビーム打ってる間のボスの向き
	void TurnBeamToPlayer();
	// 突進の予備動作の時のプレイヤーに向く処理
	void TurnRushToPlayer(Vector3 playerPos);

	//ブーメラン関連のセット
	void  boomerangSet(Vector3 playerPos,bool UpOrDown);

	// 突進のリセット関数
	void rushReset();

	// 死亡時の爆散処理
	void DeathblowUp();

private:// メンバ変数
	//ワールド変換データ
	WorldTransform worldTransform_[19];

	// キューブ用のスケールの変数
	float kyubuScale = 3.0f;
	float kyubuLengh = 2.0f;

	// 親の回転要素
	Vector3 oyaRota;
	Matrix4 oyaRotation;

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	// インプット
	Input* input_ = nullptr;

	//モデル
	Model* model_ = nullptr;

	Vector3 playerPos_;

	int maxHP = 1;
	int HP = maxHP;

	Action isAction= Action::AttackInterval;
	const int maxIntervalFrame=200;
	int intervalFrame=0;
	int randAttack=0;
	int Attack=0;
	int oldAttack=0;

	// ビーム用の変数
	#pragma region ビーム用の変数
	// ビームのワールドデータ
	WorldTransform beamWorldTransform_;
	// ビームのモデル
	Model* beamModel_ = nullptr;

	bool beamFlag = false;
	bool beamSetFlag = false;
	bool beamOBJSetFlag = false;
	int beamTimer = 0;
	int beamtoPTimer = 0;

	// 半分の時間
	int maxTimer = 6 * 60;
	// ビーム開始の時間
	int maxStartTimer = 3 * 60;
	// ビーム収束開始の時間
	int maxEndTimer = 6 * 60;
	// ポジション更新する間隔
	int beamUpdatePosIntaval = 0.3 * 60;
	// 収束時間
	int convergenceTimer = 0;
	int maxConvergenceT = 6 * 60;
	
	Vector3 oldPlayerPos;
	Vector3 oldPlayerPos2;
	Vector3 oldVelocity;
	Vector3 beamToPlayerVel;
	Vector3 beamToPlayerVelUp;
	Vector3 beamLength;
	float beamSpeed = 1.0f;
	#pragma endregion

#pragma region 移動用変数

	float angle = 1.57;

	float radius = 50;

#pragma endregion

#pragma region ブーメラン用変数

	// 消すときのフラグ
	bool AnnihilationFlag[27];

	Model* torunedoModel_ = nullptr;

	WorldTransform torunedoTrans;

	//上のブーメラン用ワールドトランスフォーム
	WorldTransform upBoomerangWorldTransform[5];

	Vector3 upVector;

	bool isUpActive = false;

	bool isUpAttack = false;

	bool isUpPreparation = false;

	//下のブーメランようワールドトランスフォーム
	WorldTransform downBoomerangWorldTransform[5];

	Vector3 downVector;

	bool isDownActive = false;

	bool isDownAttack = false;

	bool isDownDirectionRight = false;

	bool isDownPreparation = false;

#pragma endregion

#pragma region 突進用変数
	// 突進開始用のフラグ
	bool rushFlag = false;
	// 下げるときのポジションY
	float downPosY = 10.0f;
	// 元の高さを記録する変数
	float originPosY = 0.0f;

	// 開始時セットするフラグ
	bool rushStartSetFlag = false;

	// 突進時のボスとプレイヤーのベクトル
	Vector3 rushToPlayer;

	// 突進するときの車輪の速さ
	int wheelTimer = 0;
	int wheelEndTime = 4 * 60;
	float wheelSpeedX = 0.0f;
	float wheelStart = 0.0f;
	float wheelEnd = 1.5f;
	float wheelDepartureTime = 0.7f;
	// 車輪のスピードがマックスに到達したら、ベクトルを一回作成するためのフラグ
	bool RtoPFlag = false;

	// 押し戻し判定があるかどうかチェックするフラグ
	bool rushFinsh = false;
	bool rushFinshSet = false;
	int wheelTimer2 = 0;
	int wheelEndTime2 = 4 * 60;
	float wheelStart2 = 0.0f;
	float wheelEnd2 = 0.0f;

#pragma endregion

#pragma region 爆破用

	bool blowUpFlag = false;
	bool blowUpSetFlag = false;
	bool blowmatSetFlag = false;
	float randomAngleX = 0, randomAngleY = 0, randomAngleZ = 0;
	float blowStartAngle = (360 * 6) * affine::Deg2Rad;

	Vector3 blowUpVel[19];
	Vector3 blowUpRotaVel={0.4f,0.4f,0.0f};
	WorldTransform Kari;

	int medamawaitTimer = 0;
	int medamaRotaTimer=0;
	int	medamaRotaEndTime = 5 * 60;
	float startMedamaAngle = 0;
	float endMedamaAngle = DegreeToRad(360.0f * 15.0f);
	bool medamaDownFlag = false;
	float medamaGraviti = 0.1;
#pragma endregion

#pragma region 目玉
	WorldTransform medamaWT;
	Model* medamaModel_ = nullptr;
#pragma endregion

};

