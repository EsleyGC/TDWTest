
#pragma once

#include "StateTreeTaskBase.h"
#include "StateTreeLookAtTask.generated.h"

class ATDWAIController;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeLookAtTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ATDWAIController> AIController {nullptr};
	
	UPROPERTY(EditAnywhere, Category = "Input", meta=(Optional))
	AActor* TargetToLookAt {nullptr};
	
	UPROPERTY(EditAnywhere, Category = "Input", meta=(Optional))
	FVector TargetLocation {FVector::ZeroVector};	
	
	/** If true, the task will force the look at to the current direction of the character. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bForceLookIntoCurrentDirection {false};

	// If true, the task will start looking to the target immediately upon entering the state.
	// If false, the task will start from the last look at direction.
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bStartTaskLookingToTarget {false};
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bChangeLookAtSpeedBasedOnTags {false};
	
	UPROPERTY(EditAnywhere, Category = "Parameter", meta=(EditCondition="bChangeLookAtSpeedBasedOnTags", EditConditionHides))
	float LookAtSpeedsBlendSpeed {1.f} ;
	
	UPROPERTY(EditAnywhere, Category = "Parameter", meta=(EditCondition="bChangeLookAtSpeedBasedOnTags", EditConditionHides))
	TMap<FGameplayTag, float> LookAtSpeedPerTag;
	
	/** Lateral error to apply when looking at a target. Useful for making the look at less "perfect" */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float LateralMaxError = 0.f;
	
	/** Speed at which the lateral error changes over time */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta=(EditCondition="LateralMaxError > 0", EditConditionHides))
	float LateralErrorChangeSpeed = 1.f;

	float CurrentLookAtSpeed {0.f};
	FVector LastTargetLocation {FVector::ZeroVector};

	float ElapsedTime {0.f};
};

/**
 * Handles the look at target for the AI characters. Must have a valid location to look at, otherwise will look towards the movement direction.
 */
USTRUCT(meta = (DisplayName = "Look At Task", Category = "TDW"))
struct THEDARKWEST_API FStateTreeLookAtTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeLookAtTaskInstanceData;
	
	FStateTreeLookAtTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
