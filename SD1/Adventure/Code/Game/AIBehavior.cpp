#include "Game/AIBehavior.hpp"
#include "Game/AIBehavior_Wander.hpp"
#include "Game/AIBehavior_Shoot.hpp"
#include "Game/AIBehavior_Follow.hpp"
#include "Game/AIBehavior_Melee.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

AIBehavior::AIBehavior(const tinyxml2::XMLElement & AIBehaviorXmlElement)
{
	m_name = AIBehaviorXmlElement.Name();
	m_basePriority = ParseXmlAttribute(AIBehaviorXmlElement, "basePriority", 0.f);
}

AIBehavior* AIBehavior::CreateAIBehavior(const tinyxml2::XMLElement & AIBehaviorXmlElement)
{
	std::string behaviorName = AIBehaviorXmlElement.Name();
	if (behaviorName == "Wander")	return new AIBehavior_Wander(AIBehaviorXmlElement);
	if (behaviorName == "Shoot")	return new AIBehavior_Shoot(AIBehaviorXmlElement);
	if (behaviorName == "Melee")	return new AIBehavior_Melee(AIBehaviorXmlElement);
	if (behaviorName == "Follow")	return new AIBehavior_Follow(AIBehaviorXmlElement);

	ERROR_RECOVERABLE("Cannot Find StepName: " + behaviorName);
	return nullptr;
}
