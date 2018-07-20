#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

Vector2 Entity::MOVE_DIRECTION_VALUE[4] = { Vector2(0, 1), Vector2(0, -1), Vector2(1, 0), Vector2(-1, 0) };

Entity::~Entity()
{
	delete m_spriteAnimSet;
}

Entity::Entity(const std::string name, EntityDefinition * def, Map* attachedMap)
	: m_name(name),
	m_entityDef(def),
	m_mapAttached(attachedMap)
{
	m_spriteAnimSet = new SpriteAnimSet(m_entityDef->m_spriteAnimSetDef);
	m_colorTint = m_entityDef->m_defaultTint;
}

void Entity::Destroy()
{
	m_toBeDestroyed = true;
	OnDestroy();
}

void Entity::Update(float deltaTime)
{
	m_ageInSeconds += deltaTime;
	m_spriteAnimSet->Update(deltaTime);
}

void Entity::LateUpdate(float deltaTime)
{
	UNUSED(deltaTime);
}

void Entity::Render() const
{
	g_theRenderer->PushMatrix();
	g_theRenderer->TranslateInMatrix(m_position);
	if (m_spriteAnimSet->ShouldAutoOrient())
		g_theRenderer->RotateInMatrix(m_orientation);
	AABB2 UVsToUse = m_spriteAnimSet->GetCurrentUVs();
	g_theRenderer->DrawTexturedAABB(m_entityDef->m_localDrawBounds, m_spriteAnimSet->GetCurrentTexture(), UVsToUse.mins, UVsToUse.maxs, m_colorTint);
	g_theRenderer->PopMatrix();
}

void Entity::RenderDebug() const
{
	Rgba physicsRadiusColor = Rgba::cyan;
	Rgba cosmeticRadiusColor = Rgba::magenta;
	Rgba velocityColor = Rgba(255, 255, 0);
	g_theRenderer->DrawDashedCircle(m_position, m_entityDef->m_physicsRadius, physicsRadiusColor);
	//g_theRenderer->DrawDashedCircle(m_position, m_cosmeticRadius, cosmeticRadiusColor);
	g_theRenderer->DrawLine(m_position, m_position + m_velocity, velocityColor);
}

void Entity::SetMoveDirectionAnimation()
{
	MOVE_DIRECTION moveDirection = GetMoveDirection(m_velocity);

	switch (moveDirection)
	{
		//TODO: ask prof, why StartAnim doesn't work????
		case MOVE_DIRECTION::NO_MOVEMENT:
			m_spriteAnimSet->SetCurrentAnim("Idle");
			break;
		case MOVE_DIRECTION::NORTH:
			m_spriteAnimSet->SetCurrentAnim("MoveNorth");
			break;
		case MOVE_DIRECTION::SOUTH:
			m_spriteAnimSet->SetCurrentAnim("MoveSouth");
			break;
		case MOVE_DIRECTION::EAST:
			m_spriteAnimSet->SetCurrentAnim("MoveEast");
			break;
		case MOVE_DIRECTION::WEST:
			m_spriteAnimSet->SetCurrentAnim("MoveWest");
			break;
	}
}

MOVE_DIRECTION Entity::GetMoveDirection(const Vector2& velocity)
{
	if (velocity == Vector2(0, 0))
	{
		return MOVE_DIRECTION::NO_MOVEMENT;
	}

	float dotNorth = DotProduct(velocity, Vector2(0, 1));
	float dotSouth = DotProduct(velocity, Vector2(0, -1));
	float dotWest = DotProduct(velocity, Vector2(-1, 0));
	float dotEast = DotProduct(velocity, Vector2(1, 0));

	float direction = max(max(dotNorth, dotSouth), max(dotWest, dotEast));
	if (direction == dotNorth)
		return MOVE_DIRECTION::NORTH;
	else if (direction == dotSouth)
		return MOVE_DIRECTION::SOUTH;
	else if (direction == dotWest)
		return MOVE_DIRECTION::WEST;
	else if (direction == dotEast)
		return MOVE_DIRECTION::EAST;

	ERROR_RECOVERABLE("Cannot find direction");
	return MOVE_DIRECTION::NO_MOVEMENT;
}

Vector2 Entity::GetMoveDirectionValue(const Vector2 & velocity)
{
	MOVE_DIRECTION direction = GetMoveDirection(velocity);

	if (direction == MOVE_DIRECTION::NO_MOVEMENT)
		return Vector2(0, 0);

	return MOVE_DIRECTION_VALUE[direction];
}
