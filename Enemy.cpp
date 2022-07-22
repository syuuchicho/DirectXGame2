#include"Enemy.h"
#include <cassert>

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
	
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("EnemyP.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 30,5,100 };

	//�������W���Z�b�g
	worldTransform_.translation_ = move;

	//�ڋ߃t�F�[�Y������
	AppPhaInitialize();
}




void Enemy::Update()
{
	Vector3 appSpeed = { 0,0,-0.3f };
	Vector3 leaveSpeed = { 0,0,+0.3f };
	//�s���p�^�[���J��
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach(appSpeed, worldTransform_, phase_, 0.0f);
		break;
	case Phase::Leave:
		Leave(leaveSpeed, worldTransform_);
		break;
	}
	//�f�X�O���t�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet){
		return bullet->IsDead();
	});

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

	//�ڋ߃t�F�[�Y������
	AppPhaInitialize();
	
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet_ : bullets_) {
		bullet_->Update();
	}
}

void Enemy::Fire()
{
		//�e�𐶐����A������
		std::unique_ptr<EnemyBullet>newBullet=std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//�e��o�^����
		bullets_.push_back(std::move (newBullet));
		
}

void Enemy::AppPhaInitialize()
{
	//���˃^�C�}�[�J�E���g�_�E��
	shootTimer--;
	//�w��̎��ԂɒB����
	if (shootTimer <= 0){
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		shootTimer = kFireInterval;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet_ : bullets_) {
		bullet_->Draw(viewProjection);
	}
}