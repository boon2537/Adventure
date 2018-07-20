#include "Engine/Input/InputSystem.hpp"

void RunMessagePump() // NOTE: standalone function in InputSystem.cpp (not an InputSystem method)
{
	MSG queuedMessage;
	for( ;; )
	{
		const bool wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}

InputSystem::InputSystem()
{
	for (int controllerID = 0; controllerID < InputSystem::NUM_CONTROLLERS; ++ controllerID)
	{
		m_controllers[controllerID] = XboxController((unsigned int) controllerID, 0.16f, 0.95f);
	}
}

InputSystem::~InputSystem()
{
}

void InputSystem::BeginFrame()
{
	UpdateControllers();
	UpdateKeyboard();
	RunMessagePump(); // Ask Windows to call our registered WinProc function with WM_KEYDOWN notifications, etc.
}

void InputSystem::EndFrame()
{

}

void InputSystem::OnKeyPressed(unsigned char keyCode)
{
	if (!m_keyStates[static_cast<int>(keyCode)].isDown)
		m_keyStates[static_cast<int>(keyCode)].justPressed = true;

	m_keyStates[static_cast<int>(keyCode)].isDown = true;
}

void InputSystem::OnKeyReleased(unsigned char keyCode)
{
	m_keyStates[static_cast<int>(keyCode)].isDown = false;
	m_keyStates[static_cast<int>(keyCode)].justReleased = true;
}

bool InputSystem::IsKeyPressed(KEY_CODE keyCode) const
{
	return m_keyStates[static_cast<int>(keyCode)].isDown;
}

bool InputSystem::WasKeyJustPressed(KEY_CODE keyCode) const
{
	return m_keyStates[static_cast<int>(keyCode)].justPressed;
}

bool InputSystem::WasKeyJustReleased(KEY_CODE keyCode) const
{
	return m_keyStates[static_cast<int>(keyCode)].justReleased;
}

//-----------------------------------------------------------------------------------------------
void InputSystem::UpdateKeyboard()
{
	// Clear all just-changed flags, in preparation for the next round of WM_KEYDOWN, etc. messages
	for( int keyCode = 0; keyCode < InputSystem::NUM_KEYS; ++ keyCode )
	{
		m_keyStates[keyCode].justPressed = false;
		m_keyStates[keyCode].justReleased = false;
	}
}

void InputSystem::UpdateControllers()
{
	for (int controllerID = 0; controllerID < InputSystem::NUM_CONTROLLERS; ++ controllerID)
	{
		m_controllers[controllerID].UpdateController();
	}
}
