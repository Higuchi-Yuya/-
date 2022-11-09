#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


class BossPhase_1
{
public:// �����o�֐�
	// �������֐�
	void Initialize(Model* model);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:// �����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[27];

	//���f��
	Model* model_[27];

	//�f�X�t���O
	bool isDead_ = false;

};

