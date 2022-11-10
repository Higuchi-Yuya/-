#include "BossBullet.h"

void BossBullet::Initialize(Model* model, const WorldTransform worldTransform, const Vector3& OyaPos)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�����Ŏ󂯎�������̂������o�ϐ��ɑ��
	oyaPos = OyaPos;
	oldPos = worldTransform.translation_;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_.x = oyaPos.x + worldTransform.translation_.x;
	worldTransform_.translation_.y = oyaPos.y + worldTransform.translation_.y;
	worldTransform_.translation_.z = oyaPos.z + worldTransform.translation_.z;

	worldTransform_.rotation_ = worldTransform.rotation_;

	debugText_ = DebugText::GetInstance();

	FloatBlockFlagM = false;
	FloatBlockFlagP = false;
	FloatXRimitFlag = false;

	flyToPlayerFlag = false;
	ToPlayerFlag_ = false;
}

void BossBullet::Update(const Vector3& playerPos)
{
	// ���W���ړ�������(�P�t���[�����̈ړ��ʂ𑫂�����)
	FlyBlocks(playerPos);
	

	affine::makeAffine(worldTransform_);
	worldTransform_.TransferMatrix();
}

void BossBullet::Draw(const ViewProjection& viewProjection)
{
	if (ToPlayerFlag_ == false) {
		model_->Draw(worldTransform_, viewProjection);
	}
}

Vector3 BossBullet::GetBulletWorldPosition()
{
	return Vector3();
}

void BossBullet::OnCollision()
{
}

void BossBullet::FlyBlocks(Vector3 playerPos)
{
	if (oyaPos.x < worldTransform_.translation_.x) {
		FloatBlockFlagP = true;
	}
	else if (oyaPos.x > worldTransform_.translation_.x) {
		FloatBlockFlagM = true;
	}

	// �v���X�t���O
	if (FloatBlockFlagP == true && FloatXRimitFlag == false) {
		worldTransform_.translation_.x += 0.1f;

		if (worldTransform_.translation_.x >= oldPos.x + 10.0f) {
			// �����Ɉړ�������t���O���I��
			FloatXRimitFlag = true;
			//���������X�|�[��
			flyToPlayerFlag = true;
			worldTransform_.translation_.x = oldPos.x + 10.0f;

			// �v���C���[�Ɍ������x�N�g���̌v�Z
			velocity_.x = playerPos.x - worldTransform_.translation_.x;
			velocity_.y = playerPos.y - worldTransform_.translation_.y;
			velocity_.z = playerPos.z - worldTransform_.translation_.z;

			velocity_.normalize();
			velocity_ *= 0.3f;
		}
	}

	// �}�C�i�X�t���O
	if (FloatBlockFlagM == true && FloatXRimitFlag == false) {
		worldTransform_.translation_.x -= 0.1f;

		if (worldTransform_.translation_.x <= oldPos.x - 10.0f) {
			// �����Ɉړ�������t���O���I��
			FloatXRimitFlag = true;
			//���������X�|�[��
			flyToPlayerFlag = true;
			worldTransform_.translation_.x = oldPos.x - 10.0f;

			// �v���C���[�Ɍ������x�N�g���̌v�Z
			velocity_.x = playerPos.x - worldTransform_.translation_.x;
			velocity_.y = playerPos.y - worldTransform_.translation_.y;
			velocity_.z = playerPos.z - worldTransform_.translation_.z;

			velocity_.normalize();
			velocity_ *= 0.3f;
		}
	}
	if (ToPlayerFlag_ == false) {
		// �v���C���[�Ɍ������Ĕ�ԏ���
		if (flyToPlayerFlag == true) {
			// ��]���Ȃ�����ł���
			worldTransform_.rotation_.z += 0.5f;

			// �v���C���[�Ɍ������Ĕ��
			worldTransform_.translation_ += velocity_;

			if (worldTransform_.translation_.y <= playerPos.y) {
				flyToPlayerFlag = false;
				ToPlayerFlag_ = true;
			}
		}
	}
	
	debugText_->SetPos(20, 60);
	debugText_->Printf("��Ԃ��:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void BossBullet::SetToPlayerFlag(bool ToPlayerFlag)
{
	ToPlayerFlag_ = ToPlayerFlag;
}


