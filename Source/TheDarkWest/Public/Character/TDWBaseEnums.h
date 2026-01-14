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

/** Struct to hold hit reaction data including direction and, in the future, type of hit reaction */
USTRUCT(Blueprintable, BlueprintType)
struct FTDWHitReactionData
{
	GENERATED_BODY()
	
	FTDWHitReactionData(){};
	FTDWHitReactionData(const ECardinalDirection InHitDirection)
	: HitDirection(InHitDirection)	
	{	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardinalDirection HitDirection {ECardinalDirection::Forward};
};