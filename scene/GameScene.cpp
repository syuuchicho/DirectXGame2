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
}

//度数法
float Angle(float const& n)
{
	return n * 3.14 / 180;
}

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
Matrix4 CreateMatRot(Vector3 rotation) {
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

	//範囲forですべてのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		//X,Y,Z 方向のスケーリングを設定
		worldTransform.scale_ = { 1.0f,1.0f,1.0f };

		// X,Y,Z軸周りの回転角を設定
		worldTransform.rotation_ = { angledist(engine),angledist(engine),angledist(engine) };

		//X,Y,Z軸周りの平行移動を設定
		worldTransform.translation_ = { posdist(engine),posdist(engine),posdist(engine) };

		//単位行列を代入
		worldTransform.matWorld_ = CreateIdentityMatrix();

		//ワールド座標に掛け算して代入
		worldTransform.matWorld_ *= CreateMatScale(worldTransform.scale_);
		worldTransform.matWorld_ *= CreateMatRot(worldTransform.rotation_);
		worldTransform.matWorld_ *= CreateMatTrans(worldTransform.translation_);
		//行列の転送
		worldTransform.TransferMatrix();
	}
	//カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };

	//カメラ注視点座標を設定
	//viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(3.14 / 4.0f),sinf(3.14 / 4.0f),0.0f };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}
void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

	//視点移動処理
	{
		//視点の移動ベクトル
		Vector3 move = { 0,0,0 };

		//視点の移動速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move.z += kEyeSpeed;
		}
		else if (input_->PushKey(DIK_S)) {
			move.z -= kEyeSpeed;
		}
		//視点移動(ベクトルの加算)
		viewProjection_.eye += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
			"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	//注視点移動処理
	{
		//注視点の移動ベクトル
		Vector3 move = { 0,0,0 };

		//注視点の移動速さ
		const float kTargetSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move.x += kTargetSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x -= kTargetSpeed;
		}
		//視点移動(ベクトルの加算)
		viewProjection_.target += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	}

	//上方向回転処理
	{
		//上方向の回転速さ[ラジアン/frame]
		const float kUpRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;
			//2πを超えたら0に戻す
			viewAngle = fmodf(viewAngle, 3.14 * 2.0f);
		}

		//上方向ベクトルを計算(半径1の円周上の座標)
		viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
			"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	}
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
	/// </summary>f
	/// 3Dモデル描画
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
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
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}