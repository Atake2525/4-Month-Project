#include "CollisionObject3d.h"
#include "kMath.h"
#include "WinApp.h"
#include "CollisionModel.h"
#include "CollisionManager.h"
#include "Camera.h"
#include <fstream>
#include <sstream>
#include <cassert>

using namespace Microsoft::WRL;

void CollisionObject3d::Initialize() {

	//// Resourceの作成
	CreateTransformationMatrixResrouce();
	CreateLightResource();
	CreateCameraResource();

	// 書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	// 平行光源リソースに書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	// 点光源リソースに書き込むためのアドレスを取得
	pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));
	// スポットライトリソースに書き込むためのアドレスを取得
	spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData));
	// 書き込むためのアドレスを取得
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

	// 単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
	transformationMatrix->World = MakeIdentity4x4();

	// 平行光源にデータを書き込む
	directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData->intensity = 0.0f;
	directionalLightData->specularColor = { 1.0f, 1.0f, 1.0f };

	// 点光源にデータを書き込む
	pointLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLightData->position = { 0.0f, 2.0f, 0.0f };
	pointLightData->intensity = 0.0f;
	pointLightData->radius = 5.0f;
	pointLightData->dacay = 5.0f;
	pointLightData->specularColor = { 1.0f, 1.0f, 1.0f };

	// スポットライトにデータを書き込む
	spotLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	spotLightData->position = { 0.0f, 0.0f, 0.0f };
	spotLightData->distance = 10.0f;
	spotLightData->direction = Normalize({ -1.0f, 0.0f, 0.0f });
	spotLightData->intensity = 0.0f;
	spotLightData->dacay = 2.0f;
	spotLightData->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData->cosFalloffStart = std::cos(std::numbers::pi_v<float> / 2.6f);
	//spotLightData->specularColor = { 1.0f, 1.0f, 1.0f };

	// cosFalloffStartがcosAngleより下にならないように調整
	spotLightData->cosFalloffStart = max(spotLightData->cosFalloffStart, spotLightData->cosAngle);

	transform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	first = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	aabb = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	//angle = 0.0f;

	SetAxisAngle({ 0.0f, 0.1f, 0.0f });

	cameraData->worldPosition = { 1.0f, 1.0f, 1.0f };

	camera = Object3dBase::GetInstance()->GetDefaultCamera();


}

void CollisionObject3d::Update() {

	//angle += SwapRadian(1.0f);
	//SetQuaternionAngle();

	// 3DのTransform処理
	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	worldMatrix = Multiply(worldMatrix, rotateQuaternionMatrix);

	if (isParent)
	{
		worldMatrix = Multiply(worldMatrix, parent);
	}

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrix->WVP = worldViewProjectionMatrix;
	transformationMatrix->World = worldMatrix;

	Vector3 worldPos = { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] };

	aabb.min = first.min + worldPos;
	aabb.max = first.max + worldPos;
}

void CollisionObject3d::Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResourced, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResourced, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResourced) {

	if (model_) {
		model_->SetIA();
	}

	Object3dBase::GetInstance()->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(4, directionalLightResourced->GetGPUVirtualAddress());

	Object3dBase::GetInstance()->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLightResourced->GetGPUVirtualAddress());

	Object3dBase::GetInstance()->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLightResourced->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	Object3dBase::GetInstance()->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	Object3dBase::GetInstance()->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraResource->GetGPUVirtualAddress());

	// 3Dモデルが割り当てられていれば描画する
	if (model_) {
		model_->Draw();
	}
}

void CollisionObject3d::CreateTransformationMatrixResrouce() {
	transformationMatrixResource = Object3dBase::GetInstance()->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));
}

void CollisionObject3d::CreateLightResource() {
	CreateDirectionalLightResource();
	CreatePointLightResource();
	CreateSpotLightResource();
}

void CollisionObject3d::CreateDirectionalLightResource() {
	directionalLightResource = Object3dBase::GetInstance()->GetDxBase()->CreateBufferResource(sizeof(DirectionalLight));
}

void CollisionObject3d::CreatePointLightResource() {
	pointLightResource = Object3dBase::GetInstance()->GetDxBase()->CreateBufferResource(sizeof(PointLight));
}

void CollisionObject3d::CreateSpotLightResource() {
	spotLightResource = Object3dBase::GetInstance()->GetDxBase()->CreateBufferResource(sizeof(SpotLight));
}

void CollisionObject3d::CreateCameraResource() {
	cameraResource = Object3dBase::GetInstance()->GetDxBase()->CreateBufferResource(sizeof(CameraForGPU));
}

void CollisionObject3d::SetDirectionalLight(DirectionalLight* lightData) {
	directionalLightData = lightData;
}

void CollisionObject3d::SetPointLight(PointLight* lightData) {
	pointLightData = lightData;
}

void CollisionObject3d::SetSpotLight(SpotLight* lightData) {
	spotLightData = lightData;
}

void CollisionObject3d::SetModel(const std::string& filePath) {
	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);
	CreateAABB();
}

void CollisionObject3d::SetColor(const Vector4& color) {
	model_->SetColor(color);
}

const Vector4& CollisionObject3d::GetColor() const {
	return model_->GetColor();
}

const bool& CollisionObject3d::GetEnableLighting() const {
	return model_->GetEnableLighting();
}

void CollisionObject3d::SetEnableLighting(const bool& enableLighting) {
	model_->SetEnableLighting(enableLighting);
}

const Vector3& CollisionObject3d::GetRotateInDegree() const {
	return SwapDegree(transform.rotate);
}

void CollisionObject3d::SetRotateInDegree(const Vector3& rotate) {
	transform.rotate = SwapRadian(rotate);
}

void CollisionObject3d::SetTransform(const Vector3& translate, const Vector3& scale, const Vector3& rotate) {
	transform.translate = translate;
	transform.scale = scale;
	transform.rotate = rotate;
}

//const Vector3& Object3d::GetSpecularColor() const { 
//	return model_->GetSpecularColor();
//}

const float& CollisionObject3d::GetShininess() const {
	return model_->GetShininess();
}

//void Object3d::SetSpecularColor(const Vector3& specularColor) {
//	model_->SetSpecularColor(specularColor); 
//}

void CollisionObject3d::SetShininess(const float& shininess) {
	model_->SetShininess(shininess);
}

void CollisionObject3d::CreateAABB() {
	const std::vector<VertexData> vData = model_->GetVertices();

	for (VertexData vertices : vData)
	{
		first.min.x = min(first.min.x, vertices.position.x);
		first.min.y = min(first.min.y, vertices.position.y);
		first.min.z = min(first.min.z, vertices.position.z);

		first.max.x = max(first.max.x, vertices.position.x);
		first.max.y = max(first.max.y, vertices.position.y);
		first.max.z = max(first.max.z, vertices.position.z);
	}
}

const bool& CollisionObject3d::CheckCollision(Object3d* object) const {
	return CollisionAABB(aabb, object->GetAABB());
}

//const bool& Object3d::CheckCollisionSphere(const Sphere& sphere) const {
//	return CollisionAABBSphere(aabb, sphere);
//}
