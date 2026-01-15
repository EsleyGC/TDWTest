
#include "StateTreeLookAtTask.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "AI/TDWAIController.h"
#include "Character/TDWCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeLookAtTask)

static TAutoConsoleVariable<bool> CVarStateTreeDebugLookAtEnable(TEXT("TDW.StateTree.Debug.LookAt"), false, TEXT("Enable/Disable Look At debug"));

EStateTreeRunStatus FStateTreeLookAtTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const AActor* TargetToLookAt = InstanceData.TargetToLookAt;
	
	if (!IsValid(TargetToLookAt) && InstanceData.TargetLocation.IsNearlyZero() && !InstanceData.bForceLookIntoCurrentDirection)
	{
		return EStateTreeRunStatus::Failed;
	}

	if (InstanceData.bStartTaskLookingToTarget)
	{
		InstanceData.LastTargetLocation = IsValid(TargetToLookAt) ? TargetToLookAt->GetActorLocation() : InstanceData.TargetLocation;
	}
	else
	{
		const ATDWCharacter* TDWCharacter = InstanceData.AIController->GetTDWCharacter();
		if (IsValid(TDWCharacter))
		{
			InstanceData.LastTargetLocation = TDWCharacter->GetCurrentLookAtLocation();
		}
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStateTreeLookAtTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ATDWAIController* AIController = InstanceData.AIController.Get();
	if (AIController == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	const AActor* TargetToLookAt = InstanceData.TargetToLookAt;
	bool bIsTargetValid = !InstanceData.bForceLookIntoCurrentDirection && IsValid(TargetToLookAt);
	if (!bIsTargetValid && InstanceData.TargetLocation.IsNearlyZero() && !InstanceData.bForceLookIntoCurrentDirection)
	{
		return EStateTreeRunStatus::Failed;
	}

	FVector LocationToLookAt = bIsTargetValid ? TargetToLookAt->GetActorLocation() : InstanceData.TargetLocation;
	if (InstanceData.bForceLookIntoCurrentDirection)
	{
		if (const ATDWCharacter* TDWCharacter = Cast<ATDWCharacter>(AIController->GetPawn()); IsValid(TDWCharacter))
		{
			if (const UCharacterMovementComponent* MovementComponent = TDWCharacter->GetCharacterMovement())
			{
				const FVector CurrentDirection = MovementComponent->Velocity.IsNearlyZero() ? TDWCharacter->GetActorForwardVector() : MovementComponent->Velocity.GetSafeNormal2D();
				LocationToLookAt = TDWCharacter->GetActorLocation() + (CurrentDirection * 100.0f);
			}
		}		
	}
	
	if (bIsTargetValid && InstanceData.bChangeLookAtSpeedBasedOnTags && !InstanceData.LookAtSpeedPerTag.IsEmpty())
	{
		const ATDWCharacter* TDWCharacter = Cast<ATDWCharacter>(TargetToLookAt);
		if (IsValid(TDWCharacter))
		{
			float InterpSpeed = InstanceData.CurrentLookAtSpeed;
			bool bFoundMatchingTag = false;
			for (const auto& TagSpeedPair : InstanceData.LookAtSpeedPerTag)
			{
				if (TDWCharacter->HasMatchingGameplayTag(TagSpeedPair.Key))
				{
					InterpSpeed = TagSpeedPair.Value;
					bFoundMatchingTag = true;
					break;
				}
			}
			
			if (!bFoundMatchingTag)
			{
				InterpSpeed = FMath::Lerp(InterpSpeed, 10.0f, DeltaTime * InstanceData.LookAtSpeedsBlendSpeed);
			}
			
			LocationToLookAt = FMath::Lerp(InstanceData.LastTargetLocation, LocationToLookAt, DeltaTime * InterpSpeed);
			InstanceData.CurrentLookAtSpeed = InterpSpeed;
			InstanceData.LastTargetLocation = LocationToLookAt;
		}
	}
	
	const FVector PreErrorLocationToLookAt = LocationToLookAt;
	if (InstanceData.LateralMaxError > 0.f)
	{
		float& ElapsedTime = InstanceData.ElapsedTime;
		ElapsedTime += DeltaTime * InstanceData.LateralErrorChangeSpeed;
		
		//Apply perlin noise to lateral error
		const float PerlinNoiseValue = FMath::PerlinNoise1D(ElapsedTime);
		const float CurrentError = PerlinNoiseValue * InstanceData.LateralMaxError;
		
		const FVector AICharacterLocation = AIController->GetPawn()->GetActorLocation();
		const FVector DirectionToLookAt = (LocationToLookAt - AICharacterLocation).GetSafeNormal2D();
		const FVector RightVector = FVector::CrossProduct(DirectionToLookAt, FVector::UpVector);
		LocationToLookAt += RightVector * CurrentError;
	}
	
	AIController->SetLookAtLocation(LocationToLookAt);

	if (CVarStateTreeDebugLookAtEnable.GetValueOnGameThread())
	{
		const APawn* AIPawn = AIController->GetPawn();
		if (IsValid(AIPawn))
		{
			const FVector UpVectorOffset = FVector(0, 0, 100);
			const FVector AICharacterLocation = AIController->GetPawn()->GetActorLocation();
			DrawDebugLine(Context.GetWorld(), AICharacterLocation + UpVectorOffset, LocationToLookAt + UpVectorOffset, FColor::Purple, false, DeltaTime, 0, 2.0f);
			DrawDebugSphere(Context.GetWorld(), PreErrorLocationToLookAt + UpVectorOffset, 10.f, 12, FColor::Orange, false, DeltaTime, 0, 2.f);
		}
	}
	
	return EStateTreeRunStatus::Running;
}
