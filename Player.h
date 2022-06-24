#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"


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
};
