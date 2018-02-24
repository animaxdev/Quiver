#pragma once

#include <functional>

#include <Box2D/Common/b2Math.h>
#include <optional.hpp>

#include <Quiver/Physics/PhysicsUtils.h>

class b2Fixture;
class b2World;
struct b2AABB;

namespace qvr {
class Entity;
class CustomComponent;
}

template <typename T>
using optional = std::experimental::optional<T>;

namespace FixtureFilterCategories
{
const uint16 Default = 1 << 0;
const uint16 Player = 1 << 1;
const uint16 Sensor = 1 << 2;
const uint16 Projectile = 1 << 3;
const uint16 CrossbowBolt = 1 << 4;
const uint16 Enemy = 1 << 5;
// RenderOnly is set by the engine. We shouldn't touch it.
const uint16 RenderOnly = 1 << 15;
}

inline qvr::FixtureFilterBitNames CreateFilterBitNames()
{
	qvr::FixtureFilterBitNames bitNames{};
	bitNames[0] = "Default";
	bitNames[1] = "Player";
	bitNames[2] = "Sensor";
	bitNames[3] = "Projectile";
	bitNames[4] = "CrossbowBolt";
	bitNames[5] = "Enemy";
	return bitNames;
}

qvr::CustomComponent* GetCustomComponent(const b2Fixture* fixture);

qvr::Entity*          GetPlayerFromFixture(const b2Fixture* fixture);

std::experimental::optional<b2Vec2> QueryAABBToFindPlayer(
	const b2World& world,
	const b2AABB& aabb);

std::experimental::optional<b2Vec2> RayCastToFindPlayer(
	const b2World& world,
	const b2Vec2& rayStart,
	const b2Vec2& rayEnd);

std::experimental::optional<b2Vec2> RayCastToFindPlayer(
	const b2World& world,
	const b2Vec2& rayPos,
	const float angle,
	const float range);

inline float Lerp(const float a, const float b, const float t) {
	return a + t * (b - a);
}

inline float NoEase(const float t) { return t; }

template <class T>
class TimeLerper {
	float t = 0.0f;
	float secondsToReachTarget = 0.0f;
	T startVal;
	T targetVal;
public:
	TimeLerper() = default;

	TimeLerper(const T& start, const T& target, const float seconds)
	{
		SetTarget(start, target, seconds);
	}

	void SetTarget(const T& start, const T& target, const float seconds) {
		secondsToReachTarget = std::max(seconds, 0.001f);
		targetVal = target;
		startVal = start;
		t = 0.0f;
	}

	T Update(
		const float seconds, 
		std::function<float(const float)> EasingFunc = NoEase)
	{
		if (t < 1.0f) {
			t += seconds / secondsToReachTarget;
			return startVal + EasingFunc(t) * (targetVal - startVal);
		}
		return targetVal;
	}
};