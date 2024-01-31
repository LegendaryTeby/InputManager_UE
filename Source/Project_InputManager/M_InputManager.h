#pragma once

#include "CoreMinimal.h"

#include "GameInput.h"

#include "M_InputManager.generated.h"

class UDT_InputDataTable;

UCLASS(Blueprintable)
class PROJECT_INPUTMANAGER_API UM_InputManager : public UObject
{
	GENERATED_BODY()

	//Event when Input Action are completely reset (Reset asked or switch Keyboard AZERTY/QWERTY mode)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputActionReset, const TArray<FInputActionData>&, _actionInputs);
	//Event when Input Axis are completely reset (Reset asked or switch Keyboard AZERTY/QWERTY mode)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputAxisReset, const TArray<FInputAxisData>&, _axisInputs);
	//Event when Input Action or Axis are completely resets
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputReset, UM_InputManager*, _inputManager);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInputActionRebind, const FInputActionData&, _input, FInputActionKeyMapping, _oldMapping, FInputActionKeyMapping, _newMapping);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInputAxisRebind, const FInputAxisData&, _input, FInputAxisKeyMapping, _oldMapping, FInputAxisKeyMapping, _newMapping);

	UPROPERTY(EditAnywhere, Category = "Inputs Settings")
	UDT_InputDataTable* DefaultGameInputDataTable = nullptr;
	UPROPERTY(EditAnywhere, Category = "Inputs Settings")
	bool DefaultInputIsQwerty = false;

	#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Inputs Editor Settings")
	bool UseSave = false;
	#endif	

	UPROPERTY()
	FGameInput GameInput = FGameInput();
	UPROPERTY()
	class UInputSettings* InputSettings = nullptr;
	UPROPERTY()
	class US_InputSave* InputSave = nullptr;
	UPROPERTY()
	FString InputSaveSlot = "SaveInput";

	UPROPERTY()
	FOnInputActionReset onRebindableInputActionReset;
	UPROPERTY()
	FOnInputActionReset onInputActionReset;

	UPROPERTY()
	FOnInputAxisReset onRebindableInputAxisReset;
	UPROPERTY()
	FOnInputAxisReset onInputAxisReset;

	UPROPERTY()
	FOnInputReset onInputReset;

	UPROPERTY()
	FOnInputActionRebind onInputActionRebind;
	UPROPERTY()
	FOnInputAxisRebind onInputAxisRebind;

public:
	FORCEINLINE FOnInputActionReset& OnRebindableInputActionReset() { return onRebindableInputActionReset; }
	FORCEINLINE FOnInputActionReset& OnInputActionReset() { return onInputActionReset; }

	FORCEINLINE FOnInputAxisReset& OnRebindableInputAxisReset() { return onRebindableInputAxisReset; }
	FORCEINLINE FOnInputAxisReset& OnInputAxisReset() { return onInputAxisReset; }

	FORCEINLINE FOnInputReset& OnInputReset() { return onInputReset; }

	FORCEINLINE FOnInputActionRebind& OnInputActionRebind() { return onInputActionRebind; }
	FORCEINLINE FOnInputAxisRebind& OnInputAxisRebind() { return onInputAxisRebind; }

	FORCEINLINE const FGameInput& GetGameInput() const { return GameInput; }
	//Called in Blueprint
	UFUNCTION(BlueprintCallable) FORCEINLINE FGameInput GetGameInputCopy() const { return GameInput; }

	TArray<FInputActionData> GetRebindableInputAction() const;
	TArray<FInputAxisData> GetRebindableInputAxis() const;

	TArray<FKey> GetUsedInputKeys() const;
	TArray<FInputChord> GetUsedActionInputChords() const;

public:
	UM_InputManager();

	//Reset Input Action & Axis from Default Game Input Data Table
	void ResetInputToDefault();

	//Reset Input Action & Axis to AZERTY or QWERTY
	void SwitchKeyboardInputMode(const bool _qwerty);

	//Force a Rebind careless of it's rebindable or key already used 
	bool ForceRebindAction(const FName& _actionName, const FKey& _oldKey, const FKey& _newKey);
	//Force a Rebind careless of it's rebindable or key already used 
	bool ForceRebindAction(const FName& _actionName, const FInputChord& _oldKey, const FInputChord& _newKey);
	//Rebind an Action by the given ActionName & Key with the New Key
	bool RebindAction(const FName& _actionName, const FKey& _oldKey, const FKey& _newKey);
	//Rebind an Action by the given ActionName & InputChord with the InputChord
	bool RebindAction(const FName& _actionName, const FInputChord& _oldKey, const FInputChord& _newKey);

	//Rebind an Axis by the given ActionName & Key with the New Key
	bool RebindAxis(const FName& _axisName, const FKey& _oldKey, const FKey& _newKey);

	//Create Input Mappings in Input Settings
	void InitializeGameInput();
	//Remove Input Mappings from Input Settings
	void TerminateGameInput();

private:
	//Load the Default Game Input from Default Game Input Data Table
	void LoadDefaultGameInput();
	//Load the Game Input from Save (if exist. If not Default will be loaded)
	void LoadSavedGameInput();

	//Create Input Action Mappings in Input Settings
	void InitializeAction();
	//Create Input Axis Mappings in Input Settings
	void InitializeAxis();

	//Remove Input Action Mappings from Input Settings
	void TerminateAction();
	//Remove Input Axis Mappings from Input Settings
	void TerminateAxis();

	//Load Game Input Data from Save or Default Input Data Table
	void LoadInputData();
	//Save the current Game Input Data in Save (if save exist)
	void SaveInputData() const;

	//Reset Remove current Input Action and remplace it with given Input Action
	void ResetAction(const TArray<FInputActionData>& _defaultActionData);
	//Reset Remove current Input Axis and remplace it with given Input Axis
	void ResetAxis(const TArray<FInputAxisData>& _defaultAxisData);
};