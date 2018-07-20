#include "Game/MapGenStep.hpp"
#include "Game/MapGenStep_FillAndEdge.hpp"
#include "Game/MapGenStep_Mutate.hpp"
#include "Game/MapGenStep_FromFile.hpp"
#include "Game/MapGenStep_SpawnActor.hpp"
#include "Game/MapGenStep_SpawnItem.hpp"
#include "Game/MapGenStep_CellularAutomata.hpp"
#include "Game/MapGenStep_PerlinNoise.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

MapGenStep::~MapGenStep()
{
}

MapGenStep::MapGenStep(const tinyxml2::XMLElement & genStepXmlElement)
{
	if (genStepXmlElement.FindAttribute("iterations") != 0)
		m_iterations = ParseXmlAttribute(genStepXmlElement, "iterations", m_iterations);
	if (genStepXmlElement.FindAttribute("chanceToRun") != 0)
		m_chanceToRun = ParseXmlAttribute(genStepXmlElement, "chanceToRun", m_chanceToRun);
}

void MapGenStep::Generate(Map & map)
{
	if (CheckRandomChance(m_chanceToRun))
	{
		int runCount = GetRandomIntInRange(m_iterations.min, m_iterations.max);
		for (int i = 0; i < runCount; i++)
			Run(map);
	}
}

MapGenStep* MapGenStep::CreateMapGenStep(const tinyxml2::XMLElement & genStepXmlElement)
{
	std::string stepName = genStepXmlElement.Name();
	if (stepName == "FillAndEdge")	return new MapGenStep_FillAndEdge(genStepXmlElement);
	if (stepName == "Mutate")		return new MapGenStep_Mutate(genStepXmlElement);
	if (stepName == "FromFile")		return new MapGenStep_FromFile(genStepXmlElement);
	if (stepName == "SpawnActor")	return new MapGenStep_SpawnActor(genStepXmlElement);
	if (stepName == "SpawnItem")	return new MapGenStep_SpawnItem(genStepXmlElement);
	if (stepName == "CellularAutomata") return new MapGenStep_CellularAutomata(genStepXmlElement);
	if (stepName == "PerlinNoise")	return new MapGenStep_PerlinNoise(genStepXmlElement);

	ERROR_RECOVERABLE("Cannot Find StepName: " + stepName);
	return nullptr;
}
