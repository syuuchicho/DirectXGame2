#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include <d3d12.h>
#include <wrl.h>
#include<math.h>

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = {1, 1, 1};
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = {0, 0, 0};
	// ローカル座標
	Vector3 translation_ = {0, 0, 0};
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	//スケーリング行列
	Matrix4 CreateMatScale(Vector3 scale) {
		Matrix4 matScale;
		matScale.m[0][0] = scale.x;
		matScale.m[1][1] = scale.y;
		matScale.m[2][2] = scale.z;
		matScale.m[3][3] = 1.0f;

		return matScale;
	}

	//回転行列
	Matrix4 CreateMatRot(Vector3  rotation) {
		//合成用回転行列を宣言
		Matrix4 matRot;
		//各軸用回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;

		matRotZ.m[0][0] = cos(rotation.z);
		matRotZ.m[0][1] = sin(rotation.z);
		matRotZ.m[1][0] = -sin(rotation.z);
		matRotZ.m[1][1] = cos(rotation.z);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cos(rotation.x);
		matRotX.m[1][2] = sin(rotation.x);
		matRotX.m[2][1] = -sin(rotation.x);
		matRotX.m[2][2] = cos(rotation.x);
		matRotX.m[3][3] = 1;

		matRotY.m[0][0] = cos(rotation.y);
		matRotY.m[0][2] = -sin(rotation.y);
		matRotY.m[1][1] = 1;
		matRotY.m[2][0] = sin(rotation.y);
		matRotY.m[2][2] = cos(rotation.y);
		matRotY.m[3][3] = 1;

		//各軸の回転行列を合成
		matRot = matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;

		return matRot;
	}
	//平行移動
	Matrix4 CreateMatTrans(Vector3 translation) {
		//平行移動を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		matTrans.m[0][0] = 1;
		matTrans.m[1][1] = 1;
		matTrans.m[2][2] = 1;
		matTrans.m[3][0] = translation.x;
		matTrans.m[3][1] = translation.y;
		matTrans.m[3][2] = translation.z;
		matTrans.m[3][3] = 1;

		return matTrans;
	}
	//単位行列
	Matrix4 CreateIdentityMatrix()
	{
		Matrix4 Identity;
		//単位行列を代入
		Identity.m[0][0] = 1.0f;
		Identity.m[1][1] = 1.0f;
		Identity.m[2][2] = 1.0f;
		Identity.m[3][3] = 1.0f;
		return Identity;
	}
};
