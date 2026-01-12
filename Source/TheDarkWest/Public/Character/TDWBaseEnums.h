#pragma once

#include "CoreMinimal.h"
#include "TDWBaseEnums.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class ECardinalDirection : uint8
{
	Forward			UMETA(DisplayName = "Forward"),
	Backward		UMETA(DisplayName = "Backward"),
	Left			UMETA(DisplayName = "Left"),
	Right			UMETA(DisplayName = "Right"),
};