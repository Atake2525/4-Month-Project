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
	// 読み込み済モデルを検索
	if (models.contains(filePath)) {
		// 読み込み済なら早期return
		return;
	}

	// モデルの生成と読み込み、初期化
	std::unique_ptr<CollisionModel> model = std::make_unique<CollisionModel>();
	model->Initialize(directoryPath, filePath);

	// モデルをmapコンテナに格納する
	models.insert(std::make_pair(filePath, std::move(model)));
}

CollisionModel* CollisionModelManager::FindModel(const std::string& filePath) {
	// 読み込み済モデルを検索
	if (models.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return models.at(filePath).get();
	}

	// ファイル名一致無し
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