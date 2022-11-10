#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//���[���h�ϊ��̏����ݒ�
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = position;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();

	//���L�����ƃ��[���J�����̐e�q�֌W������

}

void RailCamera::Update()
{
	//�ړ�(�x�N�g�������Z)
	//worldTransform_.translation_ += Vector3(0, 0, -0.1f);
	//worldTransform_.rotation_ += rotation;
	//worldTransform_.UpdateMatrix();
	//assert(worldTransform_.constBuff_);
	//�P�ʍs��
	worldTransform_.matWorld_ = worldTransform_.CreateIdentityMatrix();
	//�X�P�[�����O�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatScale(worldTransform_.scale_);
	//��]�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatRot(worldTransform_.rotation_);
	//���s�ړ�
	worldTransform_.matWorld_ *= worldTransform_.CreateMatTrans(worldTransform_.translation_);

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//���[���h�O��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = CrossV3M4(forward, worldTransform_.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = CrossV3M4(up, worldTransform_.matWorld_);
	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 100);
	debugText_->Printf("RailCamera Pos:%f,%f,%f",
		viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

WorldTransform* RailCamera::GetWorldTrans() 
{
	//�|�C���^
	WorldTransform* worldTransform;
	//�f�[�^�A�h���X��n��
	worldTransform = &worldTransform_;
	return worldTransform;
}

void RailCamera::Draw()
{

}