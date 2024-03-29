#pragma once
#include"PlayerBullet.h"
#include "Input.h"
#include "DebugText.h"
#include<memory>
//#include<list>

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:	//メンバー関数

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textrueHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textrueHandle);

/// <summary>
/// 更新
/// </summary>
	void Update();

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection &viewProjection);
	
	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate(Input* input_, float& y);

/// <summary>
/// 攻撃
/// </summary>
	void Attack();
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	
	/// <summary>
	///弾 
	/// </summary>
	std::list<std::unique_ptr<PlayerBullet>>bullets_;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:	//メンバー変数
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	//ViewProjection viewProjection_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャ
	uint32_t textureHandle_ = 0u;
	//入力処理
	Input* input_ = nullptr;
	//デバッグテキスト
	DebugText* debugText_ = nullptr;
	//弾
	std::unique_ptr<PlayerBullet>bullet_;
};
