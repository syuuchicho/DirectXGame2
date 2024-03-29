#pragma once
#include"Model.h"
#include"MathUtility.h"
using namespace MathUtility;
/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position,const Vector3&velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }
	
	Vector3 GetWorldPosition();
	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_=nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};