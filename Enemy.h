
#pragma once
#include"Model.h"
#include"EnemyBullet.h"
#include<Input.h>
#include<memory>
#include<list>


class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:

	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近する
		Leave,		//離脱する
	};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	void ApproachInitialize();

	void ApproachMove();

	void LeaveMove();

	Vector3 GetWorldPosition();
	//発射間隔
	static const int kFireInterval = 60;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
private:
	//自キャラ
	Player* player_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//弾
	std::list < std::unique_ptr< EnemyBullet >> bullets_;
	//入力処理
	Input* input_ = nullptr;
	//発射タイマー
	int32_t fireTimer = 0;
};
