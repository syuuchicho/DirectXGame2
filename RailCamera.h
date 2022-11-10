#pragma once
#include"Model.h"
#include"assert.h"
#include"DebugText.h"
using namespace MathUtility;

class RailCamera
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �r���[�v���W�F�N�V�������擾
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }


	//���[���h�s����擾
	WorldTransform* GetWorldTrans();

	/// <summary>
	/// �f�o�b�O�e�L�X�g
	/// </summary>
	void Draw();


	Vector3 CrossV3M4(Vector3& v, const Matrix4& m)
	{
		v.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
		v.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
		v.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
		return v;
	}
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;


	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	Vector3 eyeBox={0,0,0};
};

