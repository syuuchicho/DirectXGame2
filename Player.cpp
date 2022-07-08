#include"Player.h"
#include <cassert>
//03_03

//����
void Player::Rotate(Input *input_,float &y)
{
	if (input_->PushKey(DIK_Z))
	{
		//Y���܂��̊p�x�𑝉�
		y += 0.03f;
	}
	else if (input_->PushKey(DIK_C))
	{
		//Y���܂��̊p�x������
		y -= 0.03f;
	}
}

//�U��
void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_,worldTransform_.translation_);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

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

///
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
	worldTransform_.matWorld_ = worldTransform_.CreateIdentityMatrix();
	//�X�P�[�����O�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatScale(worldTransform_.scale_);
	//��]�s��
	worldTransform_.matWorld_ *= worldTransform_.CreateMatRot(worldTransform_.rotation_);
	//���s�ړ�
	worldTransform_.matWorld_ *= worldTransform_.CreateMatTrans(worldTransform_.translation_);

	//����
	Rotate(input_,worldTransform_.rotation_.y );
	
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
	//�U��
	Attack();
	//�e�X�V
	for(std::unique_ptr<PlayerBullet>& bullet:bullets_){
		bullet->Update();
	}

}

void Player::Draw(ViewProjection& viewprojection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewprojection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewprojection);
	}
}

