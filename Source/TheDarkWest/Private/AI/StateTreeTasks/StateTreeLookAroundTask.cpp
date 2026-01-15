
#include "StateTreeLookAroundTask.h"

#include "StateTreeExecutionContext.h"
#include "AI/TDWAIController.h"
#include "Kismet/KismetSystemLibrary.h"

EStateTreeRunStatus FStateTreeLookAroundTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!IsValid(InstanceData.AIController))
	{
		return EStateTreeRunStatus::Failed;		
	}
	
	if (InstanceData.LookAroundDuration > 0.0f)
	{
		const float RandomRange = FMath::FRandRange(
			FMath::Max(0.0f, InstanceData.LookAroundDuration - InstanceData.LookAroundDurationDeviation),
			(InstanceData.LookAroundDuration + InstanceData.LookAroundDurationDeviation));
		InstanceData.TaskDuration = RandomRange;
	}
	else
	{
		InstanceData.TaskDuration = 0.0f; // Run indefinitely
	}

	if (InstanceData.bUseStartingDirectionAsBase)
	{
		const FVector CurrentLocation = InstanceData.AIController->GetPawn()->GetActorLocation();
		const FVector CurrentTargetLocation = InstanceData.AIController->GetLookAtPosition();
		FVector DirectionToTarget = (CurrentTargetLocation - CurrentLocation).GetSafeNormal2D();
		if (DirectionToTarget.IsNearlyZero())
		{
			DirectionToTarget = InstanceData.AIController->GetPawn()->GetActorForwardVector();
		}
		
		InstanceData.StartingDirection = DirectionToTarget;		
	}
	
	InstanceData.StartingTime = Context.GetWorld()->GetTimeSeconds();

	const float RandomAngleSize = FMath::FRandRange(
		FMath::Max(0.0f, InstanceData.LookAroundAngleSize - InstanceData.LookAroundAngleSizeDeviation),
		(InstanceData.LookAroundAngleSize + InstanceData.LookAroundAngleSizeDeviation));
	InstanceData.AngleSize = RandomAngleSize;
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStateTreeLookAroundTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	float& ElapsedTime = InstanceData.ElapsedTime;
	ElapsedTime += DeltaTime;
	if (InstanceData.TaskDuration > 0.0f)
	{
		const float TaskDuration = InstanceData.TaskDuration;
		if (ElapsedTime >= TaskDuration)
		{
			if (InstanceData.bUseStartingDirectionAsBase && InstanceData.bLookBackToStartingDirectionOnFinish)
			{
				if (IsValid(InstanceData.AIController) && IsValid(InstanceData.AIController->GetPawn()))
				{					
					const APawn* Pawn = InstanceData.AIController->GetPawn();
					const float TimeSinceEnd = ElapsedTime - InstanceData.TaskDuration;
					const float NormalizedTime = FMath::Clamp(TimeSinceEnd / InstanceData.LookBackToStartingDirectionDuration, 0.0f, 1.0f);
					if (NormalizedTime >= 1.0f)
					{
						const FVector CurrentLocation = Pawn->GetActorLocation();
						const FVector TargetLookAtPosition = CurrentLocation + (InstanceData.StartingDirection * 1000.0f);
						InstanceData.AIController->SetLookAtLocation(TargetLookAtPosition);
						return EStateTreeRunStatus::Succeeded;
					}
					else
					{
						const float AngleOffset = FMath::Lerp(InstanceData.LastSetOffSet, 0.0f, NormalizedTime);
						const FRotator RotationOffset(0.0f, AngleOffset, 0.0f);
						const FVector LookDirection = RotationOffset.RotateVector(InstanceData.StartingDirection);
						const FVector CurrentLocation = Pawn->GetActorLocation();
						const FVector TargetLookAtPosition = CurrentLocation + (LookDirection * 1000.0f);
						InstanceData.AIController->SetLookAtLocation(TargetLookAtPosition);
						return EStateTreeRunStatus::Running;
					}
				}
			}
			
			return EStateTreeRunStatus::Succeeded;
		}
	}

	if (!IsValid(InstanceData.AIController))
	{
		return EStateTreeRunStatus::Failed;		
	}

	const APawn* Pawn = InstanceData.AIController->GetPawn();
	if (!IsValid(Pawn))
	{
		return EStateTreeRunStatus::Failed;
	}

	const float StartTime = InstanceData.StartingTime;
	const float MultipliedElapsedTime = ElapsedTime * InstanceData.LookAroundAngleFrequency;
	const float TimeToUse = StartTime + MultipliedElapsedTime;
	
	const float SineValue = FMath::Sin((TimeToUse) * 2.0f * PI);
	const float PerlinNoiseValue = FMath::PerlinNoise1D(TimeToUse);
	const float CombinedValue = FMath::Lerp(SineValue, PerlinNoiseValue, InstanceData.RandomnessAmount);
	const float TargetAngleOffset = (CombinedValue * InstanceData.AngleSize) + InstanceData.AngleOffSet;
	float AngleOffset = TargetAngleOffset;
	
	if (ElapsedTime < InstanceData.FirstLookAtSmoothingTime)
	{
		const float NormalizedTime = FMath::Clamp(ElapsedTime / InstanceData.FirstLookAtSmoothingTime, 0.0f, 1.0f);
		AngleOffset = FMath::Lerp(0, TargetAngleOffset, NormalizedTime);
	}

	if (CVarStateTreeDebugLookAroundEnable.GetValueOnGameThread())
	{
		UKismetSystemLibrary::PrintString(InstanceData.AIController, FString::Printf(TEXT(
			"SineValue: %f, PerlinNoiseValue: %f, CombinedValue: %f, AngleOffset: %f (%f)"),
			SineValue, PerlinNoiseValue, CombinedValue, AngleOffset, TargetAngleOffset), true, true, FColor::Cyan, 100.f, TEXT("LookAroundDebug"));
	}
	
	const FRotator RotationOffset(0.0f, AngleOffset, 0.0f);
	FVector LookDirection;
	if (InstanceData.bUseStartingDirectionAsBase)
	{
		LookDirection = RotationOffset.RotateVector(InstanceData.StartingDirection);
	}
	else
	{
		const FVector CurrentForward = Pawn->GetActorForwardVector();
		LookDirection = RotationOffset.RotateVector(CurrentForward);
	}

	const FVector CurrentLocation = Pawn->GetActorLocation();
	const FVector DesiredLookAtPosition = CurrentLocation + (LookDirection * 1000.0f);
	const FVector TargetLookAtPosition = DesiredLookAtPosition;
	
	InstanceData.AIController->SetLookAtLocation(TargetLookAtPosition);
	InstanceData.LastSetOffSet = AngleOffset;
	
	return EStateTreeRunStatus::Running;
}
