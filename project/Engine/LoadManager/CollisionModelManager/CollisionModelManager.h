#include <map>
#include <string>
#include <memory>
#include "AABB.h"

#pragma once

class CollisionModel;

class CollisionModelManager {
private:
	// �V���O���g���p�^�[����K�p
	static CollisionModelManager* instance;

	// �R���X�g���N�^�A�f�X�g���N�^�̉B��
	CollisionModelManager() = default;
	~CollisionModelManager() = default;
	// �R�s�[�R���X�g���N�^�A�R�s�[������Z�q�̕���
	CollisionModelManager(CollisionModelManager&) = delete;
	CollisionModelManager& operator=(CollisionModelManager&) = delete;

public:

	// �V���O���g���C���X�^���X�̎擾
	static CollisionModelManager* GetInstance();
	// �I��
	void Finalize();

	/// <summary>
	/// ���f���t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="directoryPath"> : �f�B���N�g��(���t�@�C��)�̃p�X</param>
	/// <param name="filePath"> : ���f���t�@�C���̃p�X</param>
	/// <param name="enableLighting"> : ���C�e�B���O��K�p���邩�ǂ���</param>
	/// enableLighting�͉������͂��Ȃ����false
	void LoadModel(const std::string& directoryPath, const std::string& filePath);

	CollisionModel* FindModel(const std::string& filePath);

	// �X�V
	void Update(AABB& collisionTraget);

private:
	// ���f���[�^
	std::map<std::string, std::unique_ptr<CollisionModel>> models;

	void CreateAABB();

	AABB target;

	void CheckCollisionLeft();
	void CheckCollisionRight();
	void CheckCollisionTop();
	void CheckCollisionBottom();
	void CheckCollisionBack();
	void CheckCollisionFront();

};
