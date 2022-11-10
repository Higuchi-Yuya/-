#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
class BossPhase_1
{
public:// �����o�֐�
	// �������֐�
	void Initialize(Model* model);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw(ViewProjection viewprojection);

	// �u���b�N���΂�����
	void FlyBlocks(Vector3 playerPos);

	// �u���b�N�������_���ɒ��I���ĕ������鏈��
	void FloatRandomBlocks();

private:// �����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[27];

	// �����Ƃ��̃t���O
	bool RespawnFlag[27];

	DebugText* debugText_ = nullptr;


	//���f��
	Model* model_ = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	// �C���v�b�g
	Input* input_ = nullptr;

	// �����_���ɂƂ邽�߂̕ϐ�
	int randomBlock = 0;

	// �Ē��I����t���O
	bool ReLotteryFlag = true;

	// �u���b�N�𕂂����I���܂ł̃t���O
	bool FloatBlockFlagM = false; // ���W���}�C�i�X
	bool FloatBlockFlagP = false; // ���W���v���X
	bool FloatXRimitFlag = false; // ���ɏo���Ƃ��̐����t���O

	// �u���b�N�𓮂����O�̈ʒu
	Vector3 oldPos;

	// ��΂����u���b�N�̃��X�|�[���t���O
	bool randomRespawnFlag = false;

	// ��΂����u���b�N�v���C���[�Ɍ������ĉ�]����t���O
	bool flyToPlayerFlag = false;

	// �x�N�g��
	Vector3 velocity;
};

