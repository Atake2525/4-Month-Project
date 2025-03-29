#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "AABB.h"
#include "kMath.h"
#include "Quaternion.h"

#pragma once

class Model;
class Camera;

class CollisionObject3d {
public: // �����o�֐�
	// ������
	void Initialize();

	// �X�V
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="worldPos">CameraPosition</param>
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResourced);

	void SetModel(const std::string& filePath);

	void SetDirectionalLight(DirectionalLight* lightData);

	void SetPointLight(PointLight* lightData);

	void SetSpotLight(SpotLight* lightData);

	void SetCamera(Camera* camera) { this->camera = camera; }

	// Parent��o�^(�q)
	void SetParent(const Matrix4x4& worldMatrix) {
		parent = worldMatrix;
		isParent = true;
	}

	// Parent��j��
	void DeleteParent() {
		isParent = false;
	}

private:

	Transform transform;

	Vector3 axisAngle;

	//float angle = 0.0f;

	Matrix4x4 rotateQuaternionMatrix;

	Matrix4x4 parent;
	bool isParent = false;
	//Transform* parent = nullptr;

	Camera* camera = nullptr;

private:

	struct CameraForGPU {
		Vector3 worldPosition;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	// ���W�ϊ����\�[�X�̃o�b�t�@���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	// ���W�ϊ��s�񃊃\�[�X���̃f�[�^���w���|�C���^
	TransformationMatrix* transformationMatrix = nullptr;

	// ���s�������\�[�X�̃o�b�t�@���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;
	// ���s�������\�[�X���̃f�[�^���w���|�C���^
	DirectionalLight* directionalLightData = nullptr;

	// �_�������\�[�X�̃o�b�t�@���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource;
	// �_�������\�[�X���̃f�[�^���w���|�C���^
	PointLight* pointLightData = nullptr;

	// �X�|�b�g���C�g���\�[�X�̃o�b�t�@���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource;
	// �X�|�b�g���C�g���\�[�X���̃f�[�^���w���|�C���^
	SpotLight* spotLightData = nullptr;

	// PhongShading�p�J����
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource;
	CameraForGPU* cameraData = nullptr;

	Model* model_ = nullptr;

	// �Փ˔���ɕK�v

	// Getter�ɕԂ��悤��AABB(���W���X�V����)
	AABB aabb;

	// �����ʒu��AABB
	AABB first;

	Matrix4x4 worldMatrix;

public:

	// Getter(Transform)
	const Transform& GetTransform() const { return transform; }
	// Getter(Translate)
	const Vector3& GetTranslate() const { return transform.translate; }
	// Getter(Scale)
	const Vector3& GetScale() const { return transform.scale; }
	// Getter(Rotate)
	const Vector3& GetRotate() const { return transform.rotate; }
	// Getter(Rotate Degree)
	const Vector3& GetRotateInDegree() const;
	// Gettre(Color)
	const Vector4& GetColor() const;
	// Getter(EnableLighting)
	const bool& GetEnableLighting() const;
	// Getter(specularColor)
	//const Vector3& GetSpecularColor() const;
	// Getter(shininess)
	const float& GetShininess() const;
	// Getter(AABB)
	const AABB& GetAABB() const { return aabb; }
	// Getter(worldMatrix)
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }

	// Setter(Transform)
	void SetTransform(const Transform& transform) { this->transform = transform; }
	// Setter(Transform, pos,scale,rotate)
	void SetTransform(const Vector3& translate, const Vector3& scale, const Vector3& rotate);
	// Setter(Translate)
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	// Setter(Scale)
	void SetScale(const Vector3& scale) { transform.scale = scale; }
	// Setter(Rotate)
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	// Setter(Rotate Degree)
	void SetRotateInDegree(const Vector3& rotate);
	// Setter(Color)
	void SetColor(const Vector4& color);
	// Setter(EnableLighting)
	void SetEnableLighting(const bool& enableLighting);
	// Setter(specularColor)
	//void SetSpecularColor(const Vector3& specularColor);
	// Setter(shininess)
	void SetShininess(const float& shininess);
	// �C�ӎ���]�̎����w��̉�]�p�ɕύX
	void SetAxisAngle(const Vector3& rotate) { axisAngle = Normalize(rotate); }
	// �C�ӎ���]�̉�]�ʂ�ݒ�
	void SetQuaternionAngle(const float& angle) { rotateQuaternionMatrix = MakeRotateAxisAngle(axisAngle, angle); }


public:
	// �Փ˃`�F�b�N(AABB��AABB)
	//const bool& CheckCollision(Object3d* object) const;

	//const bool& CheckCollisionSphere(const Sphere& sphere) const;

private:

	// TransformationMatrixResource�����
	void CreateTransformationMatrixResrouce();
	// LightResource�����
	void CreateLightResource();
	// DirectionalLightResource�����
	void CreateDirectionalLightResource();
	// PointLightResource�����
	void CreatePointLightResource();
	// SpotLightResource�����
	void CreateSpotLightResource();
	// CameraResource�����
	void CreateCameraResource();

	// AABB�����f�����Q�Ƃ��Ď����I�ɍ쐬
	void CreateAABB();
};
