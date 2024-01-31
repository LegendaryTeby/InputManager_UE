#pragma once

#pragma region Action Preset

#pragma region PRESET Jump																								//Jump
#define ACTION_KEYS_JUMP_GAMEPAD	FKey(EKeys::Gamepad_FaceButton_Bottom)												//Gamepad : A
#define ACTION_KEYS_JUMP_KEYBOARD	FKey(EKeys::SpaceBar)																//Keyboard : Space Bar

#define ACTION_PRESET_JUMP { ACTION_KEYS_JUMP_KEYBOARD, ACTION_KEYS_JUMP_GAMEPAD }
#pragma endregion

#pragma endregion

#pragma region Axis Preset

#pragma region Vertical Movement																						//Vertical Movement
#define AXIS_KEYS_VERTICAL_MOVEMENT_GAMEPAD { FKey(EKeys::Gamepad_LeftY), 1 }											//Gamepad : Left Thumbstick Y-Axis
#define AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_AZERTY { FKey(EKeys::Z), 1 }, { FKey(EKeys::S), -1 }						//Keyboard AZERTY : Z (1) S (-1)
#define AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_QWERTY { FKey(EKeys::W), 1 }, { FKey(EKeys::S), -1 }						//Keyboard QWERTY : W (1) S (-1)

#define AXIS_PRESET_VERTICAL_MOVEMENT_AZERTY { AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_AZERTY, AXIS_KEYS_VERTICAL_MOVEMENT_GAMEPAD }
#define AXIS_PRESET_VERTICAL_MOVEMENT_QWERTY { AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_QWERTY, AXIS_KEYS_VERTICAL_MOVEMENT_GAMEPAD }

#define AXIS_PRESET_VERTICAL_MOVEMENT(IsQwerty) IsQwerty ? TArray<TTuple<FKey, float>>() = AXIS_PRESET_VERTICAL_MOVEMENT_QWERTY : TArray<TTuple<FKey, float>>() = AXIS_PRESET_VERTICAL_MOVEMENT_AZERTY
#pragma endregion

#pragma region Horizontal Movement																						//Horizontal Movement
#define AXIS_KEYS_HORIZONTAL_MOVEMENT_GAMEPAD { FKey(EKeys::Gamepad_LeftX), 1 }											//Gamepad : Left Thumbstick X-Axis
#define AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_AZERTY { FKey(EKeys::Q), -1 }, { FKey(EKeys::D), 1 }						//Keyboard AZERTY : Q (-1) D (1)
#define AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_QWERTY { FKey(EKeys::A), -1 }, { FKey(EKeys::D), 1 }						//Keyboard QWERTY : W (-1) D (1)

#define AXIS_PRESET_HORIZONTAL_MOVEMENT_AZERTY { AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_AZERTY, AXIS_KEYS_HORIZONTAL_MOVEMENT_GAMEPAD }
#define AXIS_PRESET_HORIZONTAL_MOVEMENT_QWERTY { AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_QWERTY, AXIS_KEYS_HORIZONTAL_MOVEMENT_GAMEPAD }

#define AXIS_PRESET_HORIZONTAL_MOVEMENT(IsQwerty) IsQwerty ? TArray<TTuple<FKey, float>>() = AXIS_PRESET_HORIZONTAL_MOVEMENT_QWERTY : TArray<TTuple<FKey, float>>() = AXIS_PRESET_HORIZONTAL_MOVEMENT_AZERTY
#pragma endregion

#pragma endregion