
#pragma once

#include "StateTreePropertyRef.h"
#include "StateTreeTaskBase.h"
#include "StateTreeIncreaseCountTask.generated.h"

class ATDWAIController;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeIncreaseCountTaskInstanceData
{
	GENERATED_BODY()

	// Ref from an integer variable in the StateTree
	// This variable will be increased each time the task is executed
	UPROPERTY(EditAnywhere, Category = Out, meta = (RefType = "int32"))
	FStateTreePropertyRef OutIntValueRef;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bOverrideValue = false;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "bOverrideValue"))
	int32 OverrideValue = 0;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "!bOverrideValue"))
	int32 IncreaseBy = 1;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bShouldIncreaseOnEnter = true;
};

/**
 * Increases a count variable in the AI controller each time the task is executed.
 * This can be used to track how many times a certain state has been entered or how many
 * actions have been performed.
 */
USTRUCT(meta = (DisplayName = "Increase Count Task", Category = "TDW"))
struct THEDARKWEST_API FStateTreeIncreaseCountTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeIncreaseCountTaskInstanceData;

	FStateTreeIncreaseCountTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
	