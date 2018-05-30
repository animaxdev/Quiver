#pragma once

#include <Quiver/Entity/CustomComponent/CustomComponent.h>

#include "CameraHelpers.h"
#include "Damage.h"
#include "Effects.h"
#include "FirePropagation.h"
#include "Utils.h"
#include "Weapon.h"

namespace sf {
class RenderTarget;
}

namespace qvr {
class Entity;
}

class PlayerEditor;
class Weapon;

struct PlayerDesc {
	ActiveEffectSet activeEffects;
	DamageCount damage = DamageCount(50);
	float moveSpeed = 1.0f;
};

class Player : public qvr::CustomComponent {
public:
	Player(qvr::Entity& entity);
	Player(qvr::Entity& entity, CameraOwner&& camera, const PlayerDesc& desc);

	void HandleInput(
		qvr::RawInputDevices& inputDevices, 
		const std::chrono::duration<float> deltaTime) override;

	void OnStep(const std::chrono::duration<float> deltaTime) override;

	void OnBeginContact(
		qvr::Entity& other, 
		b2Fixture& myFixture,
		b2Fixture& otherFixture) override;
	void OnEndContact(
		qvr::Entity& other, 
		b2Fixture& myFixture,
		b2Fixture& otherFixture) override;

	std::string GetTypeName() const override { return "Player"; }

	std::unique_ptr<qvr::CustomComponentEditor> CreateEditor() override;

	nlohmann::json ToJson() const override;
	bool FromJson(const nlohmann::json& j) override;

	PlayerDesc GetDesc();

	qvr::Camera3D       & GetCamera()       { return cameraOwner.camera; }
	qvr::Camera3D const & GetCamera() const { return cameraOwner.camera; }

	CameraOwner cameraOwner;

	friend class PlayerEditor;

private:
	void RenderCurrentWeapon(sf::RenderTarget& target) const;
	void RenderHud          (sf::RenderTarget& target) const;
	void RenderActiveEffects(sf::RenderTarget& target) const;

	ActiveEffectSet m_ActiveEffects;
	
	FiresInContact m_FiresInContact;

	float mMoveSpeed;
	
	DamageCount mDamage;
	
	std::unique_ptr<Weapon> mCurrentWeapon;

	bool mCannotDie = false;

	TimeLerper<float> fovLerper;
};