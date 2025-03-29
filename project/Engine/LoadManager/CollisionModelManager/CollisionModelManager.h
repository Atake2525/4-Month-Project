#include <map>
#include <string>
#include <memory>
#include "AABB.h"

#pragma once

class CollisionModel;

class CollisionModelManager {
private:
	// シングルトンパターンを適用
	static CollisionModelManager* instance;

	// コンストラクタ、デストラクタの隠蔽
	CollisionModelManager() = default;
	~CollisionModelManager() = default;
	// コピーコンストラクタ、コピー代入演算子の封印
	CollisionModelManager(CollisionModelManager&) = delete;
	CollisionModelManager& operator=(CollisionModelManager&) = delete;

public:

	// シングルトンインスタンスの取得
	static CollisionModelManager* GetInstance();
	// 終了
	void Finalize();

	/// <summary>
	/// モデルファイルの読み込み
	/// </summary>
	/// <param name="directoryPath"> : ディレクトリ(元ファイル)のパス</param>
	/// <param name="filePath"> : モデルファイルのパス</param>
	/// <param name="enableLighting"> : ライティングを適用するかどうか</param>
	/// enableLightingは何も入力しなければfalse
	void LoadModel(const std::string& directoryPath, const std::string& filePath);

	CollisionModel* FindModel(const std::string& filePath);

	// 更新
	void Update(AABB& collisionTraget);

private:
	// モデレータ
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
