#include"Player.h"
#include <cassert>
//05.02


//�X�P�[�����O�s��
Matrix4 CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;

	return matScale;
}
//��]�s��
Matrix4 CreateMatRot(Vector3 rotation) {
	//�����p��]�s���錾
	Matrix4 matRot;
	//�e���p��]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	matRotY.m[3][3] = 1;

	//�e���̉�]�s�������
	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;
}
//���s�ړ�
Matrix4 CreateMatTrans(Vector3 translation) {
	//���s�ړ���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	matTrans.m[3][3] = 1;

	return matTrans;
}
//�P�ʍs��
Matrix4 CreateIdentityMatrix()
{
	Matrix4 Identity;
	//�P�ʍs�����
	Identity.m[0][0] = 1.0f;
	Identity.m[1][1] = 1.0f;
	Identity.m[2][2] = 1.0f;
	Identity.m[3][3] = 1.0f;
	return Identity;
}

//void Move(Input* input_, Vector3 move, const float &moveSpeed)
//{
//
//	if (input_->PushKey(DIK_RIGHT)) {
//		move.x += moveSpeed;
//	}
//	else if (input_->PushKey(DIK_LEFT)) {
//		move.x -= moveSpeed;
//	}
//	if (input_->PushKey(DIK_UP)) {
//		move.y += moveSpeed;
//	}
//	if (input_->PushKey(DIK_DOWN)) {
//		move.y -= moveSpeed;
//	}
//}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update() {
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	float moveSpeed = 0.1f;

	if (input_->PushKey(DIK_RIGHT)) {
		move.x += moveSpeed;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move.x -= moveSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += moveSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= moveSpeed;
	}

	worldTransform_.translation_ += move;
	//�P�ʍs��
	worldTransform_.matWorld_ = CreateIdentityMatrix();
	//�X�P�[�����O�s��
	worldTransform_.matWorld_ *= CreateMatScale(worldTransform_.scale_);
	//��]�s��
	worldTransform_.matWorld_ *= CreateMatRot(worldTransform_.rotation_);
	//���s�ړ�
	worldTransform_.matWorld_ *= CreateMatTrans(worldTransform_.translation_);

	if (input_->PushKey(DIK_Z))
	{
		//Y���܂��̊p�x�𑝉�
		worldTransform_.rotation_.y += 0.03f;
	}
	else if (input_->PushKey(DIK_C))
	{
		//Y���܂��̊p�x������
		worldTransform_.rotation_.y -= 0.03f;
	}

	//�ړ����E����
	const float kMoveLimitX = 20;
	const float kMoveLimitY = 15;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�s��̓]��
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("PosX:%f", worldTransform_.translation_.x);
	debugText_->SetPos(50, 70);
	debugText_->Printf("PosY:%f", worldTransform_.translation_.y);
}

void Player::Draw(ViewProjection& viewprojection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewprojection, textureHandle_);
}
