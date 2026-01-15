
#pragma once

#include "StateTreePropertyRef.h"
#include "StateTreeTaskBase.h"
#include "StateTreeExecuteActionTask.generated.h"

class ATDWAIController;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeExecuteActionTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<ATDWAIController> AIController = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag ActionTag;

	/** If true, the task will return Succeeded status upon execution, otherwise it will return Running. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bShouldSucceed = true;
};

/**
 * Task that makes the AI Controller execute an action based on the provided ActionTag.
 */
USTRUCT(meta = (DisplayName = "Execute Action Task", Category = "TDW"))
struct THEDARKWEST_API FStateTreeExecuteActionTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeExecuteActionTaskInstanceData;

	FStateTreeExecuteActionTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
	