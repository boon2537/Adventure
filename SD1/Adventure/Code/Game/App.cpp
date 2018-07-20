#include "Game/App.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/ThirdParty/tinyxml2.h"

App::App()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Data/GameConfig.xml");
	g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*doc.RootElement());

	g_theRenderer = new Renderer();
	g_theInput = new InputSystem();
	g_audio = new AudioSystem();
	g_theGame = new Game();
	m_quitting = false;	
}

App::~App()
{
	delete g_theGame;
	delete g_audio;
	delete g_theInput;
	delete g_theRenderer;
}

bool App::IsQuitting()
{
	return m_quitting;
}

void App::Quit()
{
	m_quitting = true;
}

void App::Update()
{
	if (g_theInput->WasKeyJustPressed(KEY_CODE::ESC) || g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::BACK))
	{
		if (g_theGame->GetCurrentState() == GAME_STATE::ATTRACT)
			Quit();
	}
	g_theGame->Update();
}

void App::Render()
{
	g_theGame->Render();
}

void App::BeginFrame()
{
	g_theInput->BeginFrame();
	g_theRenderer->BeginFrame();
	g_audio->BeginFrame();
}

void App::EndFrame()
{
	g_audio->EndFrame();
	g_theRenderer->EndFrame();
	g_theInput->EndFrame();
}

void App::OnQuitRequested()
{
	Quit();
}
