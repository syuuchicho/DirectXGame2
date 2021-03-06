#include"Enemy.h"
#include <cassert>

void Approach(Vector3& appSpeed, WorldTransform& worldTransform_, Enemy::Phase& phase_, float z)
{

	//移動(ベクトルを加算)
	worldTransform_.translation_ += appSpeed;
	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < z) {
		phase_ = Enemy::Phase::Leave;
	}
}

void Leave(Vector3& leaveSpeed, WorldTransform& worldTransform_)
{
	//移動(ベクトルを加算)
	worldTransform_.translation_ += leaveSpeed;
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
	AppPhaInitialize();
}




void Enemy::Update()
{
	Vector3 appSpeed = { 0,0,-0.3f };
	Vector3 leaveSpeed = { 0,0,+0.3f };
	//行動パターン遷移
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach(appSpeed, worldTransform_, phase_, 0.0f);
		break;
	case Phase::Leave:
		Leave(leaveSpeed, worldTransform_);
		break;
	}
	//デスグラフの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet){
		return bullet->IsDead();
	});

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

	//接近フェーズ初期化
	AppPhaInitialize();
	
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet_ : bullets_) {
		bullet_->Update();
	}
}

void Enemy::Fire()
{
		//弾を生成し、初期化
		std::unique_ptr<EnemyBullet>newBullet=std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullets_.push_back(std::move (newBullet));
		
}

void Enemy::AppPhaInitialize()
{
	//発射タイマーカウントダウン
	shootTimer--;
	//指定の時間に達した
	if (shootTimer <= 0){
		//弾を発射
		Fire();
		//発射タイマーを初期化
		shootTimer = kFireInterval;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet_ : bullets_) {
		bullet_->Draw(viewProjection);
	}
}