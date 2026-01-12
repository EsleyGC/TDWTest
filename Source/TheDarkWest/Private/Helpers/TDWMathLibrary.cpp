// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/TDWMathLibrary.h"

float UTDWMathLibrary::WrapToRange(float CurrentValue, float MinValue, float MaxValue)
{
	const float Range = MaxValue - MinValue;
	float WrappedValue = FMath::Fmod(CurrentValue - MinValue, Range);
	WrappedValue = WrappedValue < 0 ? WrappedValue + Range : WrappedValue;
	return WrappedValue + MinValue;
}
