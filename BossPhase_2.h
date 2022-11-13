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
private:// �����o�ϐ�
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[19];

	// �f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	// �C���v�b�g
	Input* input_ = nullptr;

	//���f��
	Model* model_ = nullptr;


};

