#include"Enemy.h"
#include <cassert>
//03_05p7
void Approach(Vector3& appSpeed, WorldTransform& worldTransform_, Enemy::Phase& phase_, float z)
{

	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += appSpeed;
	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < z) {
		phase_ = Enemy::Phase::Leave;
	}
}

void Leave(Vector3& leaveSpeed, WorldTransform& worldTransform_)
{
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += leaveSpeed;
}


void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("EnemyP.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,5,100 };

	//�������W���Z�b�g
	worldTransform_.translation_ = move;
}

void Enemy::Update()
{
	Vector3 appSpeed = { 0,0,-0.3f };
	Vector3 leaveSpeed = { 0,0,+0.3f };
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach(appSpeed, worldTransform_, phase_, 0.0f);
		break;
	case Phase::Leave:
		Leave(leaveSpeed, worldTransform_);
		break;
	}

	//�P�ʍs��
	worldTransform_.matWorld_ = worldTransform_.CreateIdentityMatrix();
	//�X�P�[�����O�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatScale(worldTransform_.scale_);
	//��]�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatRot(worldTransform_.rotation_);
	//���s�ړ�
	worldTransform_.matWorld_ *= worldTransform_.CreateMatTrans(worldTransform_.translation_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}