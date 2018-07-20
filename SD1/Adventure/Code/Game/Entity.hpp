#pragma once

#include "Game/EntityDefinition.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteAnimSet.hpp"
#include "Engine/Core/EngineCommon.hpp"

class Map;
class Item;

enum MOVE_DIRECTION
{
	NO_MOVEMENT = -1,
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class Entity
{
public:
	virtual ~Entity();
	Entity(const std::string name, EntityDefinition* def, Map* attachedMap);
	void Destroy();
	virtual void OnDestroy() {};
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void Render() const;
	virtual void RenderDebug() const;
	virtual void RenderWorldUI() const {};
	virtual bool ShouldBeDestroyed() const { return m_toBeDestroyed; };
	virtual void DoCollision(Entity* other) { UNUSED(other) };

	void SetMoveDirectionAnimation();
	MOVE_DIRECTION GetMoveDirection(const Vector2& velocity);
	Vector2 GetMoveDirectionValue(const Vector2& velocity);

public:
	static Vector2 MOVE_DIRECTION_VALUE[4];

	std::string m_name;
	Vector2 m_position;
	Vector2 m_velocity;
	float m_orientation;
	float m_ageInSeconds;
	EntityDefinition* m_entityDef = nullptr;
	SpriteAnimSet* m_spriteAnimSet = nullptr;
	Rgba m_colorTint = Rgba::white;
	Map* m_mapAttached = nullptr;
	std::vector<Item*> m_inventory;

protected:
	bool m_toBeDestroyed = false;
};