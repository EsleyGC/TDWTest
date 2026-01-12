// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TDWMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEDARKWEST_API UTDWMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	virtual void RequestPathMove(const FVector& MoveInput) override;
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	virtual void StopMovementKeepPathing() override;
	virtual float GetMaxSpeed() const override;
	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override;
};
