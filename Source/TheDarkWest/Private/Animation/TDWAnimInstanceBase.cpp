// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TDWAnimInstanceBase.h"

#include "Character/TDWCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UTDWAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TDWCharacter = Cast<ATDWCharacter>(TryGetPawnOwner());
	if (!IsValid(TDWCharacter))
	{
		return;
	}
	
	CharacterMovementComponent = TDWCharacter->GetCharacterMovement();
}

void UTDWAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsValid(TDWCharacter) || !IsValid(CharacterMovementComponent))
	{
		return;
	}
	
	UpdateRotationValues(DeltaSeconds);
	UpdateVelocities();
	UpdateIsMoving(DeltaSeconds);
	UpdateLookAt(DeltaSeconds);
	UpdateCurrentDirectionAngle();
	
	if (ShouldChangeCardinalDirection())
	{
		CurrentCardinalDirection = SelectNewCardinalDirection();
		LastCardinalSelectionDirectionAngle = CurrentDirectionAngle;
	}
}

void UTDWAnimInstanceBase::UpdateRotationValues(const float DeltaTime)
{	
	const FVector CurrentLocation = TDWCharacter->GetActorLocation();
	const FVector TargetDirection = (LookAtLocationWorld - CurrentLocation).GetSafeNormal2D();
	
	const float TargetYaw = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, TargetDirection).Yaw;
	const float CurrentMeshYaw = TDWCharacter->GetMesh()->GetBoneTransform("root", RTS_World).Rotator().Yaw + 90;
	
	CurrentRotationDelta = FMath::FindDeltaAngleDegrees(CurrentMeshYaw, TargetYaw);
	AccumulatedRotationDelta += CurrentRotationDelta * DeltaTime;
}

void UTDWAnimInstanceBase::UpdateVelocities()
{
	WorldSpeed = TDWCharacter->GetVelocity();
	SpeedSize = WorldSpeed.Size2D();
	MaxSpeedSize = TDWCharacter->GetCharacterMovement()->GetMaxSpeed();
	NormalizedSpeed = MaxSpeedSize == KINDA_SMALL_NUMBER ? 0.0f : SpeedSize / MaxSpeedSize;
	LocalSpeed = TDWCharacter->GetTransform().InverseTransformVectorNoScale(WorldSpeed);
	if (SpeedSize >= KINDA_SMALL_NUMBER)
	{
		LastNoZeroLocalSpeed = LocalSpeed;
	}

	MajorAxisLocalDirection = NormalizeToMajorAxis(LocalSpeed);
	WorldAcceleration = CharacterMovementComponent->GetCurrentAcceleration();
	LocalAcceleration = TDWCharacter->GetTransform().InverseTransformVectorNoScale(WorldAcceleration);
	AccelerationSize = WorldAcceleration.Size2D();
}

void UTDWAnimInstanceBase::UpdateIsMoving(const float DeltaSeconds)
{
	const bool bHasSpeed = AccelerationSize > KINDA_SMALL_NUMBER;
	const bool bIsMovingFast = SpeedSize > TDWCharacter->GetCharacterMovement()->GetMaxSpeed() * 0.75f;
	bIsMoving = bHasSpeed || bIsMovingFast;
}

void UTDWAnimInstanceBase::UpdateLookAt(float DeltaSeconds)
{
	const float HalfHeight = TDWCharacter->GetDefaultHalfHeight();
	FVector TargetLocation = TDWCharacter->GetCurrentLookAtLocation();
	TargetLocation.Z += HalfHeight;
	LookAtLocationWorld = TargetLocation;
}

void UTDWAnimInstanceBase::UpdateCurrentDirectionAngle()
{
	const FVector Velocity = WorldSpeed;
	if (Velocity.IsNearlyZero())
	{
		CurrentDirectionAngle = 0;
		return;
	}

	const FVector NormalVelocity = Velocity.GetSafeNormal2D();
	const FVector ForwardDirection = TDWCharacter->GetActorForwardVector().GetSafeNormal2D();
	
	const float Angle = FMath::Acos(FVector::DotProduct(ForwardDirection, NormalVelocity));
	const float CrossZ = FVector::CrossProduct(ForwardDirection, NormalVelocity).Z;

	const float Sign = CrossZ > 0 ? 1 : -1;
	const float AngleDegrees = FMath::RadiansToDegrees(Angle);

	const float FinalDirectionAngle = AngleDegrees * Sign;
	CurrentDirectionAngle = FinalDirectionAngle;
}

bool UTDWAnimInstanceBase::ShouldChangeCardinalDirection() const
{
	if (CurrentDirectionAngle == 0)
	{
		return false;
	}

	const float AngleDifference = (CurrentDirectionAngle - LastCardinalSelectionDirectionAngle);
	if (FMath::Abs(AngleDifference) > DirectionAngleThreshold)
	{
		return true;
	}

	return false;
}

ECardinalDirection UTDWAnimInstanceBase::SelectNewCardinalDirection() const
{
	if (FMath::Abs(CurrentDirectionAngle) < 45)
	{
		return ECardinalDirection::Forward;
	}
	
	if (FMath::Abs(CurrentDirectionAngle) > 135)
	{
		return ECardinalDirection::Backward;
	}

	return CurrentDirectionAngle > 0 ? ECardinalDirection::Right : ECardinalDirection::Left;
}

FVector2D UTDWAnimInstanceBase::NormalizeToMajorAxis(const FVector& Input)
{
	if (FMath::Abs(Input.X) > FMath::Abs(Input.Y))
	{
		return FVector2D(FMath::Sign(Input.X), 0.0f);
	}
	
	return FVector2D(0.0f, FMath::Sign(Input.Y));	
}