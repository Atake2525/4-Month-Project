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

void CollisionModelManager::Update(AABB& collisionTarget) {
	target = collisionTarget;

}

void CollisionModelManager::CreateAABB() {
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