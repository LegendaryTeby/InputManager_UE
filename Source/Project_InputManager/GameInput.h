#pragma once

#include "GameInputPreset.h"

#include "GameInputSystem.h"

#include "GameInput.generated.h"

USTRUCT(BlueprintType)
struct FGameInput
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inputs | Mode")
	bool IsQwerty = false;

	#pragma region Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Inputs")
	FInputActionData JumpAction = FInputActionData("Jump", ACTION_PRESET_JUMP, true);
	
	//...
	#pragma endregion

	#pragma region Axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axis Inputs")
	FInputAxisData VerticalMovementAxis = FInputAxisData("VerticalMovement",		AXIS_PRESET_VERTICAL_MOVEMENT(IsQwerty),	true);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axis Inputs")
	FInputAxisData HorizontalMovementAxis = FInputAxisData("HorizontalMovement",	AXIS_PRESET_HORIZONTAL_MOVEMENT(IsQwerty),	true);
	
	//...
	#pragma endregion
	
	TMap<FName, FInputActionData*> InputActionData = { };
	TMap<FName, FInputAxisData*> InputAxisData = { };
	
	FGameInput() { }

	FGameInput(const bool _isQwerty) : IsQwerty(_isQwerty) { }

	// Generate each Action & Axis Mappings
	void GenerateMappings()
	{
		GenerateActionMappings();
		GenerateAxisMappings();
	}

	// Generate each Action & Axis Mappings
	void GenerateMappings(TArray<FInputActionData>& _actionInputs, TArray<FInputAxisData>& _axisInputs)
	{
		GenerateActionMappings();
		_actionInputs = GetInputActionData();
		
		GenerateAxisMappings();
		_axisInputs = GetInputAxisData();
	}
		
	TArray<FInputActionData> GetInputActionData() const
	{
		TArray<FInputActionData> _actionData = { };
		for (const TTuple<FName, FInputActionData*>& _action : InputActionData)
		{
			_actionData.Add(*_action.Value);
		}

		return _actionData;
	}
	TArray<FInputAxisData> GetInputAxisData() const
	{
		TArray<FInputAxisData> _axisData = { };
		for (const TTuple<FName, FInputAxisData*>& _axis : InputAxisData)
		{
			_axisData.Add(*_axis.Value);
		}

		return _axisData;
	}

private:
	// Generate each Action Mappings and store them in Input Action Data
	void GenerateActionMappings()
	{
		InitInputActionData(JumpAction);

		// Add other Actions...
	}
	// Generate each Axis Mappings and store them in Input Axis Data
	void GenerateAxisMappings()
	{
		InitInputAxisData(VerticalMovementAxis);
		InitInputAxisData(HorizontalMovementAxis);

		// Add other Axis...
	}

	void InitInputActionData(FInputActionData& _action)
	{
		_action.GenerateActionMappings();
		InputActionData.Add(_action.ActionName, &_action);
	}
	void InitInputAxisData(FInputAxisData& _axis)
	{
		_axis.GenerateAxisMappings();
		InputAxisData.Add(_axis.AxisName, &_axis);
	}

public:
	// Replace Action Mappings (if name exist) with the Mappings of the given Action
	void UpdateActionMappings(const FInputActionData& _action)
	{
		if (!InputActionData.Contains(_action.ActionName)) return;
		InputActionData[_action.ActionName]->Mappings = _action.Mappings;
	}
	// Replace Axis Mappings (if name exist) with the Mappings of the given Axis
	void UpdateAxisMappings(const FInputAxisData& _axis)
	{
		if (!InputAxisData.Contains(_axis.AxisName)) return;
		InputAxisData[_axis.AxisName]->Mappings = _axis.Mappings;
	}
};
