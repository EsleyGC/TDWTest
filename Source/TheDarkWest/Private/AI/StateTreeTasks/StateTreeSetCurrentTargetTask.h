
#pragma once

#include "StateTreePropertyRef.h"
#include "StateTreeTaskBase.h"
#include "StateTreeSetCurrentTargetTask.generated.h"

class ATDWAIController;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeSetCurrentTargetTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<ATDWAIController> AIController = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	AActor* ActorToSetAsTarget = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bShouldSucceed = false;
};

/**
 * Task that sets the new target for the AI Controller.
 */
USTRUCT(meta = (DisplayName = "Set Current Target", Category = "TDW"))
struct THEDARKWEST_API FStateTreeSetCurrentTargetTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeSetCurrentTargetTaskInstanceData;

	FStateTreeSetCurrentTargetTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
	