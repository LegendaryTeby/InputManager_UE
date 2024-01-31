#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GameInput.h"

#include "DT_InputDataTable.generated.h"

UCLASS()
class PROJECT_INPUTMANAGER_API UDT_InputDataTable : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Inputs AZERTY", meta = (ShowOnlyInnerProperties))
	FGameInput GameInputAZERTY = FGameInput(false);
	UPROPERTY(EditAnywhere, Category = "Inputs QWERTY", meta = (ShowOnlyInnerProperties))
	FGameInput GameInputQWERTY = FGameInput(true);
public:
	FORCEINLINE const FGameInput& GetGameInput(const bool _qwerty) { return _qwerty ? GameInputQWERTY : GameInputAZERTY; }
};
