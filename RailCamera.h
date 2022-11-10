#pragma once
#include"Model.h"
#include"assert.h"
#include"DebugText.h"
using namespace MathUtility;

class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }


	//ワールド行列を取得
	WorldTransform* GetWorldTrans();

	/// <summary>
	/// デバッグテキスト
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
	//ワールド変換データ
	WorldTransform worldTransform_;


	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	Vector3 eyeBox={0,0,0};
};

