#pragma once
#include <AABB.h> //��Œǉ�
#include "Vector3.h" 
#include "Model.h" 
#include <Object3d.h>

class Player;//��Œǉ�

class Goal
{
public:
	// ������
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	// �X�V
	void Update();
	//�`��
	void Draw();

	//�����蔻��
	void OnCollision(const Player* player);
	//�����蔻��
	AABB GetAABB();

	//�G�t�F�N�g
	void GoalEffect();

private:
	// ���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	Vector3 position_;  // �S�[���̈ʒu
	Model* model_ = nullptr;      // obj���f���f�[�^


};

