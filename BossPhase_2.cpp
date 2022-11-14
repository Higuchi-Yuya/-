#include "BossPhase_2.h"
#include <math.h>
float PI = 3.1415926;

void BossPhase_2::Initialize()
{
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();

	for (int i = 0; i < 19; i++) {

		worldTransform_[i].Initialize();

	}

	// 親
	worldTransform_[0].translation_ = { 50,10,50 };

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
	worldTransform_[10].translation_ = { 0,+2,-2 };
	worldTransform_[11].translation_ = { -2,+2, 0 };
	worldTransform_[12].translation_ = { 0,+2,+2 };
	worldTransform_[13].translation_ = { +2,+2, 0 };

	// 下の段
	worldTransform_[14].translation_ = { 0,-2, 0 };
	worldTransform_[15].translation_ = { 0,-2,-2 };
	worldTransform_[16].translation_ = { -2,-2, 0 };
	worldTransform_[17].translation_ = { 0,-2,+2 };
	worldTransform_[18].translation_ = { +2,-2, 0 };

	// 子
	for (int i = 1; i < 19; i++) {
		worldTransform_[i].parent_ = &worldTransform_[0];
	}
}

void BossPhase_2::Update(Vector3 playerPos)
{
	beamFlag = true;
	beamUpdate(playerPos);

	TransferMat();
}

void BossPhase_2::Draw(ViewProjection viewprojection)
{
	for (int i = 0; i < 19; i++) {
		model_->Draw(worldTransform_[i], viewprojection);
	}
}

void BossPhase_2::beamUpdate(Vector3 playerPos)
{
	if (beamFlag == true) {
		if (beamBlockPosSetFlag == false) {
			worldTransform_[10].translation_ = { 0,+2.0f,0 };
			worldTransform_[1].translation_ = { -2.0f,0,0 };
			worldTransform_[3].translation_ = { +2.0f,0,0 };
			worldTransform_[15].translation_ = { 0,-2.0f,0 };
			beamBlockPosSetFlag = true;
		}

		static int timer = 0;
		static int maxTimer1 = 240;
		float timerRate1 = min((float)timer / (float)maxTimer1, 1);
		float timerRate2 = min((float)(timer - maxTimer1) / (float)maxTimer1, 1);

		const static float startAngle = DegreeToRad(0);
		const static float endAngle = DegreeToRad(360 * 4);

		const float powNum = 3;

		timer++;
		if (timer <= maxTimer1)
		{
			worldTransform_[2].rotation_.z = ((endAngle - startAngle) * powf(timerRate1, powNum) + startAngle) * 2;
		}
		else
		{
			worldTransform_[2].rotation_.z = ((endAngle - startAngle) * (powf(timerRate2 - 1, powNum) + 1) + startAngle) * 2;
		}


		//worldTransform_[2].rotation_.z += beamRotaSpeedZ;
		//beamTimer++;
		//beamRotaTimer++;
		//if (worldTransform_[2].rotation_.z >= 6.28319) {
		//	worldTransform_[2].rotation_.z = 0.0f;
		//}
		//// 回転をかける時間の上半分の間だけ
		//if (beamTimer <= beamTimeInterval / 2) {
		//	// 回転速度を徐々に上げていく
		//	if (beamRotaTimer >= beamRotaIntervalP) {
		//		beamRotaTimer = 0;
		//		beamRotaSpeedZ += 0.01f;

		//		// 回転速度がこれ以上上がらないようにする
		//		if (beamRotaSpeedZ >= 0.5f) {
		//			beamRotaSpeedZ = 0.5f;
		//		}
		//	}
		//}
		//else {
		//	// 回転速度を徐々に落としていく
		//	if (beamRotaTimer >= beamRotaIntervalM) {
		//		beamRotaTimer = 0;
		//		
		//		if (rotaflag == false) {
		//			beamRotaSpeedZ -= 0.01f;
		//			if (beamRotaSpeedZ <= 0.04f) {
		//				beamRotaSpeedZ = 0.04f;
		//			}
		//		}
		//		
		//		// ビームの回転速度が 0 以下になったらビームを止める
		//		// リセットを行う

		//		if (worldTransform_[2].rotation_.z <= 5.0f && beamRotaSpeedZ <= 0.04f) {
		//			rotaflag = true;
		//			beamRotaSpeedZ -= 0.01f;
		//			if (beamRotaSpeedZ <= 0.01f) {
		//				beamRotaSpeedZ = 0.01f;
		//			}
		//		}
		//		if ((worldTransform_[2].rotation_.z >= 3.12f*2-2 && worldTransform_[2].rotation_.z <= 3.14f*2) && beamRotaSpeedZ <= 0.01f) {
		//			worldTransform_[2].rotation_.z = 6.28f;
		//			beamFlag = false;
		//			beamBlockPosSetFlag = false;
		//			rotaflag = false;
		//			beamTimer = 0;
		//			beamRotaTimer = 0;
		//			beamRotaSpeedZ = 0.01f;
		//		}
		//	}
		//}

	}
	debugText_->SetPos(20, 120);
	debugText_->Printf("rotaZ:%f", worldTransform_[2].rotation_.z);
	debugText_->SetPos(20, 100);
	debugText_->Printf("time:%d", beamTimer / 60);
}

void BossPhase_2::boomerangUpdate(Vector3 playerPos)
{
}

void BossPhase_2::TransferMat()
{
	for (int i = 0; i < 19; i++) {
		affine::makeAffine(worldTransform_[i]);

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
		}
		// それ以外の時は基本的に親と行列計算を行う
		else if (i != 0) {
			worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
		}


		worldTransform_[i].TransferMatrix();
	}
}

float BossPhase_2::Lerp(const float& startPos, const float& endPos, const float& timeRate)
{
	float dis = endPos - startPos;
	return dis * timeRate + startPos;
}
