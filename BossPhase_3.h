#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
class BossPhase_3
{
public:
	// �������֐�
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw(ViewProjection viewprojection);
private:// �����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[19];

	// �����Ƃ��̃t���O
	bool RespawnFlag[27];

	DebugText* debugText_ = nullptr;


	//���f��
	Model* model_ = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	// �C���v�b�g
	Input* input_ = nullptr;

	float speedX=0.1;

	float totalMoveX = 1;
};

