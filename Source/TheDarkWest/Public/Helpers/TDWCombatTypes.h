#pragma once

#include "CoreMinimal.h"
#include "TDWCombatTypes.generated.h"

/** Payload data structure for projectile info */
USTRUCT()
struct FProjectilePayload
{
	GENERATED_BODY()
	
	FProjectilePayload() = default;
	FProjectilePayload(AActor* InInstigatorActor, const FVector& InOrigin, const FVector& InTargetLocation, const float InDamageAmount)
		: InstigatorActor(InInstigatorActor), Origin(InOrigin), TargetLocation(InTargetLocation), DamageAmount(InDamageAmount)
	{
	}
	
	UPROPERTY()
	AActor* InstigatorActor = nullptr;
	
	UPROPERTY()
	FVector Origin = FVector::ZeroVector;
	
	UPROPERTY()
	FVector TargetLocation = FVector::ZeroVector;
	
	UPROPERTY()
	float DamageAmount = 0.0f;
};
