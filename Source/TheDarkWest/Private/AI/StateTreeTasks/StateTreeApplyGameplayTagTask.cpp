
#include "StateTreeApplyGameplayTagTask.h"

#include "AbilitySystemComponent.h"
#include "HeadMountedDisplayTypes.h"
#include "StateTreeExecutionContext.h"
#include "AI/TDWAIController.h"

EStateTreeRunStatus FStateTreeApplyGameplayTagTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const ATDWCharacter* Character = InstanceData.Character;
	if (!IsValid(Character))
	{
		return EStateTreeRunStatus::Running;
	}
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTags(InstanceData.TagsToApply);	
	return EStateTreeRunStatus::Running;
}

void FStateTreeApplyGameplayTagTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const ATDWCharacter* Character = InstanceData.Character;
	if (!IsValid(Character))
	{
		return;
	}
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTags(InstanceData.TagsToApply);	
}
