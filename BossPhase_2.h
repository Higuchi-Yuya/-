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

	void rushUpdate(Vector3 playerPos);

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

	// �L���[�u�p�̃X�P�[���̕ϐ�
	float kyubuScale = 3.0f;
	float kyubuLengh = 2.0f;

	// �e�̉�]�v�f
	Vector3 oyaRota;
	Matrix4 oyaRotation;

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

#pragma region �ړ��p�ϐ�

	float angle = 90;

	float radius = 50;

#pragma endregion

#pragma region �u�[�������p�ϐ�

#pragma endregion

#pragma region �ːi�p�ϐ�
	// �ːi�J�n�p�̃t���O
	bool rushFlag = false;
	// ������Ƃ��̃|�W�V����Y
	float downPosY = 2.0f;
	// ���̍������L�^����ϐ�
	float originPosY = 0.0f;

	// �ːi���̃{�X�ƃv���C���[�̃x�N�g��
	Vector3 rushToPlayer;

	// �����߂����肪���邩�ǂ����`�F�b�N����t���O
	bool rushFinsh = false;

#pragma endregion

};

