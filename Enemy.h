
#pragma once
#include"Model.h"
#include"EnemyBullet.h"
#include<Input.h>
#include<memory>
#include<list>


class Player;

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:

	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	void ApproachInitialize();

	void ApproachMove();

	void LeaveMove();

	Vector3 GetWorldPosition();
	//���ˊԊu
	static const int kFireInterval = 60;
private:
	//���L����
	Player* player_ = nullptr;
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//�e
	std::list < std::unique_ptr< EnemyBullet >> bullets_;
	//���͏���
	Input* input_ = nullptr;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};
