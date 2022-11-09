#include"Skydome.h"

void Skydome::Initialize(Model*model)
{
	model_ = model;
	//テクスチャ読み込み
	//textureHandle_ = TextureManager::Load("uvChecker.png");
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void Skydome::Update()
{

}

void Skydome::Draw()
{
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection_);
}