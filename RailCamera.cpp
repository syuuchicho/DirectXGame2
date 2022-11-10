#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//ワールド変換の初期設定
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = position;
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();

	//自キャラとレールカメラの親子関係を結ぶ

}

void RailCamera::Update()
{
	//移動(ベクトルを加算)
	//worldTransform_.translation_ += Vector3(0, 0, -0.1f);
	//worldTransform_.rotation_ += rotation;
	//worldTransform_.UpdateMatrix();
	//assert(worldTransform_.constBuff_);
	//単位行列
	worldTransform_.matWorld_ = worldTransform_.CreateIdentityMatrix();
	//スケーリング行列
	worldTransform_.matWorld_ *= worldTransform_.CreateMatScale(worldTransform_.scale_);
	//回転行列
	worldTransform_.matWorld_ *= worldTransform_.CreateMatRot(worldTransform_.rotation_);
	//平行移動
	worldTransform_.matWorld_ *= worldTransform_.CreateMatTrans(worldTransform_.translation_);

	//ワールド行列の平行移動成分を取得(ワールド座標)
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//ワールド前方
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = CrossV3M4(forward, worldTransform_.matWorld_);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = CrossV3M4(up, worldTransform_.matWorld_);
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 100);
	debugText_->Printf("RailCamera Pos:%f,%f,%f",
		viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

WorldTransform* RailCamera::GetWorldTrans() 
{
	//ポインタ
	WorldTransform* worldTransform;
	//データアドレスを渡す
	worldTransform = &worldTransform_;
	return worldTransform;
}

void RailCamera::Draw()
{

}