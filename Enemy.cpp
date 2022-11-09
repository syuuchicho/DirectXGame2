#include"Enemy.h"
#include"Player.h"
#include <cassert>
//03_08p19
void Enemy::ApproachInitialize()
{
	// 発射タイマーを初期化
	fireTimer = 0;
}

void Enemy::ApproachMove()
{
	// 移動ベクトルを変更する処理
	const float approachSpeed = 0.1f;

	// 敵キャラの移動ベクトル
	Vector3 approachMove = { 0,0,-approachSpeed };

	// 発射タイマーカウントダウン
	fireTimer--;
	// 指定時間に達した
	if (fireTimer < 0)
	{
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	// 移動(ベクトルを加算)
	worldTransform_.translation_ += approachMove;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveMove()
{
	// 移動ベクトルを変更する処理
	const float leaveSpeed = 0.05f;

	// 敵キャラの移動ベクトル
	Vector3 leaveMove = { -leaveSpeed,leaveSpeed,0 };

	// 発射タイマーカウントダウン
	fireTimer--;
	// 指定時間に達した
	if (fireTimer < 0)
	{
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	// 移動(ベクトルを加算)
	worldTransform_.translation_ += leaveMove;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);

	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("EnemyP.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//キャラクターの移動ベクトル
	Vector3 move = { 30,5,100 };

	//初期座標をセット
	worldTransform_.translation_ = move;

	//接近フェーズ初期化
	ApproachInitialize();
}


Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Update()
{
	// デスフラグの立った弾を解除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	switch (phase_)
	{
	case Phase::Approach:

		ApproachMove();
		break;

	case Phase::Leave:

		LeaveMove();
		break;
	}

	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//単位行列
	worldTransform_.matWorld_ = worldTransform_.CreateIdentityMatrix();
	//スケーリング行列
	worldTransform_.matWorld_ *= worldTransform_.CreateMatScale(worldTransform_.scale_);
	//回転行列
	worldTransform_.matWorld_ *= worldTransform_.CreateMatRot(worldTransform_.rotation_);
	//平行移動
	worldTransform_.matWorld_ *= worldTransform_.CreateMatTrans(worldTransform_.translation_);
	//行列の転送
	worldTransform_.TransferMatrix();
}

void Enemy::OnCollision()
{
	worldTransform_.translation_.z += 5;
}

void Enemy::Fire()

{
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得
	Vector3 playerWP = player_->GetWorldPosition();

	// 敵キャラのワールド座標を取得
	Vector3 enemyWP = GetWorldPosition();

	// 敵キャラ、自キャラの差分ベクトル
	Vector3 len = enemyWP - playerWP;

	// ベクトルの正規化
	len = MathUtility::Vector3Normalize(len);

	// ベクトルの長さを速さに合わせる
	len *= -kBulletSpeed;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, len);

	// 弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}