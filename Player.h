#pragma once
#include"PlayerBullet.h"
#include "Input.h"
#include "DebugText.h"
#include<memory>
#include<list>

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:	//�����o�[�֐�

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="textrueHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textrueHandle);

/// <summary>
/// �X�V
/// </summary>
	void Update();

/// <summary>
/// �`��
/// </summary>
/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection &viewProjection);
	
	/// <summary>
	/// ����
	/// </summary>
	void Rotate(Input* input_, float& y);

/// <summary>
/// �U��
/// </summary>
	void Attack();
	
	/// <summary>
	///�e 
	/// </summary>
	std::list<std::unique_ptr<PlayerBullet>>bullets_;

	//���[���h���W���擾
	Vector3 GetWorldPosition();

private:	//�����o�[�ϐ�
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	//ViewProjection viewProjection_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`��
	uint32_t textureHandle_ = 0u;
	//���͏���
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;
	//�e
	std::unique_ptr<PlayerBullet>bullet_;
};
