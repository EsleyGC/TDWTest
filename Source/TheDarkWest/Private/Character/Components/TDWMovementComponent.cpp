// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Components/TDWMovementComponent.h"

void UTDWMovementComponent::RequestPathMove(const FVector& MoveInput)
{
	const FVector NormalizedInput = MoveInput.GetSafeNormal2D();
	AddInputVector(NormalizedInput);
}

void UTDWMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	const FVector NormalizedMoveVelocity = MoveVelocity.GetSafeNormal2D();
	AddInputVector(NormalizedMoveVelocity);
}

void UTDWMovementComponent::StopMovementKeepPathing()
{
	AddInputVector(FVector::ZeroVector);
}

float UTDWMovementComponent::GetMaxSpeed() const
{
	return MaxWalkSpeed;
}

void UTDWMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
	Super::AddInputVector(WorldVector, bForce);
}

