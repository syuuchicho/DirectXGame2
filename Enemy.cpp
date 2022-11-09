#include"Enemy.h"
#include"Player.h"
#include <cassert>
//03_08p19
void Enemy::ApproachInitialize()
{
	// ���˃^�C�}�[��������
	fireTimer = 0;
}

void Enemy::ApproachMove()
{
	// �ړ��x�N�g����ύX���鏈��
	const float approachSpeed = 0.1f;

	// �G�L�����̈ړ��x�N�g��
	Vector3 approachMove = { 0,0,-approachSpeed };

	// ���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;
	// �w�莞�ԂɒB����
	if (fireTimer < 0)
	{
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		fireTimer = kFireInterval;
	}

	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += approachMove;
	// �K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveMove()
{
	// �ړ��x�N�g����ύX���鏈��
	const float leaveSpeed = 0.05f;

	// �G�L�����̈ړ��x�N�g��
	Vector3 leaveMove = { -leaveSpeed,leaveSpeed,0 };

	// ���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;
	// �w�莞�ԂɒB����
	if (fireTimer < 0)
	{
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		fireTimer = kFireInterval;
	}

	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += leaveMove;
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
	ApproachInitialize();
}


Vector3 Enemy::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Update()
{
	// �f�X�t���O�̗������e������
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	switch (phase_)
	{
	case Phase::Approach:

		ApproachMove();
		break;

	case Phase::Leave:

		LeaveMove();
		break;
	}

	// �e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
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

void Enemy::OnCollision()
{
	worldTransform_.translation_.z += 5;
}

void Enemy::Fire()

{
	assert(player_);

	// �e�̑��x
	const float kBulletSpeed = 1.0f;

	// ���L�����̃��[���h���W���擾
	Vector3 playerWP = player_->GetWorldPosition();

	// �G�L�����̃��[���h���W���擾
	Vector3 enemyWP = GetWorldPosition();

	// �G�L�����A���L�����̍����x�N�g��
	Vector3 len = enemyWP - playerWP;

	// �x�N�g���̐��K��
	len = MathUtility::Vector3Normalize(len);

	// �x�N�g���̒����𑬂��ɍ��킹��
	len *= -kBulletSpeed;

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, len);

	// �e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// �e�̕`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}