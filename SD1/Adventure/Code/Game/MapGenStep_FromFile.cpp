#include "Game/MapGenStep_FromFile.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/XmlUtilities.hpp"

MapGenStep_FromFile::~MapGenStep_FromFile()
{
	delete m_imageData;
}

MapGenStep_FromFile::MapGenStep_FromFile(const tinyxml2::XMLElement& genStepXmlElement)
	: MapGenStep(genStepXmlElement)
{
	std::string path = "Data/Images/" + ParseXmlAttribute(genStepXmlElement, "image", "empty");
	m_imageData = new Image(path);
}

void MapGenStep_FromFile::Run(Map& map)
{
	//find top-right position to start
	IntVector2 offset = IntVector2(GetRandomIntInRange(0, map.m_mapWidth - m_imageData->GetDimensions().x), GetRandomIntInRange(0, map.m_mapHeight - m_imageData->GetDimensions().y));
	IntVector2 start = IntVector2(offset.x, map.m_mapHeight - 1 - offset.y);

	for (int y = 0; y < m_imageData->GetDimensions().y; y++)
	{
		for (int x = 0; x < m_imageData->GetDimensions().x; x++)
		{
			if (map.GetTile(start.x + x, start.y - y).m_tileDef != nullptr)
			{
				//Alpha texel values represent the probability of that tile getting written, 
				//so an opaque (alpha=255) texel always writes a Tile, and a transparent (alpha=0) texel never does
				if (CheckRandomChance(m_imageData->GetTexel(x, y).a / 255.f))
					map.GetTile(start.x + x, start.y - y).m_tileDef = TileDefinition::GetDefinitionFromTexel(m_imageData->GetTexel(x, y));
			}
		}
	}
}
