#include "M_InputManager.h"

#include "DT_InputDataTable.h"
#include "S_InputSave.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/InputSettings.h"

UM_InputManager::UM_InputManager()
{

}

#pragma region Rebindable Input
TArray<FInputActionData> UM_InputManager::GetRebindableInputAction() const
{
	TArray<FInputActionData> _rebindableInputAction = { };

	for (const FInputActionData& _inputAction : GameInput.GetInputActionData())
		if (_inputAction.Rebindable)
			_rebindableInputAction.Add(_inputAction);

	return _rebindableInputAction;
}
TArray<FInputAxisData> UM_InputManager::GetRebindableInputAxis() const
{
	TArray<FInputAxisData> _rebindableInputAxis = { };

	for (const FInputAxisData& _inputAction : GameInput.GetInputAxisData())
		if (_inputAction.Rebindable)
			_rebindableInputAxis.Add(_inputAction);

	return _rebindableInputAxis;
}

TArray<FKey> UM_InputManager::GetUsedInputKeys() const
{
	TArray<FKey> _keys;

	for (const FInputActionData& _inputAction : GameInput.GetInputActionData())
	{
		if (_inputAction.Rebindable)
		{
			const TArray<FInputActionKeyMapping>& _mappings = _inputAction.Mappings;
			for (const FInputActionKeyMapping& _mapping : _mappings)
				if (_mapping.Key.IsValid())
					_keys.Add(_mapping.Key);
		}
	}

	for (const FInputAxisData& _inputAxis : GameInput.GetInputAxisData())
	{
		if (_inputAxis.Rebindable)
		{
			const TArray<FInputAxisKeyMapping>& _mappings = _inputAxis.Mappings;
			for (const FInputAxisKeyMapping& _mapping : _mappings)
				if (_mapping.Key.IsValid())
					_keys.Add(_mapping.Key);
		}
	}

	return _keys;
}

TArray<FInputChord> UM_InputManager::GetUsedActionInputChords() const
{
	TArray<FInputChord> _inputChords;

	for (const FInputActionData& _inputAction : GameInput.GetInputActionData())
	{
		if (_inputAction.Rebindable)
		{
			const TArray<FInputActionKeyMapping>& _mappings = _inputAction.Mappings;
			for (const FInputActionKeyMapping& _mapping : _mappings)
				if (_mapping.Key.IsValid())
					_inputChords.Add(FInputChord(_mapping.Key, _mapping.bShift, _mapping.bCtrl, _mapping.bAlt, _mapping.bCmd));
		}
	}

	return _inputChords;
}
#pragma endregion

#pragma region Rebind
bool UM_InputManager::ForceRebindAction(const FName& _actionName, const FKey& _oldKey, const FKey& _newKey)
{
	if (_oldKey == _newKey) return false;
	if (!InputSettings) return false;

	for (TTuple<FName, FInputActionData*> _inputAction : GameInput.InputActionData)
	{
		if (_inputAction.Key == _actionName)
		{
			FInputActionKeyMapping _oldMapping;
			if (!_inputAction.Value->GetMapping(_oldMapping, _oldKey)) return false;				//If Mapping does not exist abord Rebind

			if (!_inputAction.Value->UpdateKeyMapping(_oldKey, _newKey)) return false;				//If old Input Key does not exist in InputKey or Mappings abort rebind

			FInputActionKeyMapping _newMapping;
			if (!_inputAction.Value->GetMapping(_newMapping, _newKey)) return false;				//If previous Update Key Mapping failed abort rebind (Should not happen in this context, bugs can appear if happen)

			InputSettings->RemoveActionMapping(_oldMapping, false);				//Remove Mapping with the old Key (do not ForceRebuildKeymaps to avoid Action Binding removed from Input Settings)
			InputSettings->AddActionMapping(_newMapping, false);					//Add the Mapping with the new Key
			InputSettings->ForceRebuildKeymaps();													//Rebuild the Input Settings Keymaps

			onInputActionRebind.Broadcast(*_inputAction.Value, _oldMapping, _newMapping);
			return true;
		}
	}
	return false;
}
bool UM_InputManager::ForceRebindAction(const FName& _actionName, const FInputChord& _oldKey, const FInputChord& _newKey)
{
	if (_oldKey == _newKey) return false;
	if (!InputSettings) return false;

	for (TTuple<FName, FInputActionData*> _inputAction : GameInput.InputActionData)
	{
		if (_inputAction.Key == _actionName)
		{
			FInputActionKeyMapping _oldMapping;
			if (!_inputAction.Value->GetMapping(_oldMapping, _oldKey)) return false;				//If Mapping does not exist abord Rebind

			if (!_inputAction.Value->UpdateInputChordMapping(_oldKey, _newKey)) return false;		//If old Input Chord does not exist in Mappings abort rebind

			FInputActionKeyMapping _newMapping;
			if (!_inputAction.Value->GetMapping(_newMapping, _newKey)) return false;				//If previous Update Key Mapping failed abort rebind (Should not happen in this context, bugs can appear if happen)

			InputSettings->RemoveActionMapping(_oldMapping, false);				//Remove Mapping with the old Key (do not ForceRebuildKeymaps to avoid Action Binding removed from Input Settings)
			InputSettings->AddActionMapping(_newMapping, false);					//Add the Mapping with the new Key
			InputSettings->ForceRebuildKeymaps();													//Rebuild the Input Settings Keymaps

			onInputActionRebind.Broadcast(*_inputAction.Value, _oldMapping, _newMapping);
			return true;
		}
	}

	return false;
}

bool UM_InputManager::RebindAction(const FName& _actionName, const FKey& _oldKey, const FKey& _newKey)
{
	if (_oldKey == _newKey) return false;
	if (!InputSettings) return false;

	const TArray<FKey> _usedKeys = GetUsedInputKeys();
	if (_usedKeys.Contains(_newKey)) return false;

	for (TTuple<FName, FInputActionData*> _inputAction : GameInput.InputActionData)
	{
		if (_inputAction.Key == _actionName && _inputAction.Value->Rebindable)
		{
			FInputActionKeyMapping _oldMapping;
			if (!_inputAction.Value->GetMapping(_oldMapping, _oldKey)) return false;				//If Mapping does not exist abord Rebind

			if (!_inputAction.Value->UpdateKeyMapping(_oldKey, _newKey)) return false;				//If old Input Key does not exist in InputKey or Mappings abort rebind

			FInputActionKeyMapping _newMapping;
			if (!_inputAction.Value->GetMapping(_newMapping, _newKey)) return false;				//If previous Update Key Mapping failed abort rebind (Should not happen in this context, bugs can appear if happen)

			InputSettings->RemoveActionMapping(_oldMapping, false);				//Remove Mapping with the old Key (do not ForceRebuildKeymaps to avoid Action Binding removed from Input Settings)
			InputSettings->AddActionMapping(_newMapping, false);					//Add the Mapping with the new Key
			InputSettings->ForceRebuildKeymaps();													//Rebuild the Input Settings Keymaps

			onInputActionRebind.Broadcast(*_inputAction.Value, _oldMapping, _newMapping);
			return true;
		}
	}
	return false;
}
bool UM_InputManager::RebindAction(const FName& _actionName, const FInputChord& _oldKey, const FInputChord& _newKey)
{
	if (_oldKey == _newKey) return false;
	if (!InputSettings) return false;

	const TArray<FInputChord> _usedKeys = GetUsedActionInputChords();
	if (_usedKeys.Contains(_newKey)) return false;

	for (TTuple<FName, FInputActionData*> _inputAction : GameInput.InputActionData)
	{
		if (_inputAction.Key == _actionName && _inputAction.Value->Rebindable)
		{
			FInputActionKeyMapping _oldMapping;
			if (!_inputAction.Value->GetMapping(_oldMapping, _oldKey)) return false;				//If Mapping does not exist abord Rebind

			if (!_inputAction.Value->UpdateInputChordMapping(_oldKey, _newKey)) return false;		//If old Input Chord does not exist in Mappings abort rebind

			FInputActionKeyMapping _newMapping;
			if (!_inputAction.Value->GetMapping(_newMapping, _newKey)) return false;				//If previous Update Key Mapping failed abort rebind (Should not happen in this context, bugs can appear if happen)

			InputSettings->RemoveActionMapping(_oldMapping, false);				//Remove Mapping with the old Key (do not ForceRebuildKeymaps to avoid Action Binding removed from Input Settings)
			InputSettings->AddActionMapping(_newMapping, false);					//Add the Mapping with the new Key
			InputSettings->ForceRebuildKeymaps();													//Rebuild the Input Settings Keymaps

			onInputActionRebind.Broadcast(*_inputAction.Value, _oldMapping, _newMapping);
			return true;
		}
	}

	return false;
}

bool UM_InputManager::RebindAxis(const FName& _axisName, const FKey& _oldKey, const FKey& _newKey)
{
	if (_oldKey == _newKey) return false;
	if (!InputSettings) return false;

	const TArray<FKey> _usedKeys = GetUsedInputKeys();
	if (_usedKeys.Contains(_newKey)) return false;

	for (TTuple<FName, FInputAxisData*> _inputAxis : GameInput.InputAxisData)
	{
		if (_inputAxis.Key == _axisName && _inputAxis.Value->Rebindable)
		{
			FInputAxisKeyMapping _oldMapping;
			if (!_inputAxis.Value->GetMapping(_oldMapping, _oldKey)) return false;				//If Mapping does not exist abord Rebind

			if (!_inputAxis.Value->UpdateKeyMapping(_oldKey, _newKey)) return false;				//if Old Input Key does not exist in InputKey or Mappings abort rebind

			FInputAxisKeyMapping _newMapping;
			if (!_inputAxis.Value->GetMapping(_newMapping, _newKey)) return false;				//If previous Update Key Mapping failed abort rebind (Should not happen in this context, bugs can appear if happen)

			InputSettings->RemoveAxisMapping(_oldMapping, false);			//Remove Mapping with the old Key (do not ForceRebuildKeymaps to avoid Axis Binding removed from Input Settings)
			InputSettings->AddAxisMapping(_newMapping, false);				//Add the Mapping with the new Key
			InputSettings->ForceRebuildKeymaps();											//Rebuild the Input Settings Keymaps

			onInputAxisRebind.Broadcast(*_inputAxis.Value, _oldMapping, _newMapping);
			return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region Initialize
void UM_InputManager::InitializeGameInput()
{
	InputSettings = UInputSettings::GetInputSettings();

	LoadInputData();

	InitializeAction();
	InitializeAxis();
}

void UM_InputManager::InitializeAction()
{
	if (!InputSettings) return;

	for (const FInputActionData& _inputAction : GameInput.GetInputActionData())
		for (const FInputActionKeyMapping& _actionMapping : _inputAction.Mappings)
			InputSettings->AddActionMapping(_actionMapping, false);

	InputSettings->ForceRebuildKeymaps();
}
void UM_InputManager::InitializeAxis()
{
	if (!InputSettings) return;

	for (const FInputAxisData& _inputAxis : GameInput.GetInputAxisData())
		for (const FInputAxisKeyMapping& _axisMapping : _inputAxis.Mappings)
			InputSettings->AddAxisMapping(_axisMapping, false);

	InputSettings->ForceRebuildKeymaps();
}
#pragma endregion

#pragma region Terminate
void UM_InputManager::TerminateGameInput()
{
	SaveInputData();

	TerminateAction();
	TerminateAxis();
}

void UM_InputManager::TerminateAction()
{
	if (!InputSettings) return;

	for (const FInputActionData& _inputAction : GameInput.GetInputActionData())
		for (const FInputActionKeyMapping& _actionMapping : _inputAction.Mappings)
			InputSettings->RemoveActionMapping(_actionMapping);
}
void UM_InputManager::TerminateAxis()
{
	if (!InputSettings) return;

	for (const FInputAxisData& _inputAxis : GameInput.GetInputAxisData())
		for (const FInputAxisKeyMapping& _axisMapping : _inputAxis.Mappings)
			InputSettings->RemoveAxisMapping(_axisMapping);
}
#pragma endregion

#pragma region Load/Save
void UM_InputManager::LoadDefaultGameInput()
{
	if (DefaultGameInputDataTable)
		GameInput = DefaultGameInputDataTable->GetGameInput(DefaultInputIsQwerty);
	else
		GameInput = FGameInput();

	GameInput.GenerateMappings();
}
void UM_InputManager::LoadSavedGameInput()
{
	if (InputSave)
	{
		GameInput = InputSave->GetSavedGameInput();
		GameInput.GenerateMappings();
	}
	else
		LoadDefaultGameInput();
}

void UM_InputManager::LoadInputData()
{
#if WITH_EDITOR
	if (!UseSave)
	{
		LoadDefaultGameInput();
		return;
	}
#endif

	//if Save exist
	//Load Save
	//Else
	//Create Save, Load Default InputData & Generate Mappings

	if (UGameplayStatics::DoesSaveGameExist(InputSaveSlot, 0))
	{
		InputSave = Cast<US_InputSave>(UGameplayStatics::LoadGameFromSlot(InputSaveSlot, 0));
		if (InputSave)
			LoadSavedGameInput();
		else
			LoadDefaultGameInput();
	}
	else
	{
		InputSave = Cast<US_InputSave>(UGameplayStatics::CreateSaveGameObject(US_InputSave::StaticClass()));

		LoadDefaultGameInput();

		SaveInputData();
	}
}
void UM_InputManager::SaveInputData() const
{
	if (InputSave)
	{
		InputSave->SetSavedGameInput(GameInput);
		UGameplayStatics::SaveGameToSlot(InputSave, InputSaveSlot, 0);
	}
}
#pragma endregion

#pragma region Reset
void UM_InputManager::ResetInputToDefault()
{
	if (!InputSettings || !DefaultGameInputDataTable) return;

	TArray<FInputActionData> _defaultGameInputActionData;
	TArray<FInputAxisData> _defaultGameInputAxisData;

	FGameInput _defaultGameInput = DefaultGameInputDataTable->GetGameInput(GameInput.IsQwerty);				//Get the Default Game Input Data
	_defaultGameInput.GenerateMappings(_defaultGameInputActionData, _defaultGameInputAxisData);		//Generate Default Action/Axis Mappings

	ResetAction(_defaultGameInputActionData);			//Reset current Game Input Action with Default Game Input Action Data
	ResetAxis(_defaultGameInputAxisData);				//Reset current Game Input Axis with Default Game Input Axis Data

	GameInput = _defaultGameInput;						//Set current Game Input with Default
	GameInput.GenerateMappings();

	onRebindableInputActionReset.Broadcast(GetRebindableInputAction());
	onInputActionReset.Broadcast(GameInput.GetInputActionData());

	onRebindableInputAxisReset.Broadcast(GetRebindableInputAxis());
	onInputAxisReset.Broadcast(GameInput.GetInputAxisData());

	onInputReset.Broadcast(this);
}

void UM_InputManager::SwitchKeyboardInputMode(const bool _qwerty)
{
	GameInput.IsQwerty = _qwerty;
	ResetInputToDefault();
}

void UM_InputManager::ResetAction(const TArray<FInputActionData>& _defaultActionData)
{
	if (!InputSettings) return;

	const TArray<FInputActionData> _axisData = GameInput.GetInputActionData();
	const int _max = _axisData.Num();
	for (int i = 0; i < _max; i++)
	{
		for (const FInputActionKeyMapping& _mappingToRemove : _axisData[i].Mappings)				//Remove All Action Mapping
			InputSettings->RemoveActionMapping(_mappingToRemove, false);

		for (const FInputActionKeyMapping& _mappingToAdd : _defaultActionData[i].Mappings)			//Add Default Action Mapping
			InputSettings->AddActionMapping(_mappingToAdd, false);
	}
	InputSettings->ForceRebuildKeymaps();
}
void UM_InputManager::ResetAxis(const TArray<FInputAxisData>& _defaultAxisData)
{
	if (!InputSettings) return;

	const TArray<FInputAxisData> _axisData = GameInput.GetInputAxisData();
	const int _max = _axisData.Num();
	for (int i = 0; i < _max; i++)
	{
		for (const FInputAxisKeyMapping& _mappingToRemove : _axisData[i].Mappings)					//Remove All Axis Mapping
			InputSettings->RemoveAxisMapping(_mappingToRemove, false);

		for (const FInputAxisKeyMapping& _mappingToAdd : _defaultAxisData[i].Mappings)				//Add Default Axis Mapping
			InputSettings->AddAxisMapping(_mappingToAdd, false);
	}
	InputSettings->ForceRebuildKeymaps();
}
#pragma endregion