#include "player.h"
#include"affine.h"
void player::Initialize() {
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	model_ = Model::Create();

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0.0f, 0.0f, 0.0f);
	textureHandle_ = TextureManager::Load("uvChecker.png");

	//3D���e�B�N���̃��[���h�g�����X�t�H�[���̏�����
	worldTransform3DReticle_.Initialize();

	//���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle = TextureManager::Load("2D.png");

	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2{ 640,360 }, Vector4{ 1,1,1,1 }, Vector2(0.5, 0.5)));
}

void player::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<playerBullet>& bullet) {
		return bullet->IsDead();
		});

	//���@�̃��[���h���W����ړ��x�N�g�����v�Z
	Vector3 vectorX = { 1.0,0,0 };
	vectorX = affine::MatVector(worldTransform_.matWorld_, vectorX);
	Vector3 vectorY = { 0,0,1.0 };
	vectorY = affine::MatVector(worldTransform_.matWorld_, vectorY);

	Vector3 move = { 0,0,0 };
	Vector3 rot = { 0,0,0 };
	//�Q�[���p�b�h�̏�Ԃ𓾂�ϐ��iXINPUT�j
	XINPUT_STATE joyState;
	//�W���C�X�e�B�b�N��Ԏ擾
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 0.1;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 0.1;
	}
	////�v���C���[�ړ�����
	if (input_->PushKey(DIK_A))
	{
		move.x = -1 * vectorX.x;
		move.z = -1 * vectorX.z;
	}
	if (input_->PushKey(DIK_D))
	{
		move.x = 1 * vectorX.x;
		move.z = 1 * vectorX.z;
	}
	if (input_->PushKey(DIK_W))
	{
		move.x = 1 * vectorY.x;
		move.z = 1 * vectorY.z;
	}
	if (input_->PushKey(DIK_S))
	{
		move.x = -1 * vectorY.x;
		move.z = -1 * vectorY.z;
	}
	if (input_->PushKey(DIK_Q))
	{
		rot.y = 0.001;
	}
	if (input_->PushKey(DIK_E))
	{
		rot.y = -0.001;
	}
	if (input_->PushKey(DIK_Z))
	{
		rot.x = -0.001;
	}
	if (input_->PushKey(DIK_X))
	{
		rot.x = 0.001;
	}

	worldTransform_.translation_ += move;
	worldTransform_.rotation_.y += rot.y * (180 / PI);
	worldTransform_.rotation_.x += rot.x * (180 / PI);
	//�ړ����E���W
	const float kRotLimit = 30.0 * affine::Deg2Rad;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x, -kRotLimit);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x, +kRotLimit);

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && jumpFlag == 0)
	{
		jumpFlag = 1;
	}

	jump();

	affine::makeAffine(worldTransform_);
	worldTransform_.TransferMatrix();

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	debugText_->SetPos(10, 30);
	debugText_->Printf("%f,%f", worldTransform_.rotation_.x, 180 * affine::Deg2Rad);
}

void player::Attack() {
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = affine::MatVector(worldTransform_.matWorld_, velocity);
		float len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		if (len != 0)
		{
			velocity /= len;
		}
		velocity *= kBulletSpeed;

		//�e�̐������A������
		std::unique_ptr<playerBullet> newBullet = std::make_unique<playerBullet>();
		Vector3 playerWolrdPos = worldTransform_.parent_->translation_;
		playerWolrdPos += worldTransform_.translation_;
		Vector3 playerWolrdRot = worldTransform_.parent_->rotation_;
		playerWolrdRot += worldTransform_.rotation_;
		newBullet->Initialize(GetworldPosition(), velocity);

		//�e�̓o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void player::Draw(ViewProjection& viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	/*model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);*/
	//�e�`��
	for (std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}
void player::DrawUI() {
	sprite2DReticle_->Draw();
}
Vector3 player::GetworldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldpos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];
	return worldpos;
}
void player::OnCollision()
{

}
void player::jump()
{
	if (jumpFlag == 1)
	{
		worldTransform_.translation_.y += gravitySpeed;
		gravitySpeed -= 0.05f;

		if (gravitySpeed <= 0)
		{
 			jumpFlag = 2;
		}
	}

	//������
	if (jumpFlag == 2)
	{
		worldTransform_.translation_.y -= gravitySpeed;
		gravitySpeed += 0.04f;

		if (gravitySpeed >= defGravitySpeed)
		{
			jumpFlag = 0;
			gravitySpeed = defGravitySpeed;
			worldTransform_.translation_.y = 0.0f;
		}
	}
}