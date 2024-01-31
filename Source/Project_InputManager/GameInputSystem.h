#pragma once

#include "GameInputSystem.generated.h"

USTRUCT(BlueprintType)
struct FInputActionData
{
	GENERATED_BODY()

	//Name of the Action
	UPROPERTY(VisibleAnywhere)
	FName ActionName = "Action";

	//Define if the Action can be Rebind or not
	UPROPERTY(EditAnywhere)
	bool Rebindable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInputActionKeyMapping> Mappings = { };

	FInputActionData() { }

	FInputActionData(const FName& _actionName) : ActionName(_actionName) { }

	FInputActionData(const FName& _actionName, const bool _rebindable) : ActionName(_actionName), Rebindable(_rebindable) { }

	FInputActionData(const FName& _actionName, const TArray<FInputChord>& _keys, const bool _rebindable = false) : ActionName(_actionName), Rebindable(_rebindable)
	{
		for (const FInputChord& _key : _keys)
			Mappings.Add(FInputActionKeyMapping(ActionName, _key.Key, _key.bShift, _key.bCtrl, _key.bAlt, _key.bCmd));
	}

	//Generate all Action Mappings
	const TArray<FInputActionKeyMapping>& GenerateActionMappings()
	{
		for (FInputActionKeyMapping& _mapping : Mappings)
			_mapping.ActionName = ActionName;

		return Mappings;
	}

	//Get a Mapping associated with the given Key (if exist). Doesn't check Shift/Control/Command/Alt Keys 
	bool GetMapping(FInputActionKeyMapping& _mappingRef, const FKey& _key)
	{
		for (const FInputActionKeyMapping& _mapping : Mappings)
		{
			if (_mapping.Key == _key)
			{
				_mappingRef = _mapping;
				return true;
			}
		}
		return false;
	}

	//Get a Mapping associated with the given InputChord (if exist)
	bool GetMapping(FInputActionKeyMapping& _mappingRef, const FInputChord& _key)
	{
		for (const FInputActionKeyMapping& _mapping : Mappings)
		{
			if (_mapping == FInputActionKeyMapping(_mapping.ActionName, _key.Key, _key.bShift, _key.bCtrl, _key.bAlt, _key.bCmd))
			{
				_mappingRef = _mapping;
				return true;
			}
		}
		return false;
	}

	//Allow to Update a Key in Mappings (if exist)
	bool UpdateKeyMapping(const FKey& _oldKey, const FKey& _newKey)
	{
		for (FInputActionKeyMapping& _mapping : Mappings)		//Go to Mapping associated to the Key
		{
			if (_mapping.Key == _oldKey)						//Once Mapping is found...
			{
				_mapping.Key = _newKey;							//Change Key in Mapping
				return true;									//Sucess
			}
		}

		return false;											//Failed. Key does not exist in Mappings
	}

	//Allow to Update a InputChord in Mappings (if exist)
	bool UpdateInputChordMapping(const FInputChord& _oldKey, const FInputChord& _newKey)
	{
		for (FInputActionKeyMapping& _mapping : Mappings)		//Go to Mapping associated to the Key
		{
			if (FInputChord(_mapping.Key, _mapping.bShift, _mapping.bCtrl, _mapping.bAlt, _mapping.bCmd) == _oldKey)	//Once Mapping is found...
			{
				_mapping.Key = _newKey.Key;						//Change Input Chord in Mapping
				_mapping.bShift = _newKey.bShift;
				_mapping.bCtrl = _newKey.bCtrl;
				_mapping.bAlt = _newKey.bAlt;
				_mapping.bCmd = _newKey.bCmd;
				return true;									//Sucess
			}
		}

		return false;											//Failed. Key does not exist in Mappings
	}
};

USTRUCT(BlueprintType)
struct FInputAxisData
{
	GENERATED_BODY()

	//Name of the Axis
	UPROPERTY(VisibleAnywhere)
	FName AxisName = "Axis";

	//Define if the Axis can be Rebind or not
	UPROPERTY(EditAnywhere)
	bool Rebindable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInputAxisKeyMapping> Mappings = { };

	FInputAxisData() { }

	FInputAxisData(const FName& _axisName) : AxisName(_axisName) { }

	FInputAxisData(const FName& _axisName, const bool _rebindable) : AxisName(_axisName), Rebindable(_rebindable) { }

	FInputAxisData(const FName& _axisName, const TArray<TTuple<FKey, float>>& _keys, const bool _rebindable = false) : AxisName(_axisName), Rebindable(_rebindable)
	{
		for (const TTuple<FKey, float>& _key : _keys)
			Mappings.Add(FInputAxisKeyMapping(AxisName, _key.Key, _key.Value));
	}

	//Generate all Axis Mappings
	const TArray<FInputAxisKeyMapping>& GenerateAxisMappings()
	{
		for (FInputAxisKeyMapping& _mapping : Mappings)
			_mapping.AxisName = AxisName;

		return Mappings;
	}

	//Get a Mapping associated with the given Key (if exist)
	bool GetMapping(FInputAxisKeyMapping& _mappingRef, const FKey& _key)
	{
		for (const FInputAxisKeyMapping& _mapping : Mappings)
		{
			if (_mapping.Key == _key)
			{
				_mappingRef = _mapping;
				return true;
			}
		}
		return false;
	}

	//Allow to Update a Key in Mappings (if exist)
	bool UpdateKeyMapping(const FKey& _oldKey, const FKey& _newKey)
	{
		for (FInputAxisKeyMapping& _mapping : Mappings)		//Get to Mapping associated to the Key
		{
			if (_mapping.Key == _oldKey)						//Once Mapping is found...
			{
				_mapping.Key = _newKey;							//Change Key in Mapping
				return true;									//Sucess
			}
		}

		return false;											//Failed. Key does not exist in Mappings
	}
};
