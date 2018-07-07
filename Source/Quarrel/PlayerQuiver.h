#pragma once

#include <array>
#include <chrono>

#include <optional.hpp>

#include <SFML/Graphics/Color.hpp>

#include <Quiver/Graphics/ColourUtils.h>

#include "CrossbowBolt.h"

struct QuarrelTypeInfo
{
	sf::Color colour;
	CrossbowBoltEffect effect;
};

void to_json(nlohmann::json& j, const QuarrelTypeInfo& quarrelType);
void from_json(const nlohmann::json& j, QuarrelTypeInfo& quarrelType);

class QuarrelSlot
{
	std::chrono::duration<float> cooldownRemaining;
	std::chrono::duration<float> cooldownTime;
	
public:
	auto GetCooldownRatio() const -> float {
		return cooldownRemaining / cooldownTime;
	}

	auto TakeQuarrel(const std::chrono::duration<float> cooldown)
		-> std::experimental::optional<QuarrelTypeInfo>;

	void OnStep(const std::chrono::duration<float> deltaTime) {
		cooldownRemaining = std::max(cooldownRemaining - deltaTime, std::chrono::duration<float>(0));
	}

	QuarrelTypeInfo type;

	QuarrelSlot(const QuarrelTypeInfo& type) : type(type) {}
};

inline bool CanTakeQuarrel(const QuarrelSlot& slot) {
	return slot.GetCooldownRatio() <= 0.0f;
}

class PlayerQuiver {
public:
	static const int MaxEquippedQuarrelTypes = 3;

	using OptionalQuarrelSlot = std::experimental::optional<QuarrelSlot>;

	std::array<OptionalQuarrelSlot, MaxEquippedQuarrelTypes> quarrelSlots;

	void OnStep(const std::chrono::duration<float> deltaTime) {
		for (auto& quarrelSlot : quarrelSlots) {
			if (quarrelSlot) {
				quarrelSlot->OnStep(deltaTime);
			}
		}
	}
};

using OptionalQuarrelType = std::experimental::optional<QuarrelTypeInfo>;

auto TakeQuarrel(PlayerQuiver& quiver, const int slotIndex) -> OptionalQuarrelType;

void to_json  (nlohmann::json& j,       PlayerQuiver const& quiver); 
void from_json(nlohmann::json const& j, PlayerQuiver & quiver);