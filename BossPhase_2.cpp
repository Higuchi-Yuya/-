#include "BossPhase_2.h"
#include <math.h>
float PI = 3.1415926;

void BossPhase_2::Initialize()
{
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	beamModel_ = Model::Create();

	for (int i = 0; i < 19; i++) {

		worldTransform_[i].Initialize();

	}

	// 親
	worldTransform_[0].translation_ = { 50,10,50 };
	worldTransform_[0].scale_ = { kyubuScale,kyubuScale,kyubuScale };

	// 子の座標設定
	// 真ん中の段
	worldTransform_[1].translation_ = { +kyubuLengh, 0,-kyubuLengh };
	worldTransform_[2].translation_ = { 0, 0,-kyubuLengh };
	worldTransform_[3].translation_ = { -kyubuLengh, 0,-kyubuLengh };
	worldTransform_[4].translation_ = { -kyubuLengh, 0, 0 };
	worldTransform_[5].translation_ = { -kyubuLengh, 0,+kyubuLengh };
	worldTransform_[6].translation_ = { 0, 0,+kyubuLengh };
	worldTransform_[7].translation_ = { +kyubuLengh, 0,+kyubuLengh };
	worldTransform_[8].translation_ = { +kyubuLengh, 0, 0 };

	// 上の段
	worldTransform_[9].translation_ = { 0,+kyubuLengh, 0 };
	worldTransform_[10].translation_ = { 0,+kyubuLengh,-kyubuLengh };
	worldTransform_[11].translation_ = { -kyubuLengh,+kyubuLengh, 0 };
	worldTransform_[12].translation_ = { 0,+kyubuLengh,+kyubuLengh };
	worldTransform_[13].translation_ = { +kyubuLengh,+kyubuLengh, 0 };

	// 下の段
	worldTransform_[14].translation_ = { 0,-kyubuLengh, 0 };
	worldTransform_[15].translation_ = { 0,-kyubuLengh,-kyubuLengh };
	worldTransform_[16].translation_ = { -kyubuLengh,-kyubuLengh, 0 };
	worldTransform_[17].translation_ = { 0,-kyubuLengh,+kyubuLengh };
	worldTransform_[18].translation_ = { +kyubuLengh,-kyubuLengh, 0 };

	// 子
	for (int i = 1; i < 19; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}

	beamWorldTransform_.Initialize();
}

void BossPhase_2::Update(Vector3 playerPos)
{

	angle += 0.9;
	if (angle >= 360)
	{
		angle = 0;
	}

	worldTransform_[0].translation_.x = 50 * cos(angle * affine::Deg2Rad);
	worldTransform_[0].translation_.z = 50 * sin(angle * affine::Deg2Rad);

	if (input_->TriggerKey(DIK_1))
	{
		beamReset();
	}

	if (beamOBJSetFlag == false) {
		TurnBodyToPlayer(playerPos);
	}

	beamUpdate(playerPos);

	TransferMat();
}

void BossPhase_2::Draw(ViewProjection viewprojection)
{
	for (int i = 0; i < 19; i++) {
		model_->Draw(worldTransform_[i], viewprojection);

	}
	// ビームを打ち始めたら描画
	if (beamSetFlag == true) {
		beamModel_->Draw(beamWorldTransform_, viewprojection);
	}

}

void BossPhase_2::beamUpdate(Vector3 playerPos)
{
	if (beamFlag == true) {
		if (beamSetFlag == false) {
			beamWorldTransform_.scale_.x = 0.5f;
			beamWorldTransform_.scale_.y = 0.5f;
			worldTransform_[10].translation_ = { 0,+kyubuLengh,0 };
			worldTransform_[1].translation_ = { -kyubuLengh,0,0 };
			worldTransform_[3].translation_ = { +kyubuLengh,0,0 };
			worldTransform_[15].translation_ = { 0,-kyubuLengh,0 };
			beamSetFlag = true;
		}

		// 今の時間

		// どれくらいの間動くかの半分の時間
		float timerRate1 = min((float)beamTimer / (float)maxTimer, 1);
		float timerRate2 = min((float)(beamTimer - maxTimer) / (float)maxTimer, 1);

		const static float startAngle = DegreeToRad(0);
		// 時間内にどれくらい回転をさせるか
		const static float endAngle = DegreeToRad(360 * 6);

		const float powNum = 3;

		beamTimer++;

		if (beamTimer <= maxTimer)
		{
			worldTransform_[2].rotation_.z = ((endAngle - startAngle) * powf(timerRate1, powNum) + startAngle) * 2;

		}
		else
		{
			worldTransform_[2].rotation_.z = ((endAngle - startAngle) * (powf(timerRate2 - 1, powNum) + 1) + startAngle) * 2;
		}

		if (beamTimer < maxStartTimer) {
			beamToPlayerVel = playerPos - worldTransform_[0].translation_;
		}

		// 打ってる時の動作
		if (beamTimer >= maxStartTimer) {
			if (beamOBJSetFlag == false) {
				//beamWorldTransform_.translation_ = worldTransform_[0].translation_;
				oldPlayerPos = playerPos;
				// 打つ方向に向けてオブジェクトを回転させる
				Vector3 velocity = playerPos - worldTransform_[0].translation_;
				velocity.normalize();
				beamToPlayerVel = velocity;
				beamToPlayerVel *= beamSpeed;

				beamOBJSetFlag = true;
			}
			TurnBeamToPlayer();
			beamtoPTimer++;
			if (beamtoPTimer == beamUpdatePosIntaval) {
				beamtoPTimer = 0;
				oldVelocity = playerPos - oldPlayerPos;
				oldVelocity.normalize();
				oldVelocity *= 1.0f;
			}
			if ((oldPlayerPos.x < playerPos.x - 0.5f || oldPlayerPos.x > playerPos.x + 0.5f) ||
				(oldPlayerPos.z < playerPos.z - 0.5f || oldPlayerPos.z > playerPos.z + 0.5f)||
				(oldPlayerPos.y < playerPos.y - 0.5f || oldPlayerPos.y > playerPos.y + 0.5f)) {
				oldPlayerPos += oldVelocity;
			}
			
			// ビームの長さの固定
			if (beamWorldTransform_.scale_.z < 80)
			{
				beamWorldTransform_.scale_.z += beamSpeed;
				beamWorldTransform_.matWorld_.m[3][0] += beamToPlayerVel.x;
				beamWorldTransform_.matWorld_.m[3][1] += beamToPlayerVel.y;
				beamWorldTransform_.matWorld_.m[3][2] += beamToPlayerVel.z;
			}
		}

		// うち終わりの処理
		if (beamTimer >= maxEndTimer) {
			beamWorldTransform_.scale_.x = (float)easing_Out(0.5f, 0.0f, convergenceTimer, maxConvergenceT);
			beamWorldTransform_.scale_.y = (float)easing_Out(0.5f, 0.0f, convergenceTimer, maxConvergenceT);
			convergenceTimer++;
			if (convergenceTimer >= maxConvergenceT) {
				convergenceTimer = maxConvergenceT;
			}
			if (beamWorldTransform_.scale_.x <= 0.0f) {
				beamFlag = false;
				beamOBJSetFlag = false;
				worldTransform_[1].translation_ = { +2, 0,-2 };
				worldTransform_[3].translation_ = { -2, 0,-2 };
				worldTransform_[10].translation_ = { 0,+2,-2 };
				worldTransform_[15].translation_ = { 0,-2,-2 };
			}
		}
	}

	debugText_->SetPos(20, 100);
	debugText_->Printf("beamX:%f", beamWorldTransform_.translation_.x);
	debugText_->SetPos(20, 120);
	debugText_->Printf("pos:%f", worldTransform_[0].matWorld_.m[3][0]);
	debugText_->SetPos(20, 140);
	debugText_->Printf("scale:%f", beamWorldTransform_.scale_.z);
}

void BossPhase_2::boomerangUpdate(Vector3 playerPos)
{
}

void BossPhase_2::rushUpdate(Vector3 playerPos)
{

}

void BossPhase_2::TransferMat()
{
	for (int i = 0; i < 19; i++) {
		
		affine::makeAffine(worldTransform_[i]);
		affine::makeAffine(beamWorldTransform_);
		
		// ビーム打ち始めたら行列の掛け算を調整
		if (beamFlag == true) {
			// ビームで使うものを行列計算する
			if (i == 10 || i == 3 || i == 1 || i == 15) {
				//worldTransform_[2].matWorld_ *= worldTransform_[0].matWorld_;
				worldTransform_[i].matWorld_ *= worldTransform_[2].matWorld_;

			}
			else if (i != 0) {
				worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
			}
			// ビームオブジェの行列の転送

			beamWorldTransform_.matWorld_ *= worldTransform_[0].matWorld_;
		}
		// それ以外の時は基本的に親と行列計算を行う
		else if (i != 0) {
			worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
		}

		worldTransform_[i].TransferMatrix();
	}

	beamWorldTransform_.TransferMatrix();
}

float BossPhase_2::Lerp(const float& startPos, const float& endPos, const float& timeRate)
{
	float dis = endPos - startPos;
	return dis * timeRate + startPos;
}

double BossPhase_2::easing_Out(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * (1 - (1 - time) * (1 - time)));
}

void BossPhase_2::beamReset()
{
	beamFlag = true;
	beamSetFlag = false;
	beamOBJSetFlag = false;

	beamWorldTransform_.translation_={0.0f,0.0f,0.0f};
	beamWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	beamWorldTransform_.rotation_ = { 0.0f,0.0f,0.0f };

	worldTransform_[2].rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_[1].translation_ = { +kyubuLengh, 0,-kyubuLengh };
	worldTransform_[3].translation_ = { -kyubuLengh, 0,-kyubuLengh };
	worldTransform_[10].translation_ = { 0,+kyubuLengh,-kyubuLengh };
	worldTransform_[15].translation_ = { 0,-kyubuLengh,-kyubuLengh };

	beamTimer = 0;
	beamtoPTimer = 0;
	convergenceTimer = 0;
}

void BossPhase_2::TurnBodyToPlayer(Vector3 playerPos)
{
	// 打つ方向に向けてオブジェクトを回転させる
	Vector3 velocity = playerPos - worldTransform_[0].translation_;
	velocity.normalize();

	// Y軸周り角度(θy)
	worldTransform_[0].rotation_.y = -std::atan2(velocity.z, velocity.x) - DegreeToRad(90);

	// Y軸周りに-θy回す回転行列を計算
	Matrix4 RotY;
	affine::makeMatRotY(RotY, -worldTransform_[0].rotation_.y);

	// velosity_に回転行列を掛け算してvelosityZを求める
	Vector3 velocityZ = velocity;
	velocityZ = affine::MatVector(RotY, velocityZ);

	// X軸周り角度(θx)
	worldTransform_[0].rotation_.x = std::atan2(velocityZ.y, -velocityZ.z);
}

void BossPhase_2::TurnBeamToPlayer()
{
	// 打つ方向に向けてオブジェクトを回転させる
	Vector3 velocity = oldPlayerPos - worldTransform_[0].translation_;
	velocity.normalize();

	// Y軸周り角度(θy)
	worldTransform_[0].rotation_.y = -std::atan2(velocity.z, velocity.x) - DegreeToRad(90);
	
	// Y軸周りに-θy回す回転行列を計算
	Matrix4 RotY;
	affine::makeMatRotY(RotY, -worldTransform_[0].rotation_.y);

	// velosity_に回転行列を掛け算してvelosityZを求める
	Vector3 velocityZ = velocity;
	velocityZ = affine::MatVector(RotY, velocityZ);

	// X軸周り角度(θx)
	worldTransform_[0].rotation_.x = std::atan2(velocityZ.y, -velocityZ.z);
	
}
