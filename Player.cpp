#include"Player.h"
#include <cassert>

Vector3 CrossV3M4(Vector3& v, const Matrix4& m)
{
	v.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	v.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	v.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	return v;
}

//����
void Player::Rotate(Input *input_,float &y)
{
	if (input_->PushKey(DIK_Z))
	{
		//Y���܂��̊p�x�𑝉�
		y += 0.01f;
	}
	else if (input_->PushKey(DIK_C))
	{
		//Y���܂��̊p�x������
		y -= 0.01f;
	}
}

//�U��
void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = CrossV3M4(velocity, worldTransform_.CreateMatRot(worldTransform_.rotation_));
		

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_,worldTransform_.translation_,velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	
	return worldPos;
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

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
	});

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

	//�J�����Ǐ]
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

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
	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	
	/*debugText_->SetPos(50, 50);
	debugText_->Printf("PosX:%f", worldTransform_.translation_.x);
	debugText_->SetPos(50, 70);
	debugText_->Printf("PosY:%f", worldTransform_.translation_.y);*/
	//�U��
	Attack();
	//�e�X�V
	for(std::unique_ptr<PlayerBullet>& bullet:bullets_){
		bullet->Update();
	}
}

void Player::SetParent(WorldTransform *worldTransform)
{
	worldTransform_.parent_ = worldTransform;
}

void Player::OnCollision()
{
	
}
void Player::Draw(ViewProjection& viewprojection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewprojection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewprojection);
	}
}
