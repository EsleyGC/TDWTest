// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "TDWMathLibrary.generated.h"

/**
 * 
 */
UCLASS()
class THEDARKWEST_API UTDWMathLibrary : public UKismetMathLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 *@brief Wraps a value to ensure it falls within a specified range.
	 *
	 *This method wraps the given value within the specified range defined by MinValue and MaxValue.
	 *If the value exceeds the upper bound (MaxValue), it cycles back to the lower bound (MinValue).
	 *
	 *@param CurrentValue The value to be wrapped.
	 *@param MinValue The lower bound of the range.
	 *@param MaxValue The upper bound of the range.
	 *@return The wrapped value within the specified range.
	 */	
	UFUNCTION(BlueprintPure, Category = "TDWMath")
	static float WrapToRange(float CurrentValue, float MinValue, float MaxValue);
};
