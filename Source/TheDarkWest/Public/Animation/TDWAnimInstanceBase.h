// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/TDWBaseEnums.h"
#include "TDWAnimInstanceBase.generated.h"

class UCharacterMovementComponent;
class ATDWCharacter;

/**
 * 
 */
UCLASS()
class THEDARKWEST_API UTDWAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void UpdateRotationValues(const float DeltaTime);
	void UpdateVelocities();
	void UpdateIsMoving(const float DeltaSeconds);
	void UpdateLookAt(float DeltaSeconds);
	void UpdateCurrentDirectionAngle();

	bool ShouldChangeCardinalDirection() const;
	ECardinalDirection SelectNewCardinalDirection() const;
	
	static FVector2D NormalizeToMajorAxis(const FVector& Input);

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Character")
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
	/**
	 * The angle threshold to determine if the character should keep the same cardinal direction
	 * or if it should change to the new one.
	 */
	UPROPERTY(EditAnywhere, Category = "TDW|Movement|Direction")
	float DirectionAngleThreshold = 30;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|LookAt")
	FVector LookAtLocationWorld;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Cardinals")
	ECardinalDirection CurrentCardinalDirection;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Cardinals")
	float CurrentDirectionAngle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Cardinals")
	float LastCardinalSelectionDirectionAngle;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Rotation")
	float TargetLookAtYaw;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Rotation")
	float AccumulatedRotationDelta;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Rotation")
	float CurrentRotationDelta;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	FVector WorldSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	FVector LocalSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	FVector LastNoZeroLocalSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	FVector2D MajorAxisLocalDirection;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	float SpeedSize;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	float NormalizedSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	float MaxSpeedSize;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	FVector WorldAcceleration;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	FVector LocalAcceleration;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|Velocities")
	float AccelerationSize;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "TDW|IsMoving")
	bool bIsMoving;
	
private:
	UPROPERTY()
	TObjectPtr<ATDWCharacter> TDWCharacter;
};
