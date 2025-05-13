#include "UI.h"
#include "SpriteBase.h"
#include "TextureManager.h"
#include "WinApp.h"

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

UI::~UI() {
	delete sprite;
}

void UI::SetSprite(const std::string& filename) {
	TextureManager::GetInstance()->LoadTexture(filename);

	sprite->SetTexture(filename);
}

void UI::CreateButton(const Vector2& spritePosition, const Origin& origin, const std::string& filename) {
	input = Input::GetInstance();

	TextureManager::GetInstance()->LoadTexture(filename);

	sprite = new Sprite();
	sprite->Initialize(filename);
	sprite->SetPosition(spritePosition);

	switch (origin)
	{
	case Origin::Center:
		sprite->SetTextureLeftTop({ 0.5f, 0.5f });
		break;
	case Origin::Top:
		sprite->SetTextureLeftTop({ 0.5f, 0.0f });
		break;
	case Origin::Bottom:
		sprite->SetTextureLeftTop({ 0.5f, 1.0f });
		break;
	case Origin::LeftTop:
		sprite->SetTextureLeftTop({ 0.0f, 0.0f });
		break;
	case Origin::RightTop:
		sprite->SetTextureLeftTop({ 1.0f, 0.0f });
		break;
	case Origin::LeftBottom:
		sprite->SetTextureLeftTop({ 0.0f, 1.0f });
		break;
	case Origin::RightBottom:
		sprite->SetTextureLeftTop({ 1.0f, 1.0f });
		break;
	case Origin::LeftCenter:
		sprite->SetTextureLeftTop({ 0.0f, 0.5f });
		break;
	case Origin::RightCenter:
		sprite->SetTextureLeftTop({ 1.0f, 0.5f });
		break;
	}
}

const bool& UI::OnButton() const {
	sprite->Update();
	// スプライトのサイズ
	Vector2 spriteSize = sprite->GetScale();
	// スプライトの位置
	Vector2 spritePos = sprite->GetPosition();
	// マウスの位置
	Vector3 mousePos = input->GetWindowMousePos3();
	// スプライトのAABBを計算する
	AABB spriteAABB = {
		{spritePos.x, spritePos.y, 0.0f},
		{spritePos.x + spriteSize.x, spritePos.y + spriteSize.y, 0.0f},
	};
	// ウィンドウ内のMousePositionをAABBで計算する
	AABB mousePosAABB = {
		{mousePos.x, mousePos.y},
		{mousePos.x, mousePos.y},
	};

	// スプライトのAABB内にマウスポインタがある状態で左クリックをする
	if (CollisionAABB(spriteAABB, mousePosAABB) == true && GetAsyncKeyState(VK_LBUTTON) == -32768)
	{
		return true;
	}
	ImGui::Begin("Button");
	ImGui::DragFloat2("mousePos", &mousePosAABB.min.x, 0.1f);
	ImGui::End();

	return false;
}

void UI::Draw() {
	sprite->Draw();
}

const bool& UI::CollisionAABB(const AABB& a, const AABB& b) const {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	return false;
}