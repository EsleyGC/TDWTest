
#pragma once

#include "StateTreePropertyRef.h"
#include "StateTreeTaskBase.h"
#include "StateTreeApplyGameplayTagTask.generated.h"

class ATDWCharacter;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeApplyGameplayTagTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<ATDWCharacter> Character = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTagContainer TagsToApply;
};

/**
 * Task that applies a gameplay tag when entering a state, and removes the tag when exiting.
 */
USTRUCT(meta = (DisplayName = "Apply Tag Task", Category = "TDW"))
struct THEDARKWEST_API FStateTreeApplyGameplayTagTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeApplyGameplayTagTaskInstanceData;

	FStateTreeApplyGameplayTagTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
	