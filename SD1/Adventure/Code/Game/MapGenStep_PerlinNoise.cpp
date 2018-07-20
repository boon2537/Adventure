#include "Game/MapGenStep_PerlinNoise.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/ThirdParty/SquirrelNoise/SmoothNoise.hpp"

MapGenStep_PerlinNoise::~MapGenStep_PerlinNoise()
{

}

MapGenStep_PerlinNoise::MapGenStep_PerlinNoise(const tinyxml2::XMLElement& genStepXmlElement)
	: MapGenStep(genStepXmlElement)
{
	m_toType = ParseXmlAttribute(genStepXmlElement, "toType", m_toType);
	m_ifType = ParseXmlAttribute(genStepXmlElement, "ifType", m_ifType);
	m_ifNoise = ParseXmlAttribute(genStepXmlElement, "ifNoise", FloatRange(0, 1));
	m_gridSize = ParseXmlAttribute(genStepXmlElement, "gridSize", IntRange(0, 0));
	m_octaves = ParseXmlAttribute(genStepXmlElement, "octaves", 1);
	m_persistence = ParseXmlAttribute(genStepXmlElement, "persistence", 0.5f);
	m_octaveScale = ParseXmlAttribute(genStepXmlElement, "octaveScale", 2.f);
}

void MapGenStep_PerlinNoise::Run(Map& map)
{
	//TODO: figure out gridSize....

	for (int y = 0; y < map.m_mapHeight; y++)
	{
		for (int x = 0; x < map.m_mapWidth; x++)
		{
			if (m_ifType == nullptr || m_ifType == map.GetTile(x, y).m_tileDef)
			{
				float perlinValue = Compute2dPerlinNoise((float) x / map.m_mapWidth, (float) y / map.m_mapHeight, 1.0f, m_octaves, m_persistence, m_octaveScale);
				perlinValue = RangeMapFloat(perlinValue, -1.f, 1.f, 0, 1.f);
				if (perlinValue >= m_ifNoise.min && perlinValue <= m_ifNoise.max)
				{
					map.GetTile(x, y).m_tileDef = m_toType;
				}
			}
		}
	}
}
