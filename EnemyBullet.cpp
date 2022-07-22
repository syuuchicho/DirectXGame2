#include"EnemyBullet.h"
#include <cassert>
//03_07p4->//03_02p10
void EnemyBullet::Initialize(Model* model, const Vector3& position)
{
	//null�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemyBulletP.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_=position;
}

void EnemyBullet::Update()
{
	//�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	worldTransform_.translation_ += velocity;

	//�P�ʍs��
	worldTransform_.matWorld_ = worldTransform_.CreateIdentityMatrix();
	//�X�P�[�����O�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatScale(worldTransform_.scale_);
	//��]�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatRot(worldTransform_.rotation_);
	//���s�ړ�
	worldTransform_.matWorld_ *= worldTransform_.CreateMatTrans(worldTransform_.translation_);
	//���[���h�s���]��
	worldTransform_.TransferMatrix();
	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}