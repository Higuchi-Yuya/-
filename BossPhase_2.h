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

	double easing_Out(double start, double end, double time, double max_time);


	float DegreeToRad(float num)
	{
		return num / 180 * MathUtility::PI;
	}

	float RadToDegree(float num)
	{
		return num / MathUtility::PI * 180;
	}

	// �r�[���֘A�̃��Z�b�g
	void beamReset();

	void TurnBodyToPlayer(Vector3 playerPos);

	void TurnBeamToPlayer();

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
	bool beamSetFlag = false;
	bool beamOBJSetFlag = false;
	int beamTimer = 0;
	int beamtoPTimer = 0;

	// �����̎���
	int maxTimer = 6 * 60;
	// �r�[���J�n�̎���
	int maxStartTimer = 3 * 60;
	// �r�[�������J�n�̎���
	int maxEndTimer = 6 * 60;
	// �|�W�V�����X�V����Ԋu
	int beamUpdatePosIntaval = 0.3 * 60;
	// ��������
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

	

};

