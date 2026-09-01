#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ESH_GameState : uint8
{
	None = 0    UMETA(DisplayName = "None"),
	Game		UMETA(DisplayName = "Game"),
};