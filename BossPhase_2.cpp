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

	// �e
	worldTransform_[0].translation_ = { 50,10,50 };

	// �q�̍��W�ݒ�

	// �^�񒆂̒i
	worldTransform_[1].translation_ = { +2, 0,-2 };
	worldTransform_[2].translation_ = { 0, 0,-2 };
	worldTransform_[3].translation_ = { -2, 0,-2 };
	worldTransform_[4].translation_ = { -2, 0, 0 };
	worldTransform_[5].translation_ = { -2, 0,+2 };
	worldTransform_[6].translation_ = { 0, 0,+2 };
	worldTransform_[7].translation_ = { +2, 0,+2 };
	worldTransform_[8].translation_ = { +2, 0, 0 };

	// ��̒i
	worldTransform_[9].translation_ = { 0,+2, 0 };
	worldTransform_[10].translation_ = { 0,+2,-2 };
	worldTransform_[11].translation_ = { -2,+2, 0 };
	worldTransform_[12].translation_ = { 0,+2,+2 };
	worldTransform_[13].translation_ = { +2,+2, 0 };

	// ���̒i
	worldTransform_[14].translation_ = { 0,-2, 0 };
	worldTransform_[15].translation_ = { 0,-2,-2 };
	worldTransform_[16].translation_ = { -2,-2, 0 };
	worldTransform_[17].translation_ = { 0,-2,+2 };
	worldTransform_[18].translation_ = { +2,-2, 0 };

	// �q
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
		//// ��]�������鎞�Ԃ̏㔼���̊Ԃ���
		//if (beamTimer <= beamTimeInterval / 2) {
		//	// ��]���x�����X�ɏグ�Ă���
		//	if (beamRotaTimer >= beamRotaIntervalP) {
		//		beamRotaTimer = 0;
		//		beamRotaSpeedZ += 0.01f;

		//		// ��]���x������ȏ�オ��Ȃ��悤�ɂ���
		//		if (beamRotaSpeedZ >= 0.5f) {
		//			beamRotaSpeedZ = 0.5f;
		//		}
		//	}
		//}
		//else {
		//	// ��]���x�����X�ɗ��Ƃ��Ă���
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
		//		// �r�[���̉�]���x�� 0 �ȉ��ɂȂ�����r�[�����~�߂�
		//		// ���Z�b�g���s��

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

		// �r�[���ł��n�߂���s��̊|���Z�𒲐�
		if (beamFlag == true) {
			// �r�[���Ŏg�����̂��s��v�Z����
			if (i == 10 || i == 3 || i == 1 || i == 15) {
				//worldTransform_[2].matWorld_ *= worldTransform_[0].matWorld_;
				worldTransform_[i].matWorld_ *= worldTransform_[2].matWorld_;

			}
			else if (i != 0) {
				worldTransform_[i].matWorld_ *= worldTransform_[0].matWorld_;
			}
		}
		// ����ȊO�̎��͊�{�I�ɐe�ƍs��v�Z���s��
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
