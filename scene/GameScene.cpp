#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include<random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	//自キャラの解放
	delete player_;
	delete enemy_;
}

//度数法
float Angle(float const& n)
{
	return n * 3.14 / 180;
}

float Radian(float const& n)
{
	return n / 3.14 * 180;
}

//クランプ
float Clamp(float const& min, float const& max, float num)
{
	if (num < min)
	{
		return min;
	}
	else if (num > max)
	{
		return max;
	}
	return num;
}

////スケーリング行列
//Matrix4 CreateMatScale(Vector3 scale) {
//	Matrix4 matScale;
//	matScale.m[0][0] = scale.x;
//	matScale.m[1][1] = scale.y;
//	matScale.m[2][2] = scale.z;
//	matScale.m[3][3] = 1.0f;
//
//	return matScale;
//}
//
////回転行列
//Matrix4 CreateMatRot(Vector3 rotation) {
//	//合成用回転行列を宣言
//	Matrix4 matRot;
//	//各軸用回転行列を宣言
//	Matrix4 matRotX, matRotY, matRotZ;
//
//	matRotZ.m[0][0] = cos(rotation.z);
//	matRotZ.m[0][1] = sin(rotation.z);
//	matRotZ.m[1][0] = -sin(rotation.z);
//	matRotZ.m[1][1] = cos(rotation.z);
//	matRotZ.m[2][2] = 1;
//	matRotZ.m[3][3] = 1;
//
//	matRotX.m[0][0] = 1;
//	matRotX.m[1][1] = cos(rotation.x);
//	matRotX.m[1][2] = sin(rotation.x);
//	matRotX.m[2][1] = -sin(rotation.x);
//	matRotX.m[2][2] = cos(rotation.x);
//	matRotX.m[3][3] = 1;
//
//	matRotY.m[0][0] = cos(rotation.y);
//	matRotY.m[0][2] = -sin(rotation.y);
//	matRotY.m[1][1] = 1;
//	matRotY.m[2][0] = sin(rotation.y);
//	matRotY.m[2][2] = cos(rotation.y);
//	matRotY.m[3][3] = 1;
//
//	//各軸の回転行列を合成
//	matRot = matRotZ;
//	matRot *= matRotX;
//	matRot *= matRotY;
//
//	return matRot;
//}
////平行移動
//Matrix4 CreateMatTrans(Vector3 translation) {
//	//平行移動を宣言
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	matTrans.m[0][0] = 1;
//	matTrans.m[1][1] = 1;
//	matTrans.m[2][2] = 1;
//	matTrans.m[3][0] = translation.x;
//	matTrans.m[3][1] = translation.y;
//	matTrans.m[3][2] = translation.z;
//	matTrans.m[3][3] = 1;
//
//	return matTrans;
//}
////単位行列
//Matrix4 CreateIdentityMatrix()
//{
//	Matrix4 Identity;
//	//単位行列を代入
//	Identity.m[0][0] = 1.0f;
//	Identity.m[1][1] = 1.0f;
//	Identity.m[2][2] = 1.0f;
//	Identity.m[3][3] = 1.0f;
//	return Identity;
//}

#pragma region 旋回



#pragma endregion

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float>posdist(-10.0, 10.0);//座標
	std::uniform_real_distribution<float>angledist(0, 3.14);//角度

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	////カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = Angle(10.0f);

	////アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	//敵の生成
	enemy_ = new Enemy();
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	//敵の初期化
	enemy_->Initialize(model_,textureHandle_);

}
void GameScene::Update() {
	//デバッグカメラの更新
	//debugCamera_->Update();
	//自キャラの更新
	player_->Update();

	//敵の更新
	enemy_->Update();
#pragma region 視点移動処理
	{
	//	//視点の移動ベクトル
	//	Vector3 move = { 0,0,0 };

	//	//視点の移動速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move.z += kEyeSpeed;
	//	}
	//	else if (input_->PushKey(DIK_S)) {
	//		move.z -= kEyeSpeed;
	//	}
	//	//視点移動(ベクトルの加算)
	////	viewProjection_.eye += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 50);
	//	debugText_->Printf(
	//		"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}
#pragma endregion

#pragma region 注視点移動処理
	{
	//	//注視点の移動ベクトル
	//	Vector3 move = { 0,0,0 };

	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move.x += kTargetSpeed;
	//	}
	//	else if (input_->PushKey(DIK_RIGHT)) {
	//		move.x -= kTargetSpeed;
	//	}
	//	//視点移動(ベクトルの加算)
	////	viewProjection_.target += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf(
	//		"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	}
#pragma endregion

#pragma region 上方向回転処理
	{
	//	//上方向の回転速さ[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, 3.14 * 2.0f);
	//	}

	//	//上方向ベクトルを計算(半径1の円周上の座標)
	////	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 90);
	//	debugText_->Printf(
	//		"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	}
#pragma endregion

#pragma region Fov変更処理
	{
		////上キーで視野角を広げる
		//if (input_->PushKey(DIK_UP)) {
		//	//viewProjection_.fovAngleY += Angle(1.0f);
		//	viewProjection_.fovAngleY = Clamp(0.01f, Angle(180.0f), viewProjection_.fovAngleY);
		//}
		////下キーで視野角を広げる
		//if (input_->PushKey(DIK_DOWN)) {
		//	//viewProjection_.fovAngleY -= Angle(1.0f);
		//	viewProjection_.fovAngleY = Clamp(0.01f, Angle(180.0f), viewProjection_.fovAngleY);
		//}

		////行列の再計算
		//viewProjection_.UpdateMatrix();

		////デバッグ用表示
		//debugText_->SetPos(50, 110);
		//debugText_->Printf(
		//	"fovAngleY(Degree):(%f)", Radian(viewProjection_.fovAngleY));
	}
#pragma endregion

#pragma region クリップ距離変更処理
	//{
	//	//上下キーでニアクリップ距離を増減
	//	if (input_->PushKey(DIK_UP))
	//	{
	//		viewProjection_.nearZ += 1.0f;
	//	}
	//	else if (input_->PushKey(DIK_DOWN))
	//	{
	//		viewProjection_.nearZ -= 1.0f;
	//	}

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 130);
	//	debugText_->Printf(
	//		"nearZ:%f", viewProjection_.nearZ);
	//}
#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 0, 0), Vector3(80, 30, 100), Vector4(255, 255, 255, 255));

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//自キャラの描画
	player_->Draw(viewProjection_);
	
	//敵の描画
	enemy_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
	
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}