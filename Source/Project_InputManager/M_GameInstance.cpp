#include "M_GameInstance.h"

#include "M_InputManager.h"

void UM_GameInstance::Init()
{
	InitializeGameInstance();

	Super::Init();
}

void UM_GameInstance::Shutdown()
{
	TerminateInputManager();
	
	Super::Shutdown();
}

void UM_GameInstance::InitializeGameInstance()
{
	InitializeInputManager();
	//...
}

void UM_GameInstance::InitializeInputManager()
{
	if (InputManagerType.Get())
	{
		InputManager = NewObject<UM_InputManager>(this, InputManagerType);
		InputManager->InitializeGameInput();
	}
}
void UM_GameInstance::TerminateInputManager()
{
	if (!InputManager) return;

	InputManager->TerminateGameInput();
}
