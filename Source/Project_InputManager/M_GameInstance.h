#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "M_GameInstance.generated.h"

class UM_InputManager;

UCLASS()
class PROJECT_INPUTMANAGER_API UM_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UM_InputManager> InputManagerType;

	UPROPERTY()
	UM_InputManager* InputManager;

public:
	const UM_InputManager* GetInputManager() const { return InputManager; }
	
private:
	virtual void Init() override;
	virtual void Shutdown() override;

	void InitializeGameInstance();
	
	void InitializeInputManager();
	void TerminateInputManager();
};
