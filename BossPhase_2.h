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
public:// �T�u�N���X
	enum class Motion {
		standby,
		beam,
		boomerang,
		rush,
		reflector,
	};

public:// �����o�֐�
	// �������֐�
	void Initialize();

	// �X�V����
	void Update(Vector3 playerPos);

	// �`�揈��
	void Draw(ViewProjection viewprojection);

private:
	void beamUpdate(Vector3 playerPos);

	void boomerangUpdate(Vector3 playerPos);

	// �s��̓]���ƍX�V
	void TransferMat();

	float Lerp(const float& startPos, const float& endPos, const float& timeRate);

	float DegreeToRad(float num)
	{
		return num / 180 * MathUtility::PI;
	}

	float RadToDegree(float num)
	{
		return num / MathUtility::PI * 180;
	}

private:// �����o�ϐ�
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[19];

	// �f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	// �C���v�b�g
	Input* input_ = nullptr;

	//���f��
	Model* model_ = nullptr;

	// �r�[���p�̕ϐ�
	#pragma region �r�[���p�̕ϐ�
	// �r�[���̃��[���h�f�[�^
	WorldTransform beamWorldTransform_;
	// �r�[���̃��f��
	Model* beamModel_ = nullptr;

	bool beamFlag = false;
	bool beamBlockPosSetFlag = false;
	bool rotaflag = false;
	int beamTimer = 0;
	int beamTimeInterval = 8 * 60;

	int beamRotaTimer = 0;
	int beamRotaIntervalP = 10;
	int beamRotaIntervalM = 5;

	float beamRotaSpeedZ = 0.01f;
	float beamRotaSpeedZ_2 = 0.01f;

	#pragma endregion

	

};

