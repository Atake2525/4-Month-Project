#include "CollisionModelManager.h"
#include "CollisionModel.h"

CollisionModelManager* CollisionModelManager::instance = nullptr;

CollisionModelManager* CollisionModelManager::GetInstance() {
	if (instance == nullptr) {
		instance = new CollisionModelManager;
	}
	return instance;
}

void CollisionModelManager::Finalize() {
	delete instance;
	instance = nullptr;
}

void CollisionModelManager::LoadModel(const std::string& directoryPath, const std::string& filePath) {
	// �ǂݍ��ݍσ��f��������
	if (models.contains(filePath)) {
		// �ǂݍ��ݍςȂ瑁��return
		return;
	}

	// ���f���̐����Ɠǂݍ��݁A������
	std::unique_ptr<CollisionModel> model = std::make_unique<CollisionModel>();
	model->Initialize(directoryPath, filePath);

	// ���f����map�R���e�i�Ɋi�[����
	models.insert(std::make_pair(filePath, std::move(model)));
}

CollisionModel* CollisionModelManager::FindModel(const std::string& filePath) {
	// �ǂݍ��ݍσ��f��������
	if (models.contains(filePath)) {
		// �ǂݍ��݃��f����߂�l�Ƃ���return
		return models.at(filePath).get();
	}

	// �t�@�C������v����
	return nullptr;
}