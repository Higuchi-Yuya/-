#include"RailCamera.h"
#include"affine.h"
#include<assert.h>
void RailCamera::Initialize(Vector3 trans, Vector3 rot,const WorldTransform* worldTransform) {

	//ワールドトランスフォームの初期設定
	this->worldTransform_.translation_ = trans;
	this->worldTransform_.rotation_ = rot;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();

	input_ = Input::GetInstance();

	worldTransform_.parent_ = worldTransform;
}
void RailCamera::Update() {

	affine::makeMatIdentity(worldTransform_.matWorld_);
	affine::makeMatRot(worldTransform_.matWorld_, worldTransform_.rotation_);
	affine::makeMatTrans(worldTransform_.matWorld_, worldTransform_.translation_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	viewProjection_.eye =Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = affine::MatVector(worldTransform_.matWorld_, forward);
	//視点から前方に適当な距離進んだ位置が注視点
	forward += viewProjection_.eye;
	viewProjection_.target = forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = affine::MatVector(worldTransform_.matWorld_, up);
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(0.0f, 0.0f);
	debugText_->Printf("eye=%f,%f,%f", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
}